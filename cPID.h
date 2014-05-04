// cPID.h

#ifndef _CPID_h
#define _CPID_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <PID_v1.h>

class cPID: public PID
{
	private:
	double _Is;

	double _Setpoint;

	public:
	double _Output;
	cPID(double _p,double _i ,double _d, int _mode);
	double run(double Setpoint, double Is);
	void run(void);
};


#endif

