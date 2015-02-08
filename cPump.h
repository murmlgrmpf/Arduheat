#ifndef cPump_h
#define cPump_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cLFPWM.h"
#include <PID_v1.h>

#define PWMPeriod 2000

class cPump : public PID
{
	public:
	/// Creates a pump object with a certain pin.
	/** The power of the pump is set to zero by default.*/
	/** \param PinPump the pin used to drive the Pump*/
	/** \param Power the power the pump in percent*/
	cPump(int PinPump_,float p, float i, float d, int mode, double Power = 0.0):
	PWM(PWMPeriod),
	PID(&Is, &Power, &Setpoint, p, i, d, mode)
	{
		PinPump = PinPump_;
		pinMode(PinPump, OUTPUT);
		
		Is = 0;
		Power = 0;
		Setpoint = 0;
		
		run();
	}
	/// Set the power of the pump in percent
	/** Set the power of the pump in percent and execute
	\param Power_ in percent*/
	double run(double Power_ = 0.0)
	{
		Power = Power_;
		SetMode(MANUAL);
		
		Compute();
		// Pump is running if switching time of PWM is not yet exceeded
		digitalWrite(PinPump, !PWM.get(Power)); // Pump switched on if true and off if false
		return Power;
	}
	/// Regulate the power by pid controller
	/** Update setpoint and is value, then execute pid controller and set pump
	\param Setpoint_ update Setpoint value
	\param Is_ update Is value*/
	double run(double Setpoint_, double Is_)
	{
		Is = Is_;
		Setpoint = Setpoint_;
		SetMode(AUTOMATIC);
		
		Compute();
		// Pump is running if switching time of PWM is not yet exceeded
		digitalWrite(PinPump, !PWM.get(Power)); // Pump switched on if true and off if false
		return Power;
	}
	
	double get(void)
	{
		return Power;
	}
	
	private:
	int PinPump;
	double Is;
	double Setpoint;
	double Power;
	
	cLFPWM PWM;
	
};

#endif
