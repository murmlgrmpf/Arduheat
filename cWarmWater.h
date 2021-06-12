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
	Pump(PinPumpWarmWater,0.0, 0.0, 0.0, DIRECT),
	Period(86400000), // 1(d)*24(d/h)*60(min/h)*60(s/min)*1000(mw/s)
	IsTemp(&MPNumSys[0], &MPChanSys[idxTempWarmWater], &SysTempOffset[idxTempWarmWater]),
	IsTempBoilerTop(&MPNumSys[0], &MPChanSys[idxTempBoilerTop], &SysTempOffset[idxTempBoilerTop]),
	IsTempWarmWaterFromBoiler(&MPNumSys[0], &MPChanSys[idxTempWarmWaterFromBoiler], &SysTempOffset[idxTempWarmWaterFromBoiler]),
	IsTempWaterFromOutside(&MPNumSys[0], &MPChanSys[idxTempWaterFromOutside], &SysTempOffset[idxTempWaterFromOutside])
	{
		PowerStatic = 0;	
		pinMode(PinWarmWaterFlow, INPUT);
		WarmWaterFlow = 0;
		// Initialize PID controllers for pump
				Pump.SetOutputLimits(-1.0, 1.0);
                SpTemp = 46;
	}
	
	void Control(void);
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	void getData(JsonObject& root);
	
	double SpTemp;
	cTimer Period;
	
	private:
	float WarmWaterFlow;
	double PowerStatic;
	cPump Pump;
	cTempSensor IsTemp;
	cTempSensor IsTempBoilerTop;
	cTempSensor IsTempWarmWaterFromBoiler;
	cTempSensor IsTempWaterFromOutside;
};


#endif

