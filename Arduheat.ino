#include <Arduino.h>
#include <SPI.h>
#include <PID_v1.h>
// RTC
#include <Wire.h>
#include <RTClib.h>

#include "cHeating.h"
#include "config.h"
#include "cTrigger.h"

//// RTC //////////
RTC_DS1307 rtc;
DateTime TimeNow;
///////////////////

cTrigger trigger(10000);
cHeating Heating;
cConfig Config(&Serial,&Heating);

/**
* @fn void setup(void)
* @brief Initializes the serial connection and defines the in- and outputs.
*/
void setup() {
	// Initialize the Bridge, Console and FileSystem
	Serial.begin(57600);  // Set the baud.
	while (!Serial) {
	}  // Wait until Serial1 is ready

	// RTC
	Wire.begin();
	rtc.begin();
	////////////////////////
	//// following line sets the RTC to the date & time this sketch was compiled
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	TimeNow = rtc.now();

	// analogReference(DEFAULT);
	analogReference(EXTERNAL);
	analogRead(0);

	Config.resetConf();
}

/**
* @fn void loop(void)
* @brief Runs the main program which controls the system.
*/
void loop() {
	Heating.Control();
	Config.updateConf();

	if (trigger.get()) {
		Config.sendLog();
		TimeNow = rtc.now();
	}
}
