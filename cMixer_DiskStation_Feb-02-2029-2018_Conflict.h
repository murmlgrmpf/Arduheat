#ifndef cMixer_h
#define cMixer_h

#include "Arduino.h"

#include "cLFPWM.h"
#include "cTrigger.h"
#include <PID_v1.h>

class cMixer  : public PID
{
	public:
	/// Creates the mixer
	cMixer(int pinOpen_, int pinClose_,double p, double i, double d, int mode):
	PWM(20000),
	Timer(180000),// 3(min)*60(s/min)*1000(ms/s)
	PID(&Is, &Power, &Setpoint, p, i, d, mode)
	{
		pinOpen = pinOpen_;
		pinMode_wrap(pinOpen, OUTPUT);
		
		pinClose = pinClose_;
		pinMode_wrap(pinClose, OUTPUT);
		
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
		// Start Sequence in order to initialize I-portion
		if (GetMode()==MANUAL)
			Power = 0.5;// Test - was 0.0

		SetMode(AUTOMATIC);
		Compute();
		
		run();
		
		return Power;
	}
	
	double get(void){return Power;}
	
	private:
	cLFPWM PWM;
  cTimer Timer;
	int pinOpen;
	int pinClose;
	
	double Is;
	double Setpoint;
	double Power;
  boolean direction;
	
	void run(void)
	{
                // If direction did change or power is less than 100%: reset timer
                if ((direction != (Power>=0))||(abs(Power)<1.0))
                    Timer.restart();
                
		direction = (Power>=0); // set current direction
                
		// Check PWM and hysteresis and Timer for limit switch off
		if((PWM.get(abs(Power)))&&(abs(Power)> 0.06)&&(Timer.get())) {
			// Drive in direction
			digitalWrite_wrap(pinClose, !direction);
			digitalWrite_wrap(pinOpen, direction);
		}
		else {
			// Stop
			digitalWrite_wrap(pinClose, LOW);
			digitalWrite_wrap(pinOpen, LOW);
		}
	}
};

#endif
