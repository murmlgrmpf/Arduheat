#ifndef cPump_h
#define cPump_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cLFPWM.h"

#define PWMPeriod 4000

class cPump
{
	public:
	/// Creates a pump object with a certain pin.
	/** The power of the pump is set to zero by default.*/
	/** \param PinPump the pin used to drive the Pump*/
	/** \param Power the power the pump in percent*/
	cPump(int PinPump_, double Power_ = 0.0):PWM(PWMPeriod)
	{
		PinPump = PinPump_;
		pinMode(PinPump, OUTPUT);
		setPower(Power_);
	}
	
	/// Set the power of the pump in percent
	/** Set the power of the pump in percent and execute
	\param Value of the desired power in percent*/
	void setPower( double Power_ )
	{
		// Limit Power
		Power = max(min(Power_,1.0),0.0);
		
		// Pump is running if switching time of PWM is not yet exceeded
		digitalWrite(PinPump, !PWM.get(Power)); // Pump switched on if true and off if false
	}
	
	/// Get the power of the pump in percent
	/** \return power of the pump in percent */
	double getPower(void){return Power;}
	
	private:
	int PinPump;
	double Power;
	
	cLFPWM PWM;
	
};

#endif
