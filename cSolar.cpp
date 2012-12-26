#include "cSolar.h"

cSolar::cSolar()
:Valve(PinValveSolarOpen,PinValveSolarClose)
{
}

void cSolar::Probe(boolean bFlame)
{
  Pump.setMassFlowRate(0.5);
}

void cSolar::run(void)
{
  if (_bFlame)
  {
    digitalWrite(PinStartHeatSource1, HIGH);}
  else
  {
    digitalWrite(PinStartHeatSource1, LOW);};
}

float cSolar::TempLead(){
   return readTemperature(SystempMultiplexer,MultiplexTempSolarLead);}
float cSolar::TempReturn(){
   return readTemperature(SystempMultiplexer,MultiplexTempSolarReturn);}
float cSolar::Intensity(){
   return readTemperature(SystempMultiplexer,MultiplexIntensitySolar);}
