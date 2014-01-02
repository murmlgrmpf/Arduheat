#include "cHeating.h"


cHeating::cHeating(void)
:ValveWarmWater(PinValveWarmWaterOpen,PinValveWarmWaterClose),
ValveHeatSource1(PinValveHeatSource1Open,PinValveHeatSource1Close),
PIDWarmWater(&_TempWarmWater, &PumpWarmWater.Power, &_SpTempWarmWater, 0.01, 0.001, 0.05, DIRECT),
PIDPumpHeating(&_dIsTempHeatingReturn, &PumpHeating.Power, &_dSpTempHeatingReturn, 0.001, 0.0001, 0.005, DIRECT),
PIDMixer(&_dIsTempHeatingLead, &_dPowerMixer, &_dSpTempHeatingLead, 0.001, 0.0, 0.005, DIRECT),
IsTempHeatingLead(SystemMultiplexer,MultiplexTempHeatingLead,OffsetTempHeatingLead),
IsTempHeatingReturn(SystemMultiplexer,MultiplexTempHeatingReturn,OffsetTempHeatingReturn),
PumpWarmWater(PinPumpWarmWater),
PumpSolar(PinPumpSolar),
PumpHeating(PinPumpHeating),
PumpCirculation(PinPumpCirculation)
{
	_dPowerMixer = 0;
	_SpTempWarmWater = SpTempWarmWater;
	
	// Initialize room numbers and Pins
	for(int i = 0; i<16; i++)
	{
		// The rooms know their multiplexers and pin out by the room number
		Rooms[i].init(i+1);
	}
	
	// Initialize PID controllers for pumps
	PIDWarmWater.SetOutputLimits(0.0, 1.0);
	PIDWarmWater.SetMode(MANUAL);
	
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


void cHeating::ControlWarmWater(){
	
	// Warm Water needed
	if(FlowMeter.get()>0.0)
	{
		// Set Variable value to actual temperature for PID
		_TempWarmWater = TempWarmWater();
		
		//    timeoptimal control until 7 degrees below Setpoint
		if(TempWarmWater()<=SpTempWarmWater - 7)
		{
			//      turn the PID on
			PIDWarmWater.SetMode(MANUAL);
			//      open Valve
			ValveWarmWater.set(true);
			//      Set Pump to maximum mass flow rate
			PumpWarmWater.setPower(1.0);
		}
		//    Close valve if overshoot is occuring
		else if (TempWarmWater()>=SpTempWarmWater+2)
		{
			PIDWarmWater.SetMode(MANUAL);
			//     stop pump
			PumpWarmWater.setPower(0.0);
			//     close valve
			ValveWarmWater.set(false);
		}
		else
		{
			//      open Valve
			ValveWarmWater.set(true);
			
			//************************
			//      open loop control
			//      fMassFlowPumpWarmWater = HxWarmWater.calcMassflow(SetpointTempWarmWater, 8.1560, TempBoilerTop(), FlowMeter.get() );
			//      PumpWarmWater.setMassFlowRate(fMassFlowPumpWarmWater);
			//************************
			
			//      closed loop control
			//      turn the PID on
			//      It takes over the last ouput value of the pid controller to be the initial value for integral part
			PIDWarmWater.SetMode(AUTOMATIC);
			PIDWarmWater.Compute();
			PumpWarmWater.setPower(PumpWarmWater.Power);
		}
	}
	else
	{
		//     stop pump
		PumpWarmWater.setPower(0.0);
		//     close valve
		ValveWarmWater.set(false);
	}
}


double cHeating::TempSolarReturn(){
return (OffsetTempSolarReturn + Temperatures.getTemp(SystemMultiplexer,MultiplexTempSolarReturn));}
double cHeating::TempSolarLead(){
return (OffsetTempSolarLead + Temperatures.getTemp(SystemMultiplexer,MultiplexTempSolarLead));}
double cHeating::TempWarmWater(){
return (OffsetTempWarmWater + Temperatures.getTemp(SystemMultiplexer,MultiplexTempWarmWater));}
double cHeating::IntensitySolar(){
return (OffsetSolarIntensity + Temperatures.getTemp(SystemMultiplexer,MultiplexSolarIntensity));}