#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include <TimeLib.h>

#define BufferSize 1000
#define TimeOut 1000
#define numChars 500

class cConfig 
{
public:
    cConfig(HardwareSerial* serial_, cHeating* Heating_) {
        newData = false;
        serial = serial_;
        Heating = Heating_;
    }
    
    void resetConf() {
        // Delete config on yun to trigger resynchronization with config file
        serial->println("{\"Reset\":[]}");
    }
    
    void sendConf() {
        // Write config to sd card via linux
        serial->print("{\"Conf\":[");
        printElement(&(Heating->Rooms), &cRooms::getOffsetTime); serial->print(",");
        printElement(&(Heating->Rooms), &cRooms::getOffsetTemp); serial->print(",");
        printElement(&(Heating->Rooms), &cRooms::getRooms); serial->print(",");
        printElement(&(Heating->WarmWater),&cWarmWater::getSP);
        serial->println("]}");
    }

    void updateConf() {
        readSerial();
        if (newData) {
            // Debug
            serial->print("Message: ");
            serial->println(receivedChars);

            // Update Time of Timelib
            processTimeSyncMessage();

            //int posReturn = 0;
            //posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setOffsetTime);
            //posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setOffsetTemp);
            //posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setRooms);
            //posReturn += readConfigLine(&(Heating->WarmWater), &cWarmWater::setSP);
            
            sendConf();
            newData = false;
        }
    }

    //////////////Logfile//////////////////
    void sendLog() {
        // Write data to file.  Start with log time in micros.
		serial->print("{\"Log\":[");
        printElement(&(Heating->Rooms), &cRooms::getData); serial->print(",");
        printElement(Heating, &cHeating::getData); serial->print(",");
		printElement(&(Heating->TransferStation), &cTransferStation::getData); serial->print(",");
		printElement(&(Heating->Boiler), &cBoiler::getData); serial->print(",");
		printElement(&(Heating->WarmWater), &cWarmWater::getData); serial->print(",");
		printElement(&(Heating->FlowHeater), &cFlowHeater::getData); serial->print(",");
		printElement(&(Heating->Pool), &cPool::getData); serial->print(",");
        printTimeLibTime();
		serial->println("]}");
    }

private:
    template <typename T>
    void printElement(T* Obj, void(T::*getF)(ArduinoJson::JsonObject&)) {
            StaticJsonBuffer<BufferSize> jsonBuffer;
            JsonObject& root = jsonBuffer.createObject();
            
            (Obj->*getF)(root);
            root.printTo(*serial);
    }

    void printTimeLibTime() {
        StaticJsonBuffer<BufferSize> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        time_t dt = now();

        char buffer[25]; // large enough for any DateTime, including invalid ones
        sprintf(buffer, "%u-%02d-%02dT%02d:%02d:%02d", year(dt), month(dt), day(dt), hour(dt), minute(dt), second(dt));

        root["TimeArduino"] = buffer;
        root["TimeLibTimeStatus"] = timeStatus();
        root["TimeLibTime"] = dt;
        
        root.printTo(*serial);
    }
    
    void readSerial() {
        static uint32_t ndx = 0;
        char endMarker = '\n';
        char rc;
        while (serial->available() > 0 && newData == false) {
            rc = serial->read();

            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                        ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                ndx = 0;
                newData = true;
            }
        }
    }

    template <typename T>
    int readConfigLine(T* Obj, int(T::*setF)(ArduinoJson::JsonObject&)) {
        StaticJsonBuffer<BufferSize> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(receivedChars);
        
        // Set configuration parameters
        int posReturn = (Obj->*setF)(root);

        return posReturn;
    }

    void processTimeSyncMessage() {
        unsigned long raspberryTime;
        const unsigned long DEFAULT_TIME = 1357041600; // jan 1 2013

        StaticJsonBuffer<BufferSize> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(receivedChars);

        if (root.containsKey("TimeRasp")) {
            if (root["TimeRasp"].is<unsigned long>()) {
                raspberryTime = root["TimeRasp"].as<unsigned long>();
                if (raspberryTime >= DEFAULT_TIME) {
                    setTime(raspberryTime);
                    serial->println("Successfully set Arduino time.");
                }
            }
        }
    }
    
    HardwareSerial* serial;
    cHeating* Heating;
    char receivedChars[numChars];  // an array to store the received data
    boolean newData;
};

#endif
