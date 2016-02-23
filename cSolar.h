#ifndef cHeatSource_h
#define cHeatSource_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include "cPump.h"
#include "cLFPWM.h"
#include "cTrigger.h"
#include <cPID.h>
#include <ArduinoJson.h>

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
// Solar Probing period
#define ProbePeriod 1800000 //30min*60sec/min*1000ms/sec = 2700000ms

class cSolarIntensity
{
	public:
	cSolarIntensity(void):
	Trigger(TimePeriod)
	{
		pinMode(PinSolarIntensity, INPUT);
		float intensity = 0;
	}

	float get( void )
	{
		if (Trigger.get()) {
			
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
        cTrigger Trigger;
};


class cSolar
{
	public:
	
	cSolar()
	:Valve(PinValveSolarOpen,PinValveSolarClose),
	Pump(PinPumpSolar,0.01, 0.0002, 0.0, DIRECT),
	TempToCollector(&MPNumSys[0], &MPChanSys[idxTempSolarToCollector], &SysTempOffset[idxTempSolarToCollector]),
	TempFromCollector(&MPNumSys[0], &MPChanSys[idxTempSolarFromCollector], &SysTempOffset[idxTempSolarFromCollector]),
	TempToSystem(&MPNumSys[0], &MPChanSys[idxTempSolarToSystem], &SysTempOffset[idxTempSolarToSystem]),
	TempFromSystem(&MPNumSys[0], &MPChanSys[idxTempSolarFromSystem], &SysTempOffset[idxTempSolarFromSystem]),
	Probe(ProbePeriod)
	{
		boolean sufficientHeat = false;
		Pump.SetOutputLimits(0.1, 1.0);
        }
	
	boolean harvest(double SpTempSource, boolean enable = true)
	{
		// Harvesting hysteresis
		if (TempFromCollector.get() < SpTempSource-2) // Exploit ChargeMargin of Boiler = 4
                    sufficientHeat = false;
		//if (TempFromCollector.get() > SpTempSource+4)
		if (TempFromCollector.get() > SpTempSource+2)
                    sufficientHeat = true;
		
		if (sufficientHeat)// Run pump such that temperature difference between Return and Lead is equal to 10 degree.
		{
			float TempDiff = TempFromCollector.get()-SpTempSource;
			Pump.run(TempFromSystem.get()+TempDiff+3,TempToCollector.get());
		}
		else if (Probe.get(0.1) && (SolarIntensity.get()>335))
		{
			Pump.run(0.25);
		}
		else Pump.run();
		
		// Open valve if sufficient heat is available
		Valve.set(sufficientHeat && enable);
		
		// Return if there is residual heat so that charging of boiler continues
		return sufficientHeat;
	}
	
	float TempSource()
        {
            return(max(TempToSystem.get(),TempFromSystem.get()));
        }
	
	void getData(JsonObject& root);
	
	boolean sufficientHeat;
	
	cTempSensor TempFromCollector;
	cTempSensor TempToSystem;
	
	private:
	
	cTempSensor TempToCollector;
	cTempSensor TempFromSystem;
	
	cSolarIntensity SolarIntensity;
        
        cLFPWM Probe;
	
	cValve Valve;
	cPump Pump;
};

#endif
