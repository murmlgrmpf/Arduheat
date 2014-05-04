#ifndef cHeatSource_h
#define cHeatSource_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include "cPump.h"

class cSolar
{
	private:
	boolean _bFlame;
	void run(void);
	cTempSensor TempLead;
	cTempSensor TempReturn;
	cTempSensor Intensity;
	cValve Valve;
	cPump Pump;
	
	public:
	cSolar();
	

	
	void Probe(boolean bFlame);
};

#endif
