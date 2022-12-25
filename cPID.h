// cPID.h

#ifndef _CPID_h
#define _CPID_h

#include <PID_v1.h>

class cPID : public PID
{
	public:
	
	cPID(double p,double i ,double d, int mode):
	PID(&Is, &Output, &Setpoint, p, i, d, mode)
	{
			Is = 0;
			Output = 0;
			Setpoint = 0;
			run();
	}
	
	double run(double Setpoint_, double Is_)
	{
		Is = Is_;
		Setpoint = Setpoint_;
		SetMode(AUTOMATIC);
		Compute();
		return Output;
	}
	
	double run(double Output_ = 0.0)
	{
		Output = Output_;
		SetMode(MANUAL);
		Compute();
		return Output;
	}
	
	double get(void)
	{
		return Output;
	}
	
	private:
	double Is;
	double Setpoint;
	double Output;
};


#endif

