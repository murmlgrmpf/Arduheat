#include "cHeating.h"


cHeating::cHeating(void):
ValveHeatSource1(PinValveHeatSource1Open,PinValveHeatSource1Close),
PIDPumpHeating(&_dIsTempHeatingReturn, &PumpHeating.Power, &_dSpTempHeatingReturn, 0.001, 0.0001, 0.005, DIRECT),
PIDMixer(&_dIsTempHeatingLead, &_dPowerMixer, &_dSpTempHeatingLead, 0.001, 0.0, 0.005, DIRECT),
IsTempHeatingLead(SystemMultiplexer,MultiplexTempHeatingLead,OffsetTempHeatingLead),
IsTempHeatingReturn(SystemMultiplexer,MultiplexTempHeatingReturn,OffsetTempHeatingReturn),
PumpSolar(PinPumpSolar),
PumpHeating(PinPumpHeating),
PumpCirculation(PinPumpCirculation)
{
	_dPowerMixer = 0;
	
	// Initialize room numbers and Pins
	for(int i = 0; i<16; i++)
	{
		// The rooms know their multiplexers and pin out by the room number
		Rooms[i].init(i+1);
	}
	
	// Initialize PID controllers for pumps
	PIDPumpHeating.SetOutputLimits(0.4, 1.0);
	PIDPumpHeating.SetMode(MANUAL);
	
	PIDMixer.SetOutputLimits(-1.0, 1.0);
	PIDMixer.SetMode(MANUAL);
	
}

void cHeating::Control(void){
	double SpTempBoilerCharge = 77.0;
	double SpTempHeating = 0 ;
	double needBurner = 0.0;
	boolean bneedBurner = false;
	boolean bBurnerReady = false;
	double dMaxDiff =0;
	double dMaxSp = 0;
	double dneedHeating = 0;
	
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
		// Store maximum set point
		if (Rooms[i].getSpTemp()>dMaxSp){
			dMaxSp = Rooms[i].getSpTemp();
		}
		// Store maximum sp-is difference
		if ((Rooms[i].getSpTemp() - Rooms[i].getIsTemp()) > dMaxDiff){
			dMaxDiff = Rooms[i].getSpTemp() - Rooms[i].getIsTemp();
		}
		// Check for rooms needing heat
		if(Rooms[i].getNeed()>dneedHeating){
			dneedHeating = Rooms[i].getNeed();
		}
	}
	
	// Calculate setpoint for heating
	_dSpTempHeatingLead = SpHeating.get(dMaxSp, dMaxDiff);
	Boiler.setSpTempHeatingLead(_dSpTempHeatingLead);
	_dIsTempHeatingLead = IsTempHeatingLead.get();
	_dSpTempHeatingReturn = _dIsTempHeatingLead-10;
	_dIsTempHeatingReturn = IsTempHeatingReturn.get();
	
	// Determine whether there is need for heating
	_bneedHeating = (dneedHeating>0);
	
	// Run Heating only if it is needed and no warm water is needed
	if ((!(Boiler.needWarmWater()>0.0))&& (_bneedHeating))
	{
		// Run Mixer and PID
		PIDMixer.SetMode(AUTOMATIC);
		PIDMixer.Compute();
		Mixer.run(_dPowerMixer);
		
		// Run heating pump and PID
		PIDPumpHeating.SetMode(AUTOMATIC);
		PIDPumpHeating.Compute();
		PumpHeating.setPower(PumpHeating.Power);
	}
	else
	{
		// Stop Pump Heating and PID
		PIDPumpHeating.SetMode(MANUAL);
		PumpHeating.setPower(0.0);
		// Stop mixer PID and run Mixer to closed position
		PIDMixer.SetMode(MANUAL);
		Mixer.run(-1.0);
	}
	
	// Run solar pump to avoid boiling
	PumpSolar.setPower(0.0);
	
	if(Boiler.haveWarmWater() > 0.0)
	{ // If Warmwater is full, load into heating
		SpTempBoilerCharge = Boiler.SpTempChargeHeating();
	}
	else
	{ // Else choose maximum Charging temperature
		SpTempBoilerCharge = max(Boiler.SpTempChargeWarmWater(), Boiler.SpTempChargeHeating());
	}
	
	// Ignite Burner
	bneedBurner = ((Boiler.needWarmWater()>0.0) || ((Boiler.needHeating()>0.0)&&(dneedHeating>0.0)));
	bBurnerReady = Burner.burn(bneedBurner, SpTempBoilerCharge);
	// Charge Boiler
	Boiler.charge(bBurnerReady, SpTempBoilerCharge);
	
}

