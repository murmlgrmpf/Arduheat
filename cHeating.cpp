#include "cHeating.h"


cHeating::cHeating(void)
:ValveWarmWater(PinValveWarmWaterOpen,PinValveWarmWaterClose), 
ValveBoiler(PinValveBoilerOpen,PinValveBoilerClose),
ValveHeatSource1(PinValveHeatSource1Open,PinValveHeatSource1Close)
{
  Temperatures;
  FlowMeter;
  HxWarmWater;
  SetpointTempWarmWater = 45.0;
  
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
  
}

float cHeating::TempHeatingLead(){
    return (OffsetTempHeatingLead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatingLead));}
float cHeating::TempHeatingReturn(){
    return (OffsetTempHeatingReturn + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatingReturn));}
float cHeating::TempHeatSource1Lead(){
    return (OffsetTempHeatSource1Lead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatSource1Lead));}
float cHeating::TempHeatSource1Return(){
    return (OffsetTempHeatSource1Lead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatSource1Return));}
float cHeating::TempHeatSource1Operation(){
    return (OffsetTempHeatSource1Operation + Temperatures.getTemp(SystempMultiplexer,MultiplexTempHeatSource1Operation));}
float cHeating::TempSolarReturn(){
    return (OffsetTempSolarReturn + Temperatures.getTemp(SystempMultiplexer,MultiplexTempSolarReturn));}
float cHeating::TempSolarLead(){
    return (OffsetTempSolarLead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempSolarLead));}
float cHeating::TempBoilerCharge(){
    return (OffsetTempBoilerCharge + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerCharge));}
float cHeating::TempBoilerReserve1(){
    return (OffsetTempBoilerReserve1 + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerReserve1));}
float cHeating::TempBoilerReserve2(){
    return (OffsetTempBoilerReserve2 + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerReserve2));}
float cHeating::TempBoilerHead(){
    return (OffsetTempBoilerHead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerHead));}
float cHeating::TempBoilerTop(){
    return (OffsetTempBoilerTop + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerTop));}
float cHeating::TempWarmWater(){
    return (OffsetTempWarmWater + Temperatures.getTemp(SystempMultiplexer,MultiplexTempWarmWater));}
float cHeating::TempCirculationReturn(){
    return (OffsetTempCirculationReturn + Temperatures.getTemp(SystempMultiplexer,MultiplexTempCirculationReturn));}
float cHeating::IntensitySolar(){
    return (OffsetIntensitySolar + Temperatures.getTemp(SystempMultiplexer,MultiplexIntensitySolar));}
float cHeating::TempOutside(){
    return (OffsetTempOutside + Temperatures.getTemp(SystempMultiplexer,MultiplexTempOutside));}
