// cTrigger.h

#ifndef _CTRIGGER_h
#define _CTRIGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class cTrigger
{
	private:
	unsigned long _TimePeriod;
	unsigned long _StartTime;

	public:
	cTrigger(unsigned long TimePeriod= 100);
	boolean get(void);
};


#endif

