// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "cFlowMeter.h"
#include "cTemp.h"
#include "cPump.h"
#include "PinDefinitions.h"
#include <cPID.h>
#include <RTClib.h>
#include <ArduinoJson.h>

#define SpTempWarmWater 47.0
#define SpTempWarmWaterLower -3.0

extern DateTime TimeNow;

typedef struct{
	TimeSpan time;
	double temp;
} sTempScheduleWW;

class cWarmWater
{
	public:
	cWarmWater();
	//cFlowMeter FlowMeter;
	void Control();
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	double getSpTemp(void);
	
	void getData(JsonObject& root);
	
	sTempScheduleWW TempSchedule[2];

	private:
	cPump Pump;
	cTempSensor IsTemp;
	cPID pid;

};


#endif

