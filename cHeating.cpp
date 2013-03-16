#include "cHeating.h"


cHeating::cHeating(void)
:ValveWarmWater(PinValveWarmWaterOpen,PinValveWarmWaterClose), 
ValveBoiler(PinValveBoilerOpen,PinValveBoilerClose),
ValveHeatSource1(PinValveHeatSource1Open,PinValveHeatSource1Close),
PIDWarmWater(&_TempWarmWater, &PumpWarmWater.Power, &SpTempWarmWater, 0.01, 0.001, 0.05, DIRECT)
{
  Temperatures;
  FlowMeter;
  HxWarmWater;
  SpTempWarmWater = 45.0;
  
  // Initialize room numbers and Pins
  for(int i = 0; i<16; i++)
  {
    // The rooms know their multiplexers and pinout by the room number
    Rooms[i].setRoomNumber(i+1);
  }
  // Initialize Pumps
  PumpWarmWater.setPinPump(PinPumpWarmWater);
  PumpWarmWater.setMaxMassFlowRate(PumpWarmWaterMaxMassFlowRate);
  PumpBoiler.setPinPump(PinPumpBoiler);
  PumpSolar.setPinPump(PinPumpSolar);
  PumpHeating.setPinPump(PinPumpHeating);
  PumpCirculation.setPinPump(PinPumpCirculation);
  
  // Initialize PID controller
  PIDWarmWater.SetOutputLimits(0.0, 1.0);
}

void cHeating::BurnerControl(){
  // If temperature of Boiler top is lower than required for WarmWater
  if (TempBoilerTop() < SpTempWarmWater+2){
    //
    //SpTempBoilerCharge = SpTempWarmWater+4;
  }
  
  
  
  
  
}


void cHeating::ControlWarmWater(){
  
  if(FlowMeter.get()>0.0)
  {
   _TempWarmWater = TempWarmWater();
    
   // timeoptimal control until 5 degrees below Setpoint
   if(TempWarmWater()<=SpTempWarmWater - 7)
   {
     // turn the PID on
     PIDWarmWater.SetMode(MANUAL);
     // open Valve
     ValveWarmWater.set(true);
     // Set Pump to maximum mass flow rate
     PumpWarmWater.setMassFlowRate(PumpWarmWater.getMaxMassFlowRate());
   }
   // Close valve if overshoot is occuring
   else if (TempWarmWater()>=SpTempWarmWater+2)
   {
    PIDWarmWater.SetMode(MANUAL);
    // stop pump
    PumpWarmWater.setMassFlowRate(0.0);
    // close valve
    ValveWarmWater.set(false);
   }
   else
   {
     // open Valve
     ValveWarmWater.set(true);
     
     // open loop control
//      fMassFlowPumpWarmWater = HxWarmWater.calcMassflow(SetpointTempWarmWater, 8.1560, TempBoilerTop(), FlowMeter.get() );
//      PumpWarmWater.setMassFlowRate(fMassFlowPumpWarmWater);
     
     // closed loop control
     // turn the PID on
     // It takes over the last ouput value of the pid controller to be the initial value for integral part
     PIDWarmWater.SetMode(AUTOMATIC);
     PIDWarmWater.Compute();
     PumpWarmWater.setMassFlowRate(PumpWarmWater.Power*PumpWarmWater.getMaxMassFlowRate());
   }
  }
  else
  {
    // stop pump
    PumpWarmWater.setMassFlowRate(0.0);
    // close valve
    ValveWarmWater.set(false);
  }
}

double cHeating::TempHeatingLead(){
    return (OffsetTempHeatingLead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatingLead));}
double cHeating::TempHeatingReturn(){
    return (OffsetTempHeatingReturn + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatingReturn));}
double cHeating::TempHeatSource1Lead(){
    return (OffsetTempHeatSource1Lead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatSource1Lead));}
double cHeating::TempHeatSource1Return(){
    return (OffsetTempHeatSource1Lead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatSource1Return));}
double cHeating::TempHeatSource1Operation(){
    return (OffsetTempHeatSource1Operation + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatSource1Operation));}
double cHeating::TempSolarReturn(){
    return (OffsetTempSolarReturn + Temperatures.getTemp(SystempMultiplexer,MultiplexTempSolarReturn));}
double cHeating::TempSolarLead(){
    return (OffsetTempSolarLead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempSolarLead));}
double cHeating::TempBoilerCharge(){
    return (OffsetTempBoilerCharge + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerCharge));}
double cHeating::TempBoilerReserve1(){
    return (OffsetTempBoilerReserve1 + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerReserve1));}
double cHeating::TempBoilerReserve2(){
    return (OffsetTempBoilerReserve2 + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerReserve2));}
double cHeating::TempBoilerHead(){
    return (OffsetTempBoilerHead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerHead));}
double cHeating::TempBoilerTop(){
    return (OffsetTempBoilerTop + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerTop));}
double cHeating::TempWarmWater(){
    return (OffsetTempWarmWater + Temperatures.getTemp(SystempMultiplexer,MultiplexTempWarmWater));}
double cHeating::TempCirculationReturn(){
    return (OffsetTempCirculationReturn + Temperatures.getTemp(SystempMultiplexer,MultiplexTempCirculationReturn));}
double cHeating::IntensitySolar(){
    return (OffsetIntensitySolar + Temperatures.getTemp(SystempMultiplexer,MultiplexIntensitySolar));}
double cHeating::TempOutside(){
    return (OffsetTempOutside + Temperatures.getTemp(SystempMultiplexer,MultiplexTempOutside));}
