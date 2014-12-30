#ifndef cMixer_h
#define cMixer_h

#include "Arduino.h"

#include "cLFPWM.h"

class cMixer
{
  private:
	cLFPWM PWM;
	int pinOpen;
	int pinClose;
    
  public:
	/// Creates the mixer
	cMixer(int pinOpen_, int pinClose_):PWM(5000){
		pinOpen = pinOpen_;
		pinMode(pinOpen, OUTPUT);
		
		pinClose = pinClose_;
		pinMode(pinClose, OUTPUT);
	};
	
	/// Set the power in percent
	/** Drives the mixer with a certain power (abs) into a direction (sign: open or close)
	\param Power the desired power in percent */
	void run(double Power)
	{
		boolean direction = (Power>=0);
		
		// Check PWM and hysteresis
		if(PWM.get(abs(Power))&&(abs(Power)> 0.1)) {
			// Drive in direction
			digitalWrite(pinClose, direction);
			digitalWrite(pinOpen, !direction);
		}
		else {
			// Stop
			digitalWrite(pinClose, HIGH);
			digitalWrite(pinOpen, HIGH);
		}
	}
};

#endif
