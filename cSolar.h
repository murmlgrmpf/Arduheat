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
#define ProbePeriod 90000 //1,5min*60sec/min*1000ms/sec
#define minWaitPeriod 240000//4min*60sec/min*1000ms/sec
#define maxWaitPeriod 1800000//30min*60sec/min*1000ms/sec

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
	
	cSolar():
	Valve(PinValveSolarOpen,PinValveSolarClose),
	Pump(PinPumpSolar,0.01, 0.0002, 0.0, DIRECT),
	TempFromCollector(&MPNumSys[0], &MPChanSys[idxTempSolarFromCollector], &SysTempOffset[idxTempSolarFromCollector]),
	TempToSystem(&MPNumSys[0], &MPChanSys[idxTempSolarToSystem], &SysTempOffset[idxTempSolarToSystem]),
	Probing(ProbePeriod),
	Waiting(minWaitPeriod)
	{
		boolean sufficientHeat = false;
		Pump.SetOutputLimits(0.15, 1.0);
		
		pinMode(PinValveSolar, OUTPUT);
		digitalWrite(PinValveSolar, LOW);
	}
	
	boolean hasHeat(double SpTempSource)
	{
		// introduce FlipFlop for Hysteresis of Solar Charge
		if ((sufficientHeat == false) && (SolarIntensity.get()>500) && (TempFromCollector.getRaw() > (SpTempSource + 2)))
		sufficientHeat = true;
		if ((sufficientHeat == true) && (TempFromCollector.getRaw() < max(30, (SpTempSource - 8))))
		sufficientHeat = false;

		return(sufficientHeat);
	}
	
	boolean harvest(double SpTempSource, boolean enable = true)
	{
		double tempdiff = (SpTempSource - TempFromCollector.getRaw());
		
		// Probing timer restart
		if ((!Waiting.get()) && (SolarIntensity.get()>500) && (TempFromCollector.getRaw() > 10))
		{
			Probing.restart();
		}
		
		if (sufficientHeat)
		{
			Pump.run(0.4);
			Waiting.restart();
		}
		else if (Probing.get() && (TempFromCollector.getRaw() > 10))
		{
			Pump.run(1.0);
			
			unsigned long WaitPeriod = static_cast<unsigned long>(min((minWaitPeriod + max((tempdiff-7),0)*1.4*60*1000),maxWaitPeriod));
			Waiting.setTimePeriod(WaitPeriod);
			Waiting.restart();
		}
		else Pump.run(); // Stop Pump
		
		// Open valve if sufficient heat is available
		Valve.set(sufficientHeat && enable);
		
		if (sufficientHeat && enable)
			digitalWrite(PinValveSolar, HIGH);
		
		// Return if there is residual heat so that charging of boiler continues
		return(sufficientHeat);
	}
	
	float TempSource()
	{
		return(TempToSystem.getRaw());	}
	
	void getData(JsonObject& root);
	
	boolean sufficientHeat;
	
	cTempSensor TempFromCollector;
	cTempSensor TempToSystem;
	
	private:
	cSolarIntensity SolarIntensity;

	cTimer Probing;
	cTimer Waiting;
	
	cValve Valve;
	cPump Pump;
};

#endif
