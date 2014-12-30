// cTrigger.h

#ifndef _CTRIGGER_h
#define _CTRIGGER_h

#include "Arduino.h"

class cTrigger
{
	public:
	cTrigger(unsigned long TimePeriod_= 100)
	{
		TimePeriod = TimePeriod_;
		StartTime = millis();
	}
	
	boolean get(void)
	{
		// Check if trigger should be activated
		boolean active = (millis()-StartTime > TimePeriod);
		
		// When trigger is hit: restart timer
		if(active) StartTime = millis();
		
		return active;
	}
	
	private:
	unsigned long TimePeriod;
	unsigned long StartTime;
};


#endif

