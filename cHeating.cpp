#include "cHeating.h"


cHeating::cHeating(void)
{
  //Create Rooms
  //cRoom Rooms[16];
  
  // Initialize room numbers and Pins
  for(int i = 0; i<16; i++)
  {
    Rooms[i].setRoomNumber(i+1);
    
//    Rooms[i].Valve.setPinOpen(i+26);
//    pinMode(Rooms[i].getPinValve(), OUTPUT);
//    digitalWrite(Rooms[i].getPinValve(), HIGH);
  }
  // Create Pumps
  PumpWarmWater.setPinPump(PinPumpWarmWater);
  PumpBoiler.setPinPump(PinPumpBoiler);
  PumpSolar.setPinPump(PinPumpSolar);
  PumpHeating.setPinPump(PinPumpHeating);
  PumpCirculation.setPinPump(PinPumpCirculation);
  // Create Valves
  ValveWarmWater.setPinOpen(PinValveWarmWaterOpen);
  ValveWarmWater.setPinClose(PinValveWarmWaterClose);
  ValveBoiler.setPinOpen(PinValveBoilerOpen);
  ValveBoiler.setPinClose(PinValveBoilerClose);
  ValveSolar.setPinOpen(PinValveSolarOpen);
  ValveSolar.setPinClose(PinValveSolarClose);
  ValveHeatSource1.setPinOpen(PinValveHeatSource1Open);
  ValveHeatSource1.setPinClose(PinValveHeatSource1Close);
  
  
  
}

float cHeating::TempHeatingLead(){
    return (OffsetTempHeatingLead + readTemperature(SystempMultiplexer,MultiplexTempHeatingLead));}
float cHeating::TempHeatingReturn(){
    return (OffsetTempHeatingReturn + readTemperature(SystempMultiplexer,MultiplexTempHeatingReturn));}
float cHeating::TempHeatSource1Lead(){
    return (OffsetTempHeatSource1Lead + readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Lead));}
float cHeating::TempHeatSource1Return(){
    return (OffsetTempHeatSource1Lead + readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Return));}
float cHeating::TempHeatSource1Operation(){
    return (OffsetTempHeatSource1Operation + readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Operation));}
float cHeating::TempSolarReturn(){
    return (OffsetTempSolarReturn + readTemperature(SystempMultiplexer,MultiplexTempSolarReturn));}
float cHeating::TempSolarLead(){
    return (OffsetTempSolarLead + readTemperature(SystempMultiplexer,MultiplexTempSolarLead));}
float cHeating::TempBoilerCharge(){
    return (OffsetTempBoilerCharge + readTemperature(SystempMultiplexer,MultiplexTempBoilerCharge));}
float cHeating::TempBoilerReserve1(){
    return (OffsetTempBoilerReserve1 + readTemperature(SystempMultiplexer,MultiplexTempBoilerReserve1));}
float cHeating::TempBoilerReserve2(){
    return (OffsetTempBoilerReserve2 + readTemperature(SystempMultiplexer,MultiplexTempBoilerReserve2));}
float cHeating::TempBoilerHead(){
    return (OffsetTempBoilerHead + readTemperature(SystempMultiplexer,MultiplexTempBoilerHead));}
float cHeating::TempBoilerTop(){
  TempBoilerTopFilter = AlphaFilter*TempBoilerTopFilter + (1-AlphaFilter)*(OffsetTempBoilerTop + readTemperature(SystempMultiplexer,MultiplexTempBoilerTop));
    return TempBoilerTopFilter;}
float cHeating::TempWarmWater(){
  TempWarmWaterFilter = AlphaFilter*TempWarmWaterFilter + (1-AlphaFilter)*(OffsetTempWarmWater + readTemperature(SystempMultiplexer,MultiplexTempWarmWater));
    return TempWarmWaterFilter;}
float cHeating::TempCirculationReturn(){
    return (OffsetTempCirculationReturn + readTemperature(SystempMultiplexer,MultiplexTempCirculationReturn));}
float cHeating::IntensitySolar(){
    return (OffsetIntensitySolar + readTemperature(SystempMultiplexer,MultiplexIntensitySolar));}
float cHeating::TempOutside(){
    return (OffsetTempOutside + readTemperature(SystempMultiplexer,MultiplexTempOutside));}
