// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "cFlowMeter.h"
#include "cTemp.h"
#include "cPump.h"
#include "PinDefinitions.h"
#include "cPID.h"


class cWarmWater
{
	private:
	cPump Pump;
	cTempSensor IsTempWarmWater;
	cPID pid;

	public:
	cWarmWater();
	cFlowMeter FlowMeter;
	void Control();
};


#endif

