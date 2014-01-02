#include "cSolar.h"

cSolar::cSolar()
:Valve(PinValveSolarOpen,PinValveSolarClose),
Pump(PinPumpSolar),
TempLead(SystemMultiplexer,MultiplexTempSolarLead,OffsetTempSolarLead),
TempReturn(SystemMultiplexer,MultiplexTempSolarReturn,OffsetTempSolarReturn),
Intensity(SystemMultiplexer,MultiplexSolarIntensity,OffsetSolarIntensity)
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
