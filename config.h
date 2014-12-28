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

#define iniFileName "System.cfg"

// Error messages stored in flash.
//#define error(msg) error_P(PSTR(msg))
//------------------------------------------------------------------------------
void error_P(const char* msg) {
	//sd.errorHalt_P(msg);
	Serial.print(msg);
}

#define errorSD(msg) errorSD_F(F(msg)) 

void errorSD_F(const __FlashStringHelper* msg) {
	Serial.print((msg));
	sderror = true;
}

#define errorFile(msg) errorFile_F(F(msg)) 

void errorFile_F(const __FlashStringHelper* msg) {
	Serial.print(msg);
	fileerror =  true;
}


const uint8_t ANALOG_COUNT = 4;

//const char iniFileName[13] = "System.cfg";

void initSD(void) {
	// Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
	// breadboards.  use SPI_FULL_SPEED for better performance.
	if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
		//sd.initErrorHalt();
		errorSD("Failed to init SDCard.");
	}
	
}

int setObject(JsonObject& root) {
	long time;
	double latitude;
	double longitude;
	const char* sensor;
	double spTemp;
	
	if (root.containsKey("spTemp")) {
		spTemp = root["spTemp"];
		Serial.println(spTemp);
	}
	if (root.containsKey("time")) {
		time = root["time"];
		Serial.println(time);
	}
	
	if (root.containsKey("data")) {
		latitude = root["data"][0];
		longitude = root["data"][1];
		Serial.println(latitude, 6);
		Serial.println(longitude, 6);
	}
	
	if (root.containsKey("sensor")) {
		sensor = root["sensor"];
		Serial.println(sensor);
	}
	else {
		Serial.println(F("root does not contain key sensor"));
		return 0;
	}
	
	return 1;
}


void getObject(JsonObject& root) {
	root["sensor"] = "gps3";
	root["time"] = 1351824120;
	root["spTemp"] = double(millis())/1000.5;
	JsonArray& data = root.createNestedArray("data");
	data.add(48.756080, 6);  // 6 is the number of decimals to print
	data.add(2.302038, 6);   // if not specified, 2 digits are printed
}


void writeConfRoomsTimes(SdFile* ini,  cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Rooms.getOffsetTime(root);
	
	root.printTo(*ini); ini->println();
	PgmPrint("Free RAM: "); Serial.println(FreeRam());// part of sdFatUtil
}
void writeConfRoomsTemps(SdFile* ini,  cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Rooms.getOffsetTemp(root);
	
	root.printTo(*ini); ini->println();
}
void writeConfRooms(SdFile* ini,  cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Rooms.getRooms(root);
	
	root.printTo(*ini); ini->println();
}
void writeConfBurner(SdFile* ini,  cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Burner.getSP(root);
	
	root.printTo(*ini); ini->println();
}
void writeConfBoiler(SdFile* ini,  cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Boiler.getSP(root);
	
	root.printTo(*ini); ini->println();
}
void writeConfWarmWater(SdFile* ini, cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->WarmWater.getSP(root);
	
	root.printTo(*ini); ini->println();
}

int writeConf( cHeating* Heating) {
	// Delete existing config
	if (sd.exists(iniFileName)) {
		ini.open(iniFileName, O_CREAT | O_RDWR );
		ini.remove();
	}
	if (ini.open(iniFileName, O_CREAT | O_RDWR )) {
		
		writeConfRoomsTimes(&ini,  Heating);
		writeConfRoomsTemps(&ini,  Heating);
		writeConfRooms(&ini,  Heating);
		writeConfBurner(&ini,  Heating);
		writeConfBoiler(&ini,  Heating);
		writeConfWarmWater(&ini,  Heating);
		
		delay(500);
		ini.close();
		return 1;
	}
	else {
		errorFile("ini.open create failed");
		return 0;
	}
}

int readConfigLine(char* line, cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	int posReturn= 0;
	
	JsonObject& root = jsonBuffer.parseObject(line);
	
	if (!root.success()) {
		Serial.println(F("parseObject() failed"));
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
		//Serial.println(F("Success to read in object"));
		return posReturn;
	}
	else
	{
		//Serial.println(F("Failed to read in object"));
		return 0;
	}
}

int readConfig(cHeating* Heating) {
	char line[LineSize];
	int n;
	int posReturn = 0 ;
	
	if(sd.exists(iniFileName)){
		if (ini.open(iniFileName, O_RDWR )){
			// read in standard parameters
			while ((n = ini.fgets(line,sizeof(line))) > 0){
				Serial.println(line);
				posReturn = readConfigLine(line, Heating);
			}
			
			ini.close();
		}
		else {
			errorFile("Config file ini.open failed.");
			return 0;
		}
	}
	return 1;
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
		else {
			file.print(",");
			file.print(F("unknown data type"));
		}
	}
}

void logWriteRooms( cHeating* Heating, boolean bHeaders ) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Rooms.getData(root);
	if (bHeaders) logPrintHeader(root);
	else logPrintData(root);
}
void logWriteHeating( cHeating* Heating, boolean bHeaders ) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->getData(root);
	if (bHeaders) logPrintHeader(root);
	else logPrintData(root);
}
void logWriteBurner( cHeating* Heating, boolean bHeaders ) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Burner.getData(root);
	if (bHeaders) logPrintHeader(root);
	else logPrintData(root);
}
void logWriteBoiler( cHeating* Heating, boolean bHeaders ) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->Boiler.getData(root);
	if (bHeaders) logPrintHeader(root);
	else logPrintData(root);
}
void logWriteWarmWater( cHeating* Heating, boolean bHeaders ) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Heating->WarmWater.getData(root);
	if (bHeaders) logPrintHeader(root);
	else logPrintData(root);
}


// Log a data record.
void logWrite(boolean bHeaders, cHeating* Heating) {
	// Write data to file.  Start with log time in micros. , cWarmWater* WarmWater 
	if (bHeaders){
		file.print(F("Time,"));
		file.print(F("millis"));
	}
	else {
		file.print(TimeNow.year(), DEC); file.print('/');
		file.print(TimeNow.month(), DEC); file.print('/');
		file.print(TimeNow.day(), DEC); file.print(' ');
		file.print(TimeNow.hour(), DEC); file.print(':');
		file.print(TimeNow.minute(), DEC); file.print(':');
		file.print(TimeNow.second(), DEC); file.print(F(","));
		file.print(millis());
	}
	
	logWriteRooms(Heating, bHeaders);
	logWriteHeating(Heating, bHeaders);
	logWriteBurner(Heating, bHeaders);
	logWriteBoiler(Heating, bHeaders);
	logWriteWarmWater(Heating, bHeaders);
	
	file.println();
	
	// Force data to SD and update the directory entry to avoid data loss.
	if (!file.sync() || file.getWriteError()) errorFile("write error");
}


char fileName[13];
char* genFile(void) {
	
	int y = TimeNow.year();
	int m = TimeNow.month();
	int d = TimeNow.day();
	sprintf(fileName, "%02d", y);
	fileName[0] = fileName[2];
	fileName[1] = fileName[3];
	sprintf(fileName+2, "%02d%02d00.CSV", m, d);
	
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
	if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) errorFile("Failed file.open");
	return fileName;
}

void stopLogging() {
	// Close file and stop.
	file.close();
	Serial.read();
	Serial.println(F("Done"));
	logging = false;
	Serial.println(F("Type any character to start logging"));
}


#endif