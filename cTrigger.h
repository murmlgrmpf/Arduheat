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
		if(active) restart();
		
		return active;
	}
	
	void setTimePeriod(unsigned long TimePeriod_){
		TimePeriod = TimePeriod_;
	}	
	void restart(void) {StartTime = millis();}
	
	private:
	unsigned long TimePeriod;
	unsigned long StartTime;
};

class cTimer: public cTrigger
{
public:
    cTimer(unsigned long TimePeriod_):
    cTrigger(TimePeriod_)
    {
        active=true;
    }
    
    boolean get(void) {
        if (active && cTrigger::get())
            active = false;
        
        return active;
    }
    
    void restart(void) {
        cTrigger::restart();
        active = true;
    }
    
private:
    boolean active;
};


#endif

