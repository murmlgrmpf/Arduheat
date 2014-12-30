// cLFPWM.h

#ifndef _CLFPWM_h
#define _CLFPWM_h

#include "Arduino.h"

class cLFPWM
{
	public:
	// Default period is 10sec
	cLFPWM(unsigned long SampleTime_ = 10000) 
	{
		SampleTime = SampleTime_;
		LastTime = millis();
		Power = 0.0;
	}
	
	boolean get( void )
	{
		unsigned long elapsedTime = millis()-LastTime;
		
		// Interval in which the PWM is high:
		unsigned long activeTime = round(Power*SampleTime);
		
		// active if elapsedTime is less than activeTime, false if it is more
		boolean active = (elapsedTime <= activeTime);
		
		// SampleTime is over and new period starts
		if(elapsedTime >= SampleTime) {
			//Set the next Last Period Time
			LastTime = millis();
			// Set active if switch time for next period is not exceeded
			active = (elapsedTime <= (activeTime+SampleTime));
		}
		
		return active;
	}
	
	boolean get( double Power_ )
	{
		// Adjust power level
		Power = min(max(Power_,0),1);
		// get state of PWM cycle
		return get();
	}
	
	private:
	unsigned long SampleTime;
	unsigned long LastTime;
	double Power;
};

#endif
