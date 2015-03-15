#include <Arduino.h>
// #include <avr/pgmspace.h>
#include "cWarmWater.h"// important for global TimeNow variable
#include "cHeating.h"
#include <cPID.h>

/*
#include "config.h"
#include <SdFat.h>
#include <SdFatUtil.h>
*/
#include <ArduinoJson.h>
#include <avr/pgmspace.h>

/*
#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>

#include "fileServer.h"
*/
// Speicherbedarf: 
// 2168 ohne logging und webserver
// 4352 mit  logging und webserver

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
//static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
//static uint8_t ip[] = { 192, 168, 123, 250 };


/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
//#define PREFIX ""
//WebServer webserver(PREFIX, 80);

#include <Mailbox.h>


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
	pinMode(53, OUTPUT);                       // set the SS pin as an output (necessary!)
	digitalWrite(53, HIGH);                    // but turn off the W5100 chip!
	
	Bridge.begin();
	Mailbox.begin();
	
	//// RTC test //////////
	Serial.begin(115200);
	
	//PgmPrint("Free RAM: "); // part of sdFatUtil
	//Serial.println(FreeRam());
	
	// RTC
	Wire.begin();
	rtc.begin();
	////////////////////////
	//// following line sets the RTC to the date & time this sketch was compiled
	//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	TimeNow = rtc.now();
	
	analogReference(DEFAULT);
	//analogReference(EXTERNAL);
	
	/*
	initSD();
	writeConf(&Heating); // write defaults
	readConf( &Heating);
	writeConf(&Heating); // write defaults
	
	startLogging(&Heating);
	
	/* initialize the Ethernet adapter */
	//Ethernet.begin(mac, ip);
	
	//initFileServer();

	/* setup our default command that will be run when the user accesses
	* the root page on the server */
	//webserver.setDefaultCommand(&helloCmd);

	/* run the same command if you try to load /index.html, a common
	* default page name */
	//webserver.addCommand("index.html", &helloCmd);
	
	//webserver.addCommand("rest.html", &restCmd);
	
	// Fileserver
	//webserver.setUrlPathCommand(&fsAccessCmd);

	/* start the webserver */
	//webserver.begin();
}

/**
* @fn void loop(void)
* @brief Runs the main program which controls the system.
*/
void loop()
{
	Heating.Control();
	Heating.WarmWater.Control();
	
	/*
	if (trigger.get()) {
		// Generate new logfile every day at midnight
		if (TimeNow.hour()-rtc.now().hour()==23){
		  stopLogging();
		  TimeNow = rtc.now();
		  startLogging(&Heating);
		}
		TimeNow = rtc.now();
		
// 		if(fileerror||sderror) {
// 			initSD();
// 			startLogging(&Heating);
// 		}
		//PgmPrint("Free RAM: "); Serial.println(FreeRam());// part of sdFatUtil
		if(logging) logWrite(false, &Heating);
	}
	
	if (Serial.available()&&(logging)) stopLogging();
	
	char buff[64];
	int len = 64;
	*/
	/* process incoming connections one at a time forever */
	//webserver.processConnection(buff, &len);
}