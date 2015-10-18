#include <Arduino.h>
#include "cWarmWater.h"// important for global TimeNow variable
#include "cHeating.h"
#include <cPID.h>

#include "config.h"

#include <ArduinoJson.h>
#include <avr/pgmspace.h>

// Speicherbedarf: 
// 2168 ohne logging und webserver
// 4352 mit  logging und webserver

// Logging
#include <FileIO.h>
#include <Console.h>
// Rest Interface
#include <Bridge.h>

#include "cTrigger.h"

// RTC
#include <Wire.h>
#include <RTClib.h>
#include "cRooms.h" // important for global TimeNow variable


//// RTC //////////
RTC_DS1307 rtc;
DateTime TimeNow;
////////////////////////

cTrigger trigger(10000);
cHeating Heating;


/**
* @fn void setup(void)
* @brief Initializes the serial connection and defines the in- and outputs.
*/
void  setup()
{
    // Initialize the Bridge, Console and FileSystem
  Bridge.begin();
  Console.begin();
  FileSystem.begin();
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)

  //while(!Console);  // wait for Serial port to connect.
  //Console.println("Filesystem datalogger\n");
	
	// RTC
	Wire.begin();
	rtc.begin();
	////////////////////////
	//// following line sets the RTC to the date & time this sketch was compiled
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	TimeNow = rtc.now();
	
	analogReference(DEFAULT);
	//analogReference(EXTERNAL);
	
	// Config
	readConf(&Heating);
	
        startLogging(&Heating);
}

/**
* @fn void loop(void)
* @brief Runs the main program which controls the system.
*/
void loop()
{
	Heating.Control();
	Heating.WarmWater.Control();
	
	if (trigger.get()) {
		// Generate new logfile every day at midnight
		if (TimeNow.hour()-rtc.now().hour()==23){
                stopLogging();
		  TimeNow = rtc.now();
		  startLogging(&Heating);
		}
		TimeNow = rtc.now();
		
                if(logging) logWrite(false, &Heating);
                writeConf(&Heating);
	}
	
 	if (Console.available()&&(logging)) stopLogging();
}
