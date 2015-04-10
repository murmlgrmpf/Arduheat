// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#include "Arduino.h"


#include "cTemp.h"
#include "cPump.h"
#include "PinDefinitions.h"
#include <cPID.h>
#include <ArduinoJson.h>

#define DefaultSpTempWarmWater 45.0

class cWarmWater
{
	public:
	cWarmWater(void):
	Pump(PinPumpWarmWater,0.5, 0.0, 7.0, DIRECT),
	IsTemp(&MPNumSys[0], &MPChanSys[idxTempWarmWater], &SysTempOffset[idxTempWarmWater]),
	IsTempWarmWaterToBoiler(&MPNumSys[0], &MPChanSys[idxTempWarmWaterToBoiler], &SysTempOffset[idxTempWarmWaterToBoiler])
	{
		pinMode(PinWarmWaterSwitch, INPUT_PULLUP);
		SpTemp = DefaultSpTempWarmWater;
		// Initialize PID controllers for pump
		Pump.SetOutputLimits(0.0, 1.0);
		Pump.SetSampleTime(500);
	}
	
	void Control(void);
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	double SpTemp;
	
	private:
	
	cPump Pump;
	cTempSensor IsTemp;
	cTempSensor IsTempWarmWaterToBoiler;
};


#endif

