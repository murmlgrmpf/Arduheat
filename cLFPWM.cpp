//
//
//

#include "cLFPWM.h"

cLFPWM::cLFPWM( void )
{
	cLFPWM( DefaulTimePeriod );
}

cLFPWM::cLFPWM( unsigned long TimePeriod )
{
	setPeriod( TimePeriod );
}



void cLFPWM::setPeriod( unsigned long TimePeriod )
{
	_TimePeriod = TimePeriod;
}


void cLFPWM::setPower( double Power )
{
	_Power = min(abs(Power),1);
}


boolean cLFPWM::get( double Power )
{
	//Adjust power level
	setPower(Power);
	// get state of PWM cycle
	return get();
}

boolean cLFPWM::get( void )
{
	boolean active = false;
	unsigned long _EndTime;
	unsigned long _SwitchTime;
	
	// Set the end time of the current Period
	_EndTime = round(_StartTime + _TimePeriod);
	// Set the next switching Time
	_SwitchTime = round(_StartTime + _Power*_TimePeriod);
	
	// active if Time is less than TimeSwitch
	active = (millis()<_SwitchTime);

	// TimePeriod is over and new period starts
	if(millis()>=_EndTime)
	{
		//Set the next Last Period Time
		_StartTime = millis();
	}
	
	return active;
}

