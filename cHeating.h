#ifndef cHeating_h
#define cHeating_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cBoiler.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRoom.h"
#include "cFlowMeter.h"
#include "cHeatExchanger.h"
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
	double _TempWarmWater;
	double _IntensitySolar;
	double _SpTempWarmWater;
	double Input;
	double _dPowerMixer;
	boolean _bneedHeating;
	
	PID PIDWarmWater;
	PID PIDPumpHeating;
	PID PIDMixer;
	cSpHeating SpHeating;
	cMixer Mixer;
	
	
	public:
	cRoom Rooms[16];
	
	cPump PumpWarmWater;
	cPump PumpSolar;
	cPump PumpHeating;
	cPump PumpCirculation;
	
	cValve ValveWarmWater;
	cValve ValveHeatSource1;
	
	cTempSingle IsTempHeatingLead;
	cTempSingle IsTempHeatingReturn;
	
	//     cHeatExchanger HxWarmWater;
	//
	cFlowMeter FlowMeter;
	cBoiler Boiler;
	cBurner Burner;
	cTemp Temperatures;
	
	cHeating(void);
	
	double TempSolarReturn();
	double TempSolarLead();
	double TempWarmWater();
	double IntensitySolar();
	
	void ControlWarmWater();
	void Control(void);
};

#endif
