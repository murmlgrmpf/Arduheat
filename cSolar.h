#ifndef cHeatSource_h
#define cHeatSource_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include "cPump.h"
#include <cPID.h>
#include <ArduinoJson.h>
// 5min*60sec/min*1000ms/s = 300000ms
#define ProbeInterval 600000

extern DateTime TimeNow;

class cSolar
{
	
	public:
	
	cSolar()
	:Valve(PinValveSolarOpen,PinValveSolarClose),
	pid( 0.1, 0.05, 0.025, DIRECT),
	Pump(PinPumpSolar),
	TempToCollector(&MPNumSys[0], &MPChanSys[idxTempSolarToCollector], &SysTempOffset[idxTempSolarToCollector]),
	TempFromCollector(&MPNumSys[0], &MPChanSys[idxTempSolarFromCollector], &SysTempOffset[idxTempSolarFromCollector]),
	TempToSystem(&MPNumSys[0], &MPChanSys[idxTempSolarToSystem], &SysTempOffset[idxTempSolarToSystem]),
	TempFromSystem(&MPNumSys[0], &MPChanSys[idxTempSolarFromSystem], &SysTempOffset[idxTempSolarFromSystem]),
	Intensity(&MPNumSys[0], &MPChanSys[idxSolarIntensity], &SysTempOffset[idxSolarIntensity])
	{
		boolean sufficientHeat =false;
		boolean probing = false;
		StartTime = millis();
		
		pid.SetOutputLimits(0.2, 1.0);
	}
	
	boolean harvest(double SpTempSource, boolean enable = true)
	{
		// Trigger Probing
		if ((millis()-StartTime>6*ProbeInterval)&&(TimeNow.hour()>9)&&(TimeNow.hour()<17)&&(!sufficientHeat)&&(!probing)) {
		      probing=true;
		      StartTime = millis();
		}
		
		// Harvesting hysteresis
		if (TempFromCollector.get() < SpTempSource-2) // Exploit ChargeMargin of Boiler = 4
			sufficientHeat = false;
		if (TempFromCollector.get() > SpTempSource+4)
			sufficientHeat = true;
		
		if (sufficientHeat)// Run pump such that temperature difference between Return and Lead is equal to 10 degree.
		{
			float TempDiff = TempFromCollector.get()-SpTempSource;
			Pump.setPower(pid.run(TempFromSystem.get()+TempDiff,TempToCollector.get()));
		}
		else if (probing)
		{
			Pump.setPower(pid.run(0.1));
			;
			// Check if probing interval is over
			if (millis()-StartTime>ProbeInterval) {
				probing = false;
				StartTime = millis();
			}
		}
		else Pump.setPower(pid.run());
		
		// Open valve if sufficient heat is available
		Valve.set(sufficientHeat && enable);
		
		// Return if there is residual heat so that charging of boiler continues
		return sufficientHeat;
	}
	
	void getData(JsonObject& root);
	
	void probe(boolean bFlame);
	
	boolean sufficientHeat;
	boolean probing;
	
	cTempSensor TempFromCollector;
	cTempSensor TempToSystem;
	
	private:
	
	unsigned long StartTime;
	
	cTempSensor TempToCollector;
	
	cTempSensor TempFromSystem;
	
	cTempSensor Intensity;
	cValve Valve;
	cPump Pump;
	cPID pid;
};

#endif
