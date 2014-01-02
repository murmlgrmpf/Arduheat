#ifndef cPump_h
#define cPump_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cLFPWM.h"

class cPump
{
	private:
	int _PinPump;
	float _Power;
	
	cLFPWM PWM;
	
	public:
	double Power;
	
	/// Creates a pump object with a certain pin.
	/** The power of the pump is set to zero by default.*/
	/** \param PinPump the pin used to drive the Pump*/
	/** \param Power the power the pump in percent*/
	cPump(int PinPump, float fPower = 0.0);
	/// Set the power of the pump in percent
	/** Set the power of the pump in percent and execute
	\param Value of the desired power in percent*/
	void setPower(float fPower);
	/// Get the power of the pump in percent
	/** \return power of the pump in percent */
	float getPower(void);

};

#endif
