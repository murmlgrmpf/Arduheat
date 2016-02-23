// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#include "Arduino.h"


#include "cTemp.h"
#include "cPump.h"
#include "PinDefinitions.h"
#include <cPID.h>
#include <ArduinoJson.h>

#define PumpPeriod 500

class cWarmWater
{
	public:
	cWarmWater(void):
	Pump(PinPumpWarmWater,0.12, 0.0002, 2.8, DIRECT, 0.0, PumpPeriod),
	Trigger(900000), // 15(min)*60(s/min)*1000(mw/s)
  Period(86400000), // 1(d)*24(d/h)*60(min/h)*60(s/min)*1000(mw/s)
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
	cTimer Period;
  
	private:
	void setParam();
	
	cPump Pump;
	cTrigger Trigger;
	cTempSensor IsTemp;
	cTempSensor IsTempBoilerTop;
	cTempSensor IsTempWarmWaterToBoiler;
};


#endif

