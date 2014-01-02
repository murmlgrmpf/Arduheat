#include "cPump.h"


cPump::cPump(int PinPump, float fPower):
PWM(4000)
{
	_PinPump = PinPump;
	Power = fPower;
	setPower(Power);
}


void cPump::setPower( float fPower )
{
	_Power = fPower;
	
	// Limit _Power
	if (_Power>1.0)
	{
		_Power = 1.0;
	}
	else if(_Power<0.0)
	{
		_Power = 0.0;
	}
	
	// Pump is running if switching time of PWM is not yet exceeded
	digitalWrite(_PinPump, !PWM.get(_Power)); // Pump switched on if true and off if false
}


float cPump::getPower(void)
{
	return _Power;
}
