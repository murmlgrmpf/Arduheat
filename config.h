#ifndef CONFIG_H
#define CONFIG_H

// Logging
#include <FileIO.h>
#include <Console.h>
// Rest Interface
#include <Bridge.h>
//#include <YunServer.h>
#include <YunClient.h>

#include <ArduinoJson.h>
#include <RTClib.h>

//extern YunServer server;
extern DateTime TimeNow;

boolean logging = false;

boolean sderror = false;
boolean fileerror =  false;

#define BufferSize 1000
#define LineSize 500

#define ConfigFileName "/mnt/sda1/arduino/www/data/System.cfg"
#define LogFilePath "/mnt/sda1/arduino/www/data/"
char LogFileName[13];

template <typename S, typename T>
void writeConfelement(S* ini, T* Obj, void(T::*func)(ArduinoJson::JsonObject&)) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	(Obj->*func)(root);
	
	root.printTo(*ini); ini->println();
        // Rest Api
        for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
        {
            String value;
            root[it->key].printTo(value);
            Bridge.put(String(it->key), value);
        }
}

int writeConf( cHeating* Heating) {
    File ini = FileSystem.open(ConfigFileName, FILE_WRITE);
		if (ini) {
			writeConfelement(&ini, &(Heating->Rooms), &cRooms::getOffsetTime);
			writeConfelement(&ini, &(Heating->Rooms), &cRooms::getOffsetTemp);
			writeConfelement(&ini, &(Heating->Rooms), &cRooms::getRooms);
			writeConfelement(&ini, &(Heating->Burner),&cBurner::getSP);
			writeConfelement(&ini, &(Heating->WarmWater),&cWarmWater::getSP);
			ini.close();
			return 1;
		}
		else {
			Console.print("ini.open create failed.");
			return 0;
		}
}

int readConfigLine(char* line, cHeating* Heating) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	int posReturn= 0;
	
	JsonObject& root = jsonBuffer.parseObject(line);
	
	if (!root.success()) {
		Console.println(F("Reading config: parseObject() failed."));
		return 0;
	}
	
	// Set configuration parameters
	posReturn = Heating->Rooms.setOffsetTime(root);
	posReturn += Heating->Rooms.setOffsetTemp(root);
	posReturn += Heating->Rooms.setRooms(root);
	posReturn += Heating->Burner.setSP(root);
	posReturn += Heating->WarmWater.setSP(root);
	
        return posReturn;
}

bool readConf(cHeating* Heating) {
    char line[LineSize];
    int posReturn = 0 ;
    
    if(FileSystem.exists(ConfigFileName)){
        File ini = FileSystem.open(ConfigFileName, FILE_READ);
        if (ini){
            // read in standard parameters
            int i=0;
            while(ini.available()){
                
                line[i]=ini.read();
                i++;
                // Detect full lines and avoid overflow
                if ((line[i-1]=='\r')||(i==LineSize)){
                    line[i] = '\0';
                    Console.println(line); // Debug Output
                    posReturn = readConfigLine(line, Heating);
                    i=0;
                }
            }
            
            ini.close();
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        Console.println(F("No System.cfg file found.\n Writing default config to SDCard."));
        return writeConf(Heating);
    }
}

//////////////Logfile//////////////////
void logPrintHeader(File* file, JsonObject& root ) {
	for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
		if (it->value.is<const JsonArray &>()) {
			int i = 0;
			JsonArray& data = root[it->key];
			
			for (JsonArray::iterator iter = data.begin(); iter!=data.end(); ++iter) {
				file->print(",");
				file->print(it->key);
				file->print(i++);
			}
		}
		else {
			file->print(",");
			file->print(it->key);
		}
	}
}

void logPrintData(File* file, JsonObject& root ) {
	for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
            // Update rest api
            String value;
            root[it->key].printTo(value);
            Bridge.put(String(it->key), value);
            
            // Write to Logfile
            if (it->value.is<const char *>()) {
                    file->print(",");
                    file->print(it->value.asString());
            }
            else if (it->value.is<double>()) {
                    file->print(",");
                    file->print((it->value.as<double>()));
            }
            else if (it->value.is<long>()) {
                    file->print(",");
                    file->print((it->value.as<long>()));
            }
            else if (it->value.is<JsonArray&>()) {
                    JsonArray& data = root[it->key];
                    int i = 0;
                    for (JsonArray::iterator iter = data.begin(); iter!=data.end(); ++iter) {
                            file->print(",");
                            if (root[it->key][i].is<double>()){
                                    file->print(root[it->key][i++].as<double>());
                            }
                            else if (root[it->key][i].is<long>()){
                                    file->print(root[it->key][i++].as<long>());
                            }
                            else if (root[it->key][i].is<const char *>()){
                                    file->print(root[it->key][i++].asString());
                            }
                    }
            }
            else file->print(F(",unknown data type"));
	}
}

template <typename T>
void logWriteelement(File* file, T* Obj, boolean bHeaders) {
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	
	Obj->getData(root);
	if (bHeaders) logPrintHeader(file, root);
	else logPrintData(file, root);
}

// Log a data record.
void logWrite(boolean bHeaders, cHeating* Heating) {
    char path[40] = LogFilePath;
    strcat(path, LogFileName);
    File file = FileSystem.open(path, FILE_APPEND);
    
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
    
    // Write data to file.  Start with log time in micros.
    logWriteelement(&file, &(Heating->Rooms),bHeaders);
    logWriteelement(&file, Heating,bHeaders);
    logWriteelement(&file, &(Heating->Burner),bHeaders);
    logWriteelement(&file, &(Heating->Boiler),bHeaders);
    logWriteelement(&file, &(Heating->WarmWater),bHeaders);
    logWriteelement(&file, &(Heating->Solar),bHeaders);
    
    file.println();
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
        char path[40] = LogFilePath;
        strcat(path, fileName);
	while (FileSystem.exists(path)) {
		if (fileName[BASE_NAME_SIZE + 1] != '9') {
			fileName[BASE_NAME_SIZE + 1]++;
			} else if (fileName[BASE_NAME_SIZE] != '9') {
			fileName[BASE_NAME_SIZE + 1] = '0';
			fileName[BASE_NAME_SIZE]++;
			} else {
			Console.print("Can't create file name.");
		}
		path[15] = '\0';
		strcat(path, fileName);
	}
	return fileName;
}

void startLogging(cHeating* Heating) {
    Console.print(F("Logging to: "));
    Console.println(genFile(LogFileName));
    
    logWrite(true, Heating);
    Console.println(F("Type any character to stop logging."));
    logging = true;
}

void stopLogging() {
    Console.println(F("Stopped Logging."));
    logging = false;
}

#endif