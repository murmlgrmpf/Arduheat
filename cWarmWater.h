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

#define DefaultSpTempWarmWater 47.0
#define DefaultSpTempWarmWaterLower -3.0

extern DateTime TimeNow;

class cWarmWater
{
	public:
	cWarmWater(void):
	Pump(PinPumpWarmWater),
	pid(0.5, 0.01, 0.1, DIRECT),
	IsTemp(SystemMultiplexer,MultiplexTempWarmWater,OffsetTempWarmWater)
	{
		pinMode(PinWarmWaterSwitch, OUTPUT);
		// Initialize WarmWater Schedule
		TempSchedule[0].time.set(0, 6,0,0);
		TempSchedule[0].temp = DefaultSpTempWarmWater;
		TempSchedule[1].time.set(0,20,0,0);
		TempSchedule[1].temp = DefaultSpTempWarmWater + DefaultSpTempWarmWaterLower;
		// Initialize PID controllers for pump
		pid.SetOutputLimits(0.0, 1.0);
	}
	
	void Control(void){
		// If Warm Water is needed, execute the pump 
		// with the power determined by pid controller.
		// Else switch pid to manual and stop pump.
		if(digitalRead(PinWarmWaterSwitch))
			Pump.setPower(pid.run(SpTemp(), IsTemp.get()));
		else
			Pump.setPower(pid.run());
	}
	
	double SpTemp(void)
	{
		double SpTemp = 0;
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

	private:
	cPump Pump;
	cTempSensor IsTemp;
	cPID pid;
};


#endif

