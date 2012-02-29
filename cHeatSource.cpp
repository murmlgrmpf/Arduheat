#include "cHeatSource.h"

cHeatSource::cHeatSource()
: Valve(PinValveHeatSource1Open,PinValveHeatSource1Close)
{
}

void cHeatSource::Flame(boolean bFlame)
{
  _bFlame = bFlame;
  run();
}

void cHeatSource::run(void)
{
  if (_bFlame)
    digitalWrite(PinStartHeatSource1, HIGH);
  else
  {
    digitalWrite(PinStartHeatSource1, LOW);};
}

float cHeatSource::TempLead(){
    return readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Lead);}
float cHeatSource::TempReturn(){
    return readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Return);}
float cHeatSource::TempOperation(){
    return readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Operation);}
