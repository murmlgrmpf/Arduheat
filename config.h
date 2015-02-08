#ifndef CONFIG_H
#define CONFIG_H

#include <SdFat.h>
#include <SdFatUtil.h>
#include <ArduinoJson.h>
#include <RTClib.h>

extern DateTime TimeNow;

// SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
//const uint8_t chipSelect = SS;
const uint8_t chipSelect = 04;

// File system object.
SdFat sd;

// Log file.
SdFile file;
// Config file.
SdFile ini;

boolean logging = false;

boolean sderror = false;
boolean fileerror =  false;

#define BufferSize 1000
#define LineSize 500

#define ConfigFileName "System.cfg"

// Error messages stored in flash.

#define errorSD(msg) errorSD_F(F(msg)) 

void errorSD_F(const __FlashStringHelper* msg) {
	Serial.println((msg));
	sderror = true;
}

#define errorFile(msg) errorFile_F(F(msg)) 

void errorFile_F(const __FlashStringHelper* msg) {
	Serial.print(msg);
	fileerror =  true;
	file.close();
}


void initSD(void) {
	// Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
	// breadboards.  use SPI_FULL_SPEED for better performance.
	if (!sd.begin(chipSelect, SPI_HALF_SPEED))
		errorSD("Failed to init SDCard.");
	else {
		// Reset Errors
		fileerror = false;
		sderror   = false;
	}
}

template <typename S, typename T>
void writeConfelement(S* ini, T* Obj, void(T::*func)(ArduinoJson::JsonObject&)) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	(Obj->*func)(root);
	
	root.printTo(*ini); ini->println();
}

int writeConf( cHeating* Heating) {
	if ((!sderror)&&(!fileerror)) {
		if (ini.open(ConfigFileName, O_CREAT | O_RDWR | O_TRUNC )) {
			writeConfelement(&ini, &(Heating->Rooms), &cRooms::getOffsetTime);
			writeConfelement(&ini, &(Heating->Rooms), &cRooms::getOffsetTemp);
			writeConfelement(&ini, &(Heating->Rooms), &cRooms::getRooms);
			writeConfelement(&ini, &(Heating->Burner),&cBurner::getSP);
			writeConfelement(&ini, &(Heating->Boiler),&cBoiler::getSP);
			writeConfelement(&ini, &(Heating->WarmWater),&cWarmWater::getSP);
			
			
			return ini.close();
		}
		else {
			errorFile("ini.open create failed.");
			return 0;
		}
	}
}

int readConfigLine(char* line, cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	int posReturn= 0;
	
	JsonObject& root = jsonBuffer.parseObject(line);
	
	if (!root.success()) {
		Serial.println(F("Reading config: parseObject() failed."));
		return 0;
	}
	
	// Set configuration parameters
	posReturn = Heating->Rooms.setOffsetTime(root);
	posReturn += Heating->Rooms.setOffsetTemp(root);
	posReturn += Heating->Rooms.setRooms(root);
	posReturn += Heating->Burner.setSP(root);
	posReturn += Heating->Boiler.setSP(root);
	posReturn += Heating->WarmWater.setSP(root);
	
	PgmPrint("Free RAM: "); // part of sdFatUtil
	Serial.println(FreeRam());
	
	if (posReturn>0) {
		Serial.print(posReturn);
		return posReturn;
	}
	else return 0;
}

bool readConf(cHeating* Heating) {
	if ((!sderror)&&(!fileerror)) {
		char line[LineSize];
		int n;
		int posReturn = 0 ;
		
		if(sd.exists(ConfigFileName)){
			if (ini.open(ConfigFileName, O_RDWR )){
				// read in standard parameters
				while ((n = ini.fgets(line,sizeof(line))) > 0){
					Serial.println(line); // Debug Output
					posReturn = readConfigLine(line, Heating);
				}
				
				return ini.close();
			}
			else {
				errorFile("Config file ini.open failed.");
				return 0;
			}
		}
		else {
			Serial.println(F("No System.cfg file found on SDCard.\n Writing config to SDCard."));
			return writeConf(Heating);
		}
	}
	else return 0;
}

//////////////Logfile//////////////////
void logPrintHeader(JsonObject& root ) {
	for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
		if (it->value.is<const JsonArray &>()) {
			int i = 0;
			JsonArray& data = root[it->key];
			
			for (JsonArray::iterator iter = data.begin(); iter!=data.end(); ++iter) {
				file.print(",");
				file.print(it->key);
				file.print(i++);
			}
		}
		else {
			file.print(",");
			file.print(it->key);
		}
	}
}

