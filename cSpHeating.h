// cSpHeating.h

#ifndef _CSPHEATING_h
#define _CSPHEATING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "PinDefinitions.h"
#include "cTemp.h"

class cSpHeating
{
 private:
	double _dsteil;
	double _dkh;
	double _dverst;
	double _dminvl;
	double _dmaxvl;
	cTempSensor TempOutside;


 public:
	cSpHeating(void);
	double get(double,double);
	
};


#endif

