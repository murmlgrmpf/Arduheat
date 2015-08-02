// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#include "Arduino.h"


#include "cTemp.h"
#include "cPump.h"
#include "PinDefinitions.h"
#include <cPID.h>
#include <ArduinoJson.h>

class cWarmWater
{
	public:
	cWarmWater(void):
	Pump(PinPumpWarmWater,0.12, 0.005, 2.8, DIRECT),
	Trigger(15*60*1000),
	IsTemp(&MPNumSys[0], &MPChanSys[idxTempWarmWater], &SysTempOffset[idxTempWarmWater]),
	IsTempBoilerTop(&MPNumSys[0], &MPChanSys[idxTempBoilerTop], &SysTempOffset[idxTempBoilerTop]),
	IsTempWarmWaterToBoiler(&MPNumSys[0], &MPChanSys[idxTempWarmWaterToBoiler], &SysTempOffset[idxTempWarmWaterToBoiler])
	{
		pinMode(PinWarmWaterSwitch, INPUT_PULLUP);
		// Initialize PID controllers for pump
		Pump.SetOutputLimits(0.0, 1.0);
		Pump.SetSampleTime(500);
		setParam();
	}
	
	void Control(void);
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	void getData(JsonObject& root);
	
	double SpTemp;
	
	private:
	void setParam();
	
	cPump Pump;
	cTrigger Trigger;
	cTempSensor IsTemp;
	cTempSensor IsTempBoilerTop;
	cTempSensor IsTempWarmWaterToBoiler;
};


#endif

