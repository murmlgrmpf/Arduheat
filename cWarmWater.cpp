//
//
//

#include "cWarmWater.h"

cWarmWater::cWarmWater(void):
Valve(PinValveWarmWaterOpen,PinValveWarmWaterClose),
pid(&_TempWarmWater, &Pump.Power, &_SpTempWarmWater, 0.01, 0.001, 0.05, DIRECT),
IsTempWarmWater(SystemMultiplexer,MultiplexTempWarmWater,OffsetTempWarmWater),
Pump(PinPumpWarmWater)
{
	_SpTempWarmWater = SpTempWarmWater;
	
	// Initialize PID controllers for pump
	pid.SetOutputLimits(0.0, 1.0);
	pid.SetMode(MANUAL);
}


void cWarmWater::Control(){
	
	// Warm Water needed
	if(FlowMeter.get()>0.0)
	{
		// Set Variable value to actual temperature for PID
		_TempWarmWater = IsTempWarmWater.get();
		
		//    timeoptimal control until 7 degrees below Setpoint
		if(_TempWarmWater<=SpTempWarmWater - 7)
		{
			//      turn the PID on
			pid.SetMode(MANUAL);
			//      open Valve
			Valve.set(true);
			//      Set Pump to maximum mass flow rate
			Pump.setPower(1.0);
		}
		//    Close valve if overshoot is occuring
		else if (_TempWarmWater>=SpTempWarmWater+2)
		{
			pid.SetMode(MANUAL);
			//     stop pump
			Pump.setPower(0.0);
			//     close valve
			Valve.set(false);
		}
		else
		{
			//      open Valve
			Valve.set(true);
			
			//************************
			//      open loop control
			//      fMassFlowPumpWarmWater = HxWarmWater.calcMassflow(SetpointTempWarmWater, 8.1560, TempBoilerTop(), FlowMeter.get() );
			//      PumpWarmWater.setMassFlowRate(fMassFlowPumpWarmWater);
			//************************
			
			//      closed loop control
			//      turn the PID on
			//      It takes over the last ouput value of the pid controller to be the initial value for integral part
			pid.SetMode(AUTOMATIC);
			pid.Compute();
			Pump.setPower(Pump.Power);
		}
	}
	else
	{
		//     stop pump
		Pump.setPower(0.0);
		//     close valve
		Valve.set(false);
	}
}

