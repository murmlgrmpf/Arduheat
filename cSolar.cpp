#include "cSolar.h"

cSolar::cSolar()
:Valve(PinValveSolarOpen,PinValveSolarClose),
Pump(PinPumpSolar)
{
  Probe(true);
}

void cSolar::Probe(boolean bFlame)
{
  Pump.setPower(0.5);
}

void cSolar::run(void)
{
  
}

float cSolar::TempLead(){
   return readTemperature(SystemMultiplexer,MultiplexTempSolarLead);}
float cSolar::TempReturn(){
   return readTemperature(SystemMultiplexer,MultiplexTempSolarReturn);}
float cSolar::Intensity(){
   return readTemperature(SystemMultiplexer,MultiplexSolarIntensity);}
