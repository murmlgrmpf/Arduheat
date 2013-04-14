#include "cBurner.h"

cBurner::cBurner()
:Valve(PinValveHeatSource1Open,PinValveHeatSource1Close)
{
  _MinBurnTime = 20*60*1000;// 20 Minutes in Milliseconds
}

double cBurner::haveTemp(void)
{
  _fTempHave = TempOperation();
  
  if ((_bFlame==true)&& (TempOperation()>30)){
    _fTempHave= 100;
  }
  
  
  return _fTempHave;
}


void cBurner::run(boolean bBurn)
{
  unsigned long time = millis();
  
  if ((bBurn == true) && (_bFlame == false)){
    _StartTime = time;
    _bFlame = true;
  }
  
  if ((bBurn == false) && (time > _StartTime + _MinBurnTime)) {
    _bFlame = false;
    
    _bRemainingHeat = true; // Remaining Heat sequence triggered
  }
  
  if (_bFlame){
    Valve.set(true);
    digitalWrite(PinStartHeatSource1, LOW); // Start Burner (start on Low)
  }
  else {
    digitalWrite(PinStartHeatSource1, HIGH); // Stop Burner (stop on High)
  }
}

void cBurner::stop(void)
{
  // Shut off Flame
  _bFlame= false;
  run(false);
  
  // Close Valve
  Valve.set(false);
  
}

float cBurner::TempLead(){
    return readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Lead);}
float cBurner::TempReturn(){
    return readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Return);}
float cBurner::TempOperation(){
    return readTemperature(SystempMultiplexer,MultiplexTempHeatSource1Operation);}
