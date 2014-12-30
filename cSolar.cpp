#include "cSolar.h"

cSolar::cSolar()
:Valve(PinValveSolarOpen,PinValveSolarClose),
Pump(PinPumpSolar),
TempLead(SystemMultiplexer,MultiplexTempSolarLead,OffsetTempSolarLead),
TempReturn(SystemMultiplexer,MultiplexTempSolarReturn,OffsetTempSolarReturn),
Intensity(SystemMultiplexer,MultiplexSolarIntensity,OffsetSolarIntensity)
{
  probe(true);
}

void cSolar::probe(boolean bFlame_)
{
  Pump.setPower(0.0);
}

void cSolar::run(void)
{
  Pump.setPower(0.0);
}