void logPrintData(JsonObject& root ) {
	for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
		
		if (it->value.is<const char *>()) {
			file.print(",");
			file.print(it->value.asString());
		}
		else if (it->value.is<double>()) {
			file.print(",");
			file.print((it->value.as<double>()));
		}
		else if (it->value.is<long>()) {
			file.print(",");
			file.print((it->value.as<long>()));
		}
		else if (it->value.is<JsonArray&>()) {
			JsonArray& data = root[it->key];
			int i = 0;
			for (JsonArray::iterator iter = data.begin(); iter!=data.end(); ++iter) {
				file.print(",");
				if (root[it->key][i].is<double>()){
					file.print(root[it->key][i++].as<double>());
				}
				else if (root[it->key][i].is<long>()){
					file.print(root[it->key][i++].as<long>());
				}
				else if (root[it->key][i].is<const char *>()){
					file.print(root[it->key][i++].asString());
				}
			}
		}
		else file.print(F(",unknown data type"));
	}
}

template <typename T>
void logWriteelement(T* Obj, boolean bHeaders) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Obj->getData(root);
	if (bHeaders) logPrintHeader(root);
	else logPrintData(root);
}

// Log a data record.
void logWrite(boolean bHeaders, cHeating* Heating) {
	if ((!sderror)&&(!fileerror)) {
		if (bHeaders) file.print(F("Time,millis"));
		else {
			file.print(TimeNow.year(), DEC); file.print(F("/"));
			file.print(TimeNow.month(), DEC); file.print(F("/"));
			file.print(TimeNow.day(), DEC); file.print(F(" "));
			file.print(TimeNow.hour(), DEC); file.print(F(":"));
			file.print(TimeNow.minute(), DEC); file.print(F(":"));
			file.print(TimeNow.second(), DEC); file.print(F(","));
			file.print(millis());
		}
		// Check SD Card
		if (!file.sync() || file.getWriteError()) errorFile("write error");
	}
	// Check again if SDCard was working to avoid that the program crashes upon missing sd card (removed during operation) or sd card error
	if ((!sderror)&&(!fileerror)) {
		// Write data to file.  Start with log time in micros.
		logWriteelement(&(Heating->Rooms),bHeaders);
		logWriteelement(Heating,bHeaders);
		logWriteelement(&(Heating->Burner),bHeaders);
		logWriteelement(&(Heating->Boiler),bHeaders);
		logWriteelement(&(Heating->WarmWater),bHeaders);
		logWriteelement(&(Heating->Solar),bHeaders);
		
		file.println();
		
		// Force data to SD and update the directory entry to avoid data loss.
		if (!file.sync() || file.getWriteError()) errorFile("write error");
	}
}


char* genFile(char* fileName) {
	int y = TimeNow.year();
	int m = TimeNow.month();
	int d = TimeNow.day();

	sprintf(fileName, "%02d", y);
	fileName[0] = fileName[2];
	fileName[1] = fileName[3];
	sprintf(fileName+2, "%02d", m);
	sprintf(fileName+4, "%02d", d);
	sprintf(fileName+6, "00.CSV");
	
	const uint8_t BASE_NAME_SIZE = 6;
	while (sd.exists(fileName)) {
		if (fileName[BASE_NAME_SIZE + 1] != '9') {
			fileName[BASE_NAME_SIZE + 1]++;
			} else if (fileName[BASE_NAME_SIZE] != '9') {
			fileName[BASE_NAME_SIZE + 1] = '0';
			fileName[BASE_NAME_SIZE]++;
			} else {
			errorFile("Can't create file name");
		}
	}
	if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) errorFile("Failed to create new log file.");
	return fileName;
}

void startLogging(cHeating* Heating) {
	if ((!sderror)&&(!fileerror)) {
		Serial.print(F("Logging to: "));
		char fileName[13];
		Serial.println(genFile(fileName));
		logWrite(true, Heating);
		Serial.println(F("Type any character to stop logging."));
		logging = true;
	}
	else Serial.println(F("Failed to start logging."));
}

void stopLogging() {
	if ((!sderror)&&(!fileerror)) {
		// Close file and stop.
		file.close();
		Serial.println(F("Done"));
		logging = false;
	}
	else Serial.println(F("Failed to stop logging."));
}


#endif