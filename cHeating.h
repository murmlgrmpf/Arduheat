#ifndef cHeating_h
#define cHeating_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "PinDefinitions.h"
#include "cBoiler.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRoom.h"
#include "cBurner.h"
#include "cSpHeating.h"
#include "cMixer.h"
#include <PID_v1.h>

class cHeating
{
	private:
	double _dIsTempHeatingLead;
	double _dSpTempHeatingLead;
	double _dIsTempHeatingReturn;
	double _dSpTempHeatingReturn;
	
	double _TempSolarReturn;
	double _TempSolarLead;
	double _IntensitySolar;

	double Input;
	double _dPowerMixer;
	boolean _bneedHeating;
	

	PID PIDPumpHeating;
	PID PIDMixer;
	cSpHeating SpHeating;
	cMixer Mixer;
	
	
	public:
	cRoom Rooms[16];
	

	cPump PumpSolar;
	cPump PumpHeating;
	cPump PumpCirculation;
	
	cValve ValveHeatSource1;
	
	cTempSingle IsTempHeatingLead;
	cTempSingle IsTempHeatingReturn;

	
	//     cHeatExchanger HxWarmWater;
	//

	cBoiler Boiler;
	cBurner Burner;
	cTemp Temperatures;
	
	cHeating(void);
	
	void Control(void);
};

#endif
