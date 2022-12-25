#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>

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
            int posReturn = 0;
            posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setOffsetTime);
            posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setOffsetTemp);
            posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setRooms);
            posReturn += readConfigLine(&(Heating->WarmWater), &cWarmWater::setSP);
            sendConf();
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
		printElement(&(Heating->Solar), &cSolar::getData); serial->print(",");
		printElement(&(Heating->Pool), &cPool::getData);
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
        
        newData = false;
        return posReturn;
    }
    
    HardwareSerial* serial;
    cHeating* Heating;
    char receivedChars[numChars];  // an array to store the received data
    boolean newData;
};

#endif
