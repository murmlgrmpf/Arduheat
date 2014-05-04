#ifndef cBurner_h
#define cBurner_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#define MinBurnTimeMinutes 5
#define MaxTempOperation 80

class cBurner
{
	private:
	cValve Valve;
	
	unsigned long _StartTime;
	unsigned long _MinBurnTime;
	
	boolean _sufficientHeat;
	boolean _bFlame;

	public:
	cTempSensor TempLead;
	cTempSensor TempOperation;
	cTempSensor TempReturn;
	
	cBurner(void);
	boolean isBurning(void);
	boolean burn(boolean, double);
	
};

#endif
