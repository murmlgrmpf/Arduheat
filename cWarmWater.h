// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#include "Arduino.h"


#include "cTemp.h"
#include "cPump.h"
#include "PinDefinitions.h"
#include <cPID.h>
#include <RTClib.h>
#include <ArduinoJson.h>

#define DefaultSpTempWarmWater 45.0
#define DefaultSpTempWarmWaterLower -2.0

extern DateTime TimeNow;

class cWarmWater
{
	public:
	cWarmWater(void):
	Pump(PinPumpWarmWater),
	pid(0.5, 0.01, 0.1, DIRECT),
	IsTemp(&MPNumSys[0], &MPChanSys[idxTempWarmWater], &SysTempOffset[idxTempWarmWater]),
	IsTempWarmWaterToBoiler(&MPNumSys[0], &MPChanSys[idxTempWarmWaterToBoiler], &SysTempOffset[idxTempWarmWaterToBoiler])
	{
		pinMode(PinWarmWaterSwitch, INPUT_PULLUP);
		// Initialize WarmWater Schedule
		TempSchedule[0].time.set(0, 6,0,0);
		TempSchedule[0].temp = DefaultSpTempWarmWater;
		TempSchedule[1].time.set(0,20,0,0);
		TempSchedule[1].temp = DefaultSpTempWarmWater + DefaultSpTempWarmWaterLower;
		// Initialize PID controllers for pump
		pid.SetOutputLimits(0.0, 1.0);
	}
	
	void Control(void);
	
	double SpTemp(void)
	{
		double SpTemp = TempSchedule[1].temp;
		TimeSpan rel;
		rel.set(0, TimeNow.hour(), TimeNow.minute(), 0);
		
		for(int iSwitch=0; iSwitch<2; iSwitch++) {
			if (TempSchedule[iSwitch].time.totalseconds() < rel.totalseconds())
				SpTemp = TempSchedule[iSwitch].temp;
		}
		return SpTemp;
	}
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	
	sTempSchedule TempSchedule[2];

	//private:
	cPump Pump;
	cTempSensor IsTemp;
	cTempSensor IsTempWarmWaterToBoiler;
	cPID pid;
};


#endif

