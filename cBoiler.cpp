#include "cBoiler.h"


cBoiler::cBoiler(void)
:Valve(PinValveBoilerOpen,PinValveBoilerClose),
PIDBoilerCharge(&_TempBoilerCharge, &PumpBoilerCharge.Power, &_SpTempBoilerCharge, 0.02, 0.005, 0, REVERSE),
TempBoilerCharge(SystemMultiplexer,MultiplexTempBoilerCharge,OffsetTempBoilerCharge),
TempBoilerReserve1(SystemMultiplexer,MultiplexTempBoilerReserve1,OffsetTempBoilerReserve1),
TempBoilerReserve2(SystemMultiplexer,MultiplexTempBoilerReserve2,OffsetTempBoilerReserve2),
TempBoilerHead(SystemMultiplexer,MultiplexTempBoilerHead,OffsetTempBoilerHead),
TempBoilerTop(SystemMultiplexer,MultiplexTempBoilerTop,OffsetTempBoilerTop),
PumpBoilerCharge(PinPumpBoiler)
{
  // Set minimal Pump Power to about 10%
  PIDBoilerCharge.SetOutputLimits(0.2, 1);
  PIDBoilerCharge.SetMode(MANUAL);
}


double cBoiler::SpTempChargeWarmWater(void)
{
  double TempChargeWarmWater = SpTempWarmWater+ChargeWarmWaterOffset;
  
  return TempChargeWarmWater;
}


double cBoiler::SpTempChargeHeating(void)
{
  double TempChargeHeating = max(_SpTempHeatingLead+ChargeHeatingOffset, TempBoilerReserve1.get()+ChargeHeatingOffset);
  
  return TempChargeHeating;
}


void cBoiler::charge(boolean bSourceReady,double SpCharge)
{
	if (bSourceReady)
	{
		// Open Charge valve
		Valve.set(true);
		
		// Set setpoint for PID Controller
		_SpTempBoilerCharge = SpCharge;
		// Read in charging temperature for PID Controller
		_TempBoilerCharge = TempBoilerCharge.get();

		// Run PID for Pump
		//   Time optimal control
		//   if _SpTempBoilerCharge
		PIDBoilerCharge.SetMode(AUTOMATIC);
		PIDBoilerCharge.Compute();
		
		// Run Pump
		PumpBoilerCharge.setPower(PumpBoilerCharge.Power);
	}
	else // Stop Charging
	{
		// Stop PID
		PIDBoilerCharge.SetMode(MANUAL);
		  
		// Stop Pump
		PumpBoilerCharge.setPower(0.0);
		  
		// Close Charge valve
		Valve.set(false);
	}	
}


/* Switches the Pump on and off corresponding to the power level
   After each Period the length of the period gets adjusted.
*/
double cBoiler::needHeating(void)
{
  // need = Setpoint - Actual Value
  double need = _SpTempHeatingLead -TempBoilerReserve2.get();
  
  return need;
}


double cBoiler::haveHeating(void)
{
  // have = -(Setpoint - Actual Value)
  double have = -(_SpTempHeatingLead - TempBoilerReserve1.get());
  
  return have;
}


double cBoiler::needWarmWater(void)
{
  // need = Setpoint - Actual Value
  double need = SpTempWarmWater - TempBoilerTop.get();
  
  return need;
}


double cBoiler::haveWarmWater(void)
{
  // have = -(Setpoint - Actual Value)
  double have = -(SpTempWarmWater - TempBoilerHead.get());
  
  return have;
}

void cBoiler::setSpTempHeatingLead( double SpTempHeatingLead )
{
	_SpTempHeatingLead = SpTempHeatingLead;
}
