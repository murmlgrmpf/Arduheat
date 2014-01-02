// cWarmWater.h

#ifndef _CWARMWATER_h
#define _CWARMWATER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "cFlowMeter.h"
#include "cValve.h"
#include "cTemp.h"
#include "cPump.h"
#include <PID_v1.h>
#include "PinDefinitions.h"


class cWarmWater
{
	private:
	cPump Pump;
	cTempSingle IsTempWarmWater;
	cValve Valve;
	PID pid;
	double _SpTempWarmWater;
	double _TempWarmWater;

	public:
	cWarmWater();
	cFlowMeter FlowMeter;
	void Control();
};


#endif

