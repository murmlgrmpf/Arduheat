#include "cMixer.h"


cMixer::cMixer(void):
PWM(5000)
{

}

/* Switches the Pump on and off corresponding to the power level
After each Period the length of the period gets adjusted.
*/
void cMixer::run(double Power)
{
	boolean direction;
	
	direction = (Power>=0);
	
	// Check PWM and hysteresis
	if(PWM.get(Power)&&(abs(Power)> 0.1))
	{
		// Drive in direction
		digitalWrite(PinMixerClose, direction);
		digitalWrite(PinMixerOpen, !direction);
	}
	else
	{
		// Stop
		digitalWrite(PinMixerClose, HIGH);
		digitalWrite(PinMixerOpen, HIGH);
	}
}

