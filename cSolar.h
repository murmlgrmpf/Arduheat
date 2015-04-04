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

// Intensity smoothing
#define AlphaT 10 //! Filter sampling interval
#define TimePeriod 100
//#define R    4990.0  //! OP Resistor Value [Ohm]
#define Ri1 11000.0 //! OP Resistor Value [Ohm]
#define Ri2  3740.0 //! OP Resistor Value [Ohm]
#define Ri3 10000.0 //! OP Resistor Value [Ohm]
#define Uf0 1.211
#define Wmax 1300
#define Ufmax 2.233

extern DateTime TimeNow;

class cSolarIntensity
{
	public:
	cSolarIntensity(void)
	{
		pinMode(PinSolarIntensity, INPUT);
		float intensity = 0;
	}

	float get( void )
	{
		if (millis()-StartTime > TimePeriod) {
			StartTime = millis();
			
			float Ua = analogRead(PinSolarIntensity)/1023.0*Vcc;
			float Uf = (Ua + Vcc*(Ri1/Ri3))/(1+Ri1/Ri2+Ri1/Ri3);
			float temp = Wmax*(Uf-Uf0)/(Ufmax-Uf0);
			
			float alphaFilt = AlphaT*TimePeriod/1000;
			intensity = (alphaFilt/(alphaFilt+1))*temp  + 1/(alphaFilt+1)*intensity;
		}
		return(intensity);
	}
	private:
	float intensity;
	unsigned long StartTime;
};


class cSolar
{
	public:
	
	cSolar()
	:Valve(PinValveSolarOpen,PinValveSolarClose),
	Pump(PinPumpSolar,0.1, 0, 19.5, DIRECT), // was 0.1, 0.05, 0.025
	TempToCollector(&MPNumSys[0], &MPChanSys[idxTempSolarToCollector], &SysTempOffset[idxTempSolarToCollector]),
	TempFromCollector(&MPNumSys[0], &MPChanSys[idxTempSolarFromCollector], &SysTempOffset[idxTempSolarFromCollector]),
	TempToSystem(&MPNumSys[0], &MPChanSys[idxTempSolarToSystem], &SysTempOffset[idxTempSolarToSystem]),
	TempFromSystem(&MPNumSys[0], &MPChanSys[idxTempSolarFromSystem], &SysTempOffset[idxTempSolarFromSystem])
	{
		boolean sufficientHeat =false;
		boolean probing = false;
		StartTime = millis();
		
		Pump.SetOutputLimits(0.2, 1.0); // was 0.2, Test1 0.4, Test2 0.35
	}
	
	boolean harvest(double SpTempSource, boolean enable = true)
	{
		// Trigger Probing
		if (((millis()-StartTime>6*ProbeInterval)&&(TimeNow.hour()>9)&&(TimeNow.hour()<17)&&(!sufficientHeat)&&(!probing)) || (SolarIntensity.get()>500))
		{
			probing=true;
			StartTime = millis();
		}
		
		// Harvesting hysteresis
		if (TempFromCollector.get() < SpTempSource-2) // Exploit ChargeMargin of Boiler = 4
		sufficientHeat = false;
		//if (TempFromCollector.get() > SpTempSource+4)
		if (TempFromCollector.get() > SpTempSource)
		sufficientHeat = true;
		
		if (sufficientHeat)// Run pump such that temperature difference between Return and Lead is equal to 10 degree.
		{
			float TempDiff = TempFromCollector.get()-SpTempSource;
			Pump.run(TempFromSystem.get()+TempDiff,TempToCollector.get());
		}
		else if (probing)
		{
			Pump.run(0.1);
			;
			// Check if probing interval is over
			if (millis()-StartTime>ProbeInterval) {
				probing = false;
				StartTime = millis();
			}
		}
		else Pump.run();
		
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
	
	cSolarIntensity SolarIntensity;
	
	cValve Valve;
	cPump Pump;
};

#endif
