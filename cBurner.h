#ifndef cBurner_h
#define cBurner_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include <ArduinoJson.h>

#define MinBurnTimeMinutes 20
#define MaxTempOperation 80

class cBurner
{
	
	public:
	cTempSensor TempLead;
	cTempSensor TempOperation;
	cTempSensor TempReturn;
	
	cBurner(void);
	boolean isBurning(void);
	boolean burn(boolean, double);
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	
	private:
	cValve Valve;
	
	double _MaxTempOperation;
	
	unsigned long _StartTime;
	unsigned long _MinBurnTime;
	
	boolean _sufficientHeat;
	boolean _bFlame;
};

#endif
