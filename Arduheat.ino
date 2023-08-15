#include <Arduino.h>
#include <PID_v1.h>
#include <TimeLib.h>

#include "cHeating.h"
#include "config.h"
#include "cTrigger.h"

cTrigger trigger(10000);
cHeating Heating;
cConfig Config(&Serial,&Heating);

time_t requestTimeSync() {
	Serial.println("{\"Time\":[]}");
	return 0;
}

/**
* @fn void setup(void)
* @brief Initializes the serial connection and defines the in- and outputs.
*/
void setup() {
	// Initialize the Bridge, Console and FileSystem
	Serial.begin(57600);  // Set the baud.
	while (!Serial) {
	}  // Wait until Serial1 is ready

	Serial.println("Arduino Startup.");

	// TimeLib time
	setSyncProvider(requestTimeSync);

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
	}
}
