// cLFPWM.h

#ifndef _CLFPWM_h
#define _CLFPWM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Default period is 10sec
#define DefaulTimePeriod 10000

class cLFPWM
{
	private:
	unsigned long _TimePeriod;
	unsigned long _StartTime;
	
	double _Power;
	void setPower(double Power);

	public:
	cLFPWM(void);
	cLFPWM(unsigned long TimePeriod);
	void setPeriod(unsigned long TimePeriod);

	boolean get(double Power);
	boolean get(void);
	
};

#endif

