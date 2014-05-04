// 
// 
// 

#include "cPID.h"

cPID::cPID(double _p ,double _i, double _d, int _mode):
PID(&_Is, &_Output, &_Setpoint, _p, _i, _d, _mode)
{
	_Is = 0;
	_Output = 0;
	_Setpoint = 0;
	run();
}


double cPID::run(double Setpoint, double Is) {
	_Is = Is;
	_Setpoint = Setpoint;
	SetMode(AUTOMATIC);
	Compute();
	return _Output;
}

void cPID::run( void )
{
	SetMode(MANUAL);
	Compute();
}

