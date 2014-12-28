#include <Arduino.h>
#include <avr/pgmspace.h>
#include "cWarmWater.h"// important for global TimeNow variable
#include "cHeating.h"
#include <cPID.h>
#include "config.h"
#include <SdFat.h>
#include <SdFatUtil.h>
#include <ArduinoJson.h>


#include "cTrigger.h"

// RTC
#include <Wire.h>
#include <RTClib.h>
#include "cRoom.h" // important for global TimeNow variable


//// RTC //////////
RTC_DS1307 rtc;
DateTime TimeNow;
////////////////////////

cTrigger triggerLog(10000);
cHeating Heating;

/**
* @fn void setup(void)
* @brief Initializes the serial connection and defines the in- and outputs.
*/
void  setup()
{
	pinMode(10, OUTPUT);                       // set the SS pin as an output (necessary!)
	digitalWrite(10, HIGH);                    // but turn off the W5100 chip!
	pinMode(53, OUTPUT);
	
	//// RTC test //////////
	Serial.begin(115200);
	
	PgmPrint("Free RAM: "); // part of sdFatUtil
	Serial.println(FreeRam());
	
	// RTC
	Wire.begin();
	rtc.begin();
	////////////////////////
	//// following line sets the RTC to the date & time this sketch was compiled
	//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	
	PinInitialization();
	analogReference(DEFAULT);
	//analogReference(EXTERNAL);
	
	initSD();
	if (!sderror)
	{
		writeConf( &Heating ); // write defaults
		if (!readConfig(&Heating)) {
			writeConf(&Heating); // If reading config fails, write defaults
			Serial.println(F("sd config file did not exist, writing defaults."));
		}
		writeConf( &Heating); // write defaults
	}

	
	//Serial.println(F("Type any character to start logging"));
	if ((!sderror)&&(!fileerror))
	{
		Serial.print(F("Logging to: "));
		Serial.println(genFile());
		logWrite(true, &Heating);
		Serial.println(F("Type any character to stop logging"));
		logging = true;
	}

}

/**
* @fn void loop(void)
* @brief Runs the main program which controls the system.
*/
void loop()
{
	TimeNow = rtc.now();
	Heating.Control();
	Heating.WarmWater.Control();
	
	
	if (triggerLog.get()&&logging&&(!sderror)&&(!fileerror)) {
		logWrite(false, &Heating);
		// TODO: Check for data rate too high.
	}
	
	if (Serial.available()&&(logging)&&(!sderror)&&(!fileerror)) {
		stopLogging();
	}
}