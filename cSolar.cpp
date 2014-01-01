#include "cSolar.h"

cSolar::cSolar()
:Valve(PinValveSolarOpen,PinValveSolarClose)
{
  Pump.setPinPump(PinPumpSolar);
  Pump.setMaxMassFlowRate(1);
  Probe(true);
}

void cSolar::Probe(boolean bFlame)
{
  Pump.setMassFlowRate(0.5*Pump.getMaxMassFlowRate());
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
