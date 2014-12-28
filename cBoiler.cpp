#include "cBoiler.h"


cBoiler::cBoiler(cRooms* _Rooms,cWarmWater* _WarmWater)
:Valve(PinValveBoilerOpen,PinValveBoilerClose),
PIDBoilerCharge( 0.2, 0.1, 0.05, REVERSE),
TempCharge(SystemMultiplexer,MultiplexTempBoilerCharge,OffsetTempBoilerCharge),
TempReserve1(SystemMultiplexer,MultiplexTempBoilerReserve1,OffsetTempBoilerReserve1),
TempReserve2(SystemMultiplexer,MultiplexTempBoilerReserve2,OffsetTempBoilerReserve2),
TempHead(SystemMultiplexer,MultiplexTempBoilerHead,OffsetTempBoilerHead),
TempTop(SystemMultiplexer,MultiplexTempBoilerTop,OffsetTempBoilerTop),
PumpBoilerCharge(PinPumpBoiler)
{
	Rooms = _Rooms;
	WarmWater = _WarmWater;
	_ChargeHeatingAddon = ChargeHeatingOffset;
	_ChargeWarmWaterAddon = ChargeWarmWaterOffset;
	// Set minimal Pump Power to about 10%
	PIDBoilerCharge.SetOutputLimits(0.1, 1);
	
}

double cBoiler::getSpTempCharge(void)
{
	double TempChargeHeating = max(Rooms->getSpHeating()+_ChargeHeatingAddon, TempReserve1.get()+_ChargeHeatingAddon);
	double TempChargeWarmWater = WarmWater->getSpTemp()+_ChargeWarmWaterAddon;
	
	if(needChargeWarmWater())
	{
		// Else choose maximum Charging temperature
		_SpTempCharge = max(TempChargeWarmWater, TempChargeHeating);
	}
	else
	{
		// If Warmwater is full, load into heating
		_SpTempCharge = TempChargeHeating;
	}
	
	return _SpTempCharge;
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
		// Stop PID and Pump
		PumpBoilerCharge.setPower(PIDBoilerCharge.run());
	}
	
	// Open Valve if charging or discharging
	setValve();
}

/* Switches the Pump on and off corresponding to the power level
After each Period the length of the period gets adjusted.
*/
boolean cBoiler::needChargeHeating(boolean needHeating)
{
	// need = Setpoint - Actual Value
	if( (Rooms->getSpHeating() > TempReserve1.get()) && (needHeating) )
		_needChargeHeating = true;
	
	// need = Setpoint - Actual Value
	// Reset hysteresis if boiler is full or no need for heating the rooms
	if( (Rooms->getSpHeating() < TempReserve2.get()) || (!needHeating) )
		_needChargeHeating = false;
	
	// Loading the boiler is only needed if hysteresis is triggered and rooms need heating.
	return (_needChargeHeating && needHeating);
}

boolean cBoiler::needChargeWarmWater(void)
{
	// need = Setpoint - Actual Value
	if (WarmWater->getSpTemp()-1 - TempTop.get()>0.0) _needChargeWarmWater = true;
	
	// have = -(Setpoint - Actual Value)
	if (-(WarmWater->getSpTemp()+1 - TempHead.get())>0.0) _needChargeWarmWater = false;
	
	return _needChargeWarmWater;
}

void cBoiler::getSP( JsonObject& root )
{
	root["BoilerChargeHeatingOffset"] = _ChargeHeatingAddon;
	root["BoilerChargeWarmWaterOffset"] = _ChargeWarmWaterAddon;
}

int cBoiler::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("BoilerChargeHeatingOffset")) {
		if(root["BoilerChargeHeatingOffset"].is<double>()) {
			_ChargeHeatingAddon =  root["BoilerChargeHeatingOffset"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("BoilerChargeWarmWaterOffset")) {
		if(root["BoilerChargeWarmWaterOffset"].is<double>()) {
			_ChargeWarmWaterAddon =  root["BoilerChargeWarmWaterOffset"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if (fail == 0) { // If all three parameter objects were successfully filled
		return posReturn;
	}
	else
	{
		return 0;
	}
}

void cBoiler::getData( JsonObject& root )
{
	root["BoilerValve"] =  static_cast<int>( Valve.get());

	root["BoilerTempCharge"] = TempCharge.get();
	root["BoilerTempTop"] = TempTop.get();
	root["BoilerTempHead"] = TempHead.get();
	root["BoilerTempReserve1"] = TempReserve1.get();
	root["BoilerTempReserve2"] = TempReserve2.get();
	
	root["BoilerPumpBoilerCharge"] =  PumpBoilerCharge.getPower();
	root["BoilerneedChargeWarmWater"] =  static_cast<int>( _needChargeWarmWater);
	root["BoilerneedChargeHeating"] =  static_cast<int>( _needChargeHeating);
	root["BoilerbDischarging"] =  static_cast<int>( _bDischarging);
	root["BoilerbCharging"] =  static_cast<int>( _bCharging);
	root["BoilerSpTempWarmWater"] =  WarmWater->getSpTemp();
	root["BoilerSpTempCharge"] =   _SpTempCharge;
}
