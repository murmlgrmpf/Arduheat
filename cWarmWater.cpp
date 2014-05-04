#include "cWarmWater.h"

cWarmWater::cWarmWater(void):
pid(0.5, 0.01, 0.1, DIRECT),
IsTempWarmWater(SystemMultiplexer,MultiplexTempWarmWater,OffsetTempWarmWater),
Pump(PinPumpWarmWater)
{
	// Initialize PID controllers for pump
	pid.SetOutputLimits(0.0, 1.0);
}

void cWarmWater::Control(){
	// Warm Water needed
	if(FlowMeter.get())
	{
		// Execute the pump with the power determined by pid controller
		Pump.setPower(pid.run(SpTempWarmWater, IsTempWarmWater.get()));
	}
	else
	{
		// switch pid to manual
		pid.run();
		// stop pump
		Pump.setPower(0.0);
	}
}