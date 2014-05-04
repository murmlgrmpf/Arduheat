#include "cBoiler.h"


cBoiler::cBoiler(void)
:Valve(PinValveBoilerOpen,PinValveBoilerClose),
PIDBoilerCharge( 0.02, 0.005, 0, REVERSE),
TempCharge(SystemMultiplexer,MultiplexTempBoilerCharge,OffsetTempBoilerCharge),
TempReserve1(SystemMultiplexer,MultiplexTempBoilerReserve1,OffsetTempBoilerReserve1),
TempReserve2(SystemMultiplexer,MultiplexTempBoilerReserve2,OffsetTempBoilerReserve2),
TempHead(SystemMultiplexer,MultiplexTempBoilerHead,OffsetTempBoilerHead),
TempTop(SystemMultiplexer,MultiplexTempBoilerTop,OffsetTempBoilerTop),
PumpBoilerCharge(PinPumpBoiler)
{
	// Set minimal Pump Power to about 20%
	PIDBoilerCharge.SetOutputLimits(0.2, 1);
}

double cBoiler::getSpTempCharge(void)
{
	if(needChargeWarmWater())
	{
		// Else choose maximum Charging temperature
		_SpTempCharge = max(SpTempChargeWarmWater(), SpTempChargeHeating());
	}
	else
	{
		// If Warmwater is full, load into heating
		_SpTempCharge = SpTempChargeHeating();
	}
	
	return _SpTempCharge;
}

double cBoiler::SpTempChargeWarmWater(void)
{
	double TempChargeWarmWater = SpTempWarmWater+ChargeWarmWaterOffset;
	
	return TempChargeWarmWater;
}

double cBoiler::SpTempChargeHeating(void)
{
	double TempChargeHeating = max(_SpTempHeatingLead+ChargeHeatingOffset, TempReserve1.get()+ChargeHeatingOffset);
	
	return TempChargeHeating;
}


void cBoiler::charge(boolean bCharge)
{
	_bCharging = bCharge;
	if (bCharge)
	{	
		// Run Pump
		PumpBoilerCharge.setPower(PIDBoilerCharge.run(getSpTempCharge(), TempCharge.get()));
	}
	else // Stop Charging
	{
		// Stop PID
		PIDBoilerCharge.run();
		
		// Stop Pump
		PumpBoilerCharge.setPower(0.0);
	}
	
	// Open Valve if charging or discharging
	setValve();
}


/* Switches the Pump on and off corresponding to the power level
After each Period the length of the period gets adjusted.
*/
boolean cBoiler::needChargeHeating(boolean needHeating  = false)
{
	if( HeatingEmpty() && (needHeating) )
	_needChargeHeating = true;
	
	if( HeatingFull() )
	_needChargeHeating = false;
	
	return _needChargeHeating;
}

boolean cBoiler::HeatingEmpty(void)
{
	// need = Setpoint - Actual Value
	boolean empty = _SpTempHeatingLead > TempReserve1.get();
	
	return empty;
}

boolean cBoiler::HeatingFull(void)
{
	// need = Setpoint - Actual Value
	boolean full = _SpTempHeatingLead < TempReserve2.get();
	
	return full;
}


boolean cBoiler::needChargeWarmWater(void)
{
	if (WarmWaterEmpty()>0.0) _needChargeWarmWater = true;
	
	if (WarmWaterFull()>0.0) _needChargeWarmWater = false;
	
	return _needChargeWarmWater;
}

double cBoiler::WarmWaterEmpty(void)
{
	// need = Setpoint - Actual Value
	double need = SpTempWarmWater-1 - TempTop.get();
	
	return need;
}

double cBoiler::WarmWaterFull(void)
{
	// have = -(Setpoint - Actual Value)
	double have = -(SpTempWarmWater+1 - TempHead.get());
	
	return have;
}

void cBoiler::setSpTempHeatingLead( double SpTempHeatingLead )
{
	_SpTempHeatingLead = SpTempHeatingLead;
}

void cBoiler::discharge( boolean bNeedSourceBoiler )
{
	_bDischarging = bNeedSourceBoiler;
	
	setValve();
}

boolean cBoiler::needCharge( boolean bneedHeating = false )
{
	return (needChargeWarmWater() || needChargeHeating( bneedHeating));
}

void cBoiler::setValve( void )
{
	Valve.set((_bDischarging || _bCharging));
}

void cBoiler::triggerChargeWarmWater()
{
	_needChargeWarmWater = true;
}
