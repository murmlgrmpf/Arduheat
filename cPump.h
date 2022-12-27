#ifndef cPump_h
#define cPump_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cLFPWM.h"
#include <cPID.h>

#define PWMPeriod 4000

class cPump : public PID
{
	public:
	/// Creates a pump object with a certain pin.
	/** The power of the pump is set to zero by default.*/
	/** \param PinPump the pin used to drive the Pump*/
	/** \param Power the power the pump in percent*/
	cPump(int PinPump_,float p, float i, float d, int mode, double Power_ = 0.0, unsigned long PWMPeriod_ = PWMPeriod):
	PID(&Is, &Power, &Setpoint, p, i, d, mode),
	PWM(PWMPeriod_)	
	{
		PinPump = PinPump_;
		pinMode(PinPump, OUTPUT);
		
		Is = 0;
		Power = Power_;
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
		digitalWrite(PinPump, PWM.get(Power)); // Pump switched on if true and off if false
		return Power;
	}
	/// Regulate the power by pid controller
	/** Update setpoint and is value, then execute pid controller and set pump
	\param Setpoint_ update Setpoint value
	\param Is_ update Is value*/
	double run(double Setpoint_, double Is_, double PowerStatic_ = 0.0)
	{
		Is = Is_;
		Setpoint = Setpoint_;
		SetMode(AUTOMATIC);
		
		Compute();
		PWM.setSampleTime(160/min(0.03, max(0.002, (Power+PowerStatic_))));
		// Pump is running if switching time of PWM is not yet exceeded
		digitalWrite(PinPump, PWM.get(Power+PowerStatic_)); // Pump switched on if true and off if false
		return Power;
		
	}
	
	double get(void)
	{
		return Power;
	}
	
	
	//private:
	cLFPWM PWM;
	int PinPump;
	double Is;
	double Setpoint;
	double Power;
};

#endif
