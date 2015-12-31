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
    
    void readConf() {
        readConfElement(&(Heating->Rooms), &cRooms::getOffsetTime, &cRooms::setOffsetTime);
        readConfElement(&(Heating->Rooms), &cRooms::getOffsetTemp, &cRooms::setOffsetTemp);
        readConfElement(&(Heating->Rooms), &cRooms::getRooms, &cRooms::setRooms);
        readConfElement(&(Heating->Burner), &cBurner::getSP, &cBurner::setSP);
        readConfElement(&(Heating->WarmWater), &cWarmWater::getSP, &cWarmWater::setSP);
    }
    
    int writeConf() {
        // Write config to sd card via linux
        serial->print("{\"writeConf\":[");
        printElement(&(Heating->Rooms), &cRooms::getOffsetTime);
        printElement(&(Heating->Rooms), &cRooms::getOffsetTemp);
        printElement(&(Heating->Rooms), &cRooms::getRooms);
        printElement(&(Heating->Burner),&cBurner::getSP);
        printElement(&(Heating->WarmWater),&cWarmWater::getSP);
        serial->println("]}");
    }

    void updateConf() {
        readSerial();
        int posReturn = 0;
        if (newData) {
            posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setOffsetTime);
            posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setOffsetTemp);
            posReturn += readConfigLine(&(Heating->Rooms), &cRooms::setRooms);
            posReturn += readConfigLine(&(Heating->Burner), &cBurner::setSP);
            posReturn += readConfigLine(&(Heating->WarmWater), &cWarmWater::setSP);
        }
        if (posReturn>0) {
            // Todo: send signal to linux
        }
    }

    //////////////Logfile//////////////////
    void writeLog() {
        // Write data to file.  Start with log time in micros.
        serial->print("{\"writeLog\":[");
        printElement(&(Heating->Rooms), &cRooms::getData);
        printElement(Heating, &cHeating::getData);
        printElement(&(Heating->Burner), &cBurner::getData);
        printElement(&(Heating->Boiler), &cBoiler::getData);
        printElement(&(Heating->WarmWater), &cWarmWater::getData);
        printElement(&(Heating->Solar), &cSolar::getData);
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
        static byte ndx = 0;
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
    void readConfElement(T* Obj, void(T::*getF)(ArduinoJson::JsonObject&), int(T::*setF)(ArduinoJson::JsonObject&)) {
        // Sending the getConf triggers the linux side to send the object
        serial->print("{\"readConf\":[");
        printElement(Obj,getF);
        serial->println("]}");
        // Wait for the object to arrive from linux
        unsigned long LastTime = millis();
        while(!newData&&(millis()-LastTime<TimeOut))
            readSerial();
        // Read in config
        readConfigLine(Obj, setF);
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
