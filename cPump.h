#ifndef cPump_h
#define cPump_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cLFPWM.h"
#include <cPID.h>

#define PWMPeriod 2000

class cPump
{
	public:
	/// Creates a pump object with a certain pin.
	/** The power of the pump is set to zero by default.*/
	/** \param PinPump the pin used to drive the Pump*/
	/** \param Power the power the pump in percent*/
	cPump(int PinPump_,float p= 0.0, float i= 0.0, float d= 0.0, double Power = 0.0):
	PWM(PWMPeriod),
	pid(p, i, d, DIRECT)
	{
		PinPump = PinPump_;
		pinMode(PinPump, OUTPUT);
		setPower(Power);
	}
	
	/// Set the power of the pump in percent
	/** Set the power of the pump in percent and execute
	\param Value of the desired power in percent*/
	void setPower( double Power )
	{
		// Limit Power
		Power = max(min(Power,1.0),0.0);
		
		// Pump is running if switching time of PWM is not yet exceeded
		digitalWrite(PinPump, !PWM.get(Power)); // Pump switched on if true and off if false
	}
	
	cPID pid;
	
	private:
	int PinPump;
	
	cLFPWM PWM;
	
};

#endif
