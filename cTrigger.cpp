// 
// 
// 

#include "cTrigger.h"

 cTrigger::cTrigger( unsigned long TimePeriod/*= 100*/ )
{
	_TimePeriod = TimePeriod;
	_StartTime = millis();
}

boolean cTrigger::get( void )
{
	boolean active;
	// Check if trigger should be activated
	active = (millis() > _StartTime+_TimePeriod);
	
	// If trigger is hit restart timer
	if(active) _StartTime = millis();
	
	return active;
}

