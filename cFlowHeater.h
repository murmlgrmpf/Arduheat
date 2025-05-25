#ifndef cFlowHeater_h
#define cFlowHeater_h

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
#define AlphaT_Solar 10 //! Filter sampling interval
#define TimePeriod 100
//#define R    4990.0  //! OP Resistor Value [Ohm]
#define Ri1 11000.0 //! OP Resistor Value [Ohm]
#define Ri2  3740.0 //! OP Resistor Value [Ohm]
#define Ri3 10000.0 //! OP Resistor Value [Ohm]
#define Uf0 1.211
#define Wmax 1300
#define Ufmax 2.233

class cFlowHeater
{
	public:
	
	cFlowHeater():
	TempToSystem(&MPNumSys[0], &MPChanSys[idxTempFlowHeaterToSystem], &SysTempOffset[idxTempFlowHeaterToSystem]),
	Valve(PinValveFlowHeater)
	{
		pinMode(PinFlowHeaterSwitch, INPUT_PULLUP);
		sufficientHeat = false;
	}	
	
	boolean hasHeat(double SpTempSource)
	{
		double setpointTemperatureRequiredBySink = max(30, (SpTempSource - 4));
		// introduce FlipFlop for Hysteresis of FlowHeater Charge
		if (!digitalRead(PinFlowHeaterSwitch))
			sufficientHeat = true;
		if ((sufficientHeat == true) && (TempToSystem.getRaw() < setpointTemperatureRequiredBySink))
			//Hystersis for residual heat
			sufficientHeat = false;

		return(sufficientHeat);
	}
	
	boolean harvest(double SpTempSource, boolean enable = true)
	{
		// Open valve if sufficient heat is available
		Valve.set(sufficientHeat && enable);
		
		// Return if there is residual heat so that charging of boiler continues
		return(sufficientHeat);
	}
	
	float TempSource()
	{
		return(TempToSystem.getRaw());
	}
	
	void getData(JsonObject& root);
	
	boolean sufficientHeat;
	
	cTempSensor TempToSystem;

	private:
	cValve Valve;
};

#endif
