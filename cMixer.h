#ifndef cMixer_h
#define cMixer_h

#include "Arduino.h"

#include "cLFPWM.h"
#include <PID_v1.h>

class cMixer  : public PID
{
	public:
	/// Creates the mixer
	cMixer(int pinOpen_, int pinClose_,double p, double i, double d, int mode):
	PWM(20000),
	PID(&Is, &Power, &Setpoint, p, i, d, mode)
	{
		pinOpen = pinOpen_;
		pinMode(pinOpen, OUTPUT);
		
		pinClose = pinClose_;
		pinMode(pinClose, OUTPUT);
		
		Is = 0;
		Power = 0;
		Setpoint = 0;
	}
	
	/// Set the power in percent
	/** Drives the mixer with a certain power (abs) into a direction (sign: open or close)
	\param Power the desired power in percent */
	void run(double Power_)
	{
		Power = Power_;
		SetMode(MANUAL);
		Compute();
		
		run();
	}
	
	double run(double Setpoint_, double Is_)
	{
		Is = Is_;
		Setpoint = Setpoint_;
		SetMode(AUTOMATIC);
		Compute();
		
		run();
		
		return Power;
	}
	
	double get(void){return Power;}
	
	private:
	cLFPWM PWM;
	int pinOpen;
	int pinClose;
	
	double Is;
	double Setpoint;
	double Power;
	
	void run()
	{
		boolean direction = (Power>=0);
		// Check PWM and hysteresis
		if(PWM.get(abs(Power))&&(abs(Power)> 0.05)) {
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
