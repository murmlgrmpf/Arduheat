#ifndef cHeatSource_h
#define cHeatSource_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "Temperatures.h"
#include "cValve.h"
#include "cPump.h"

class cSolar
{
  private:
  boolean _bFlame;
  void run(void);

    
  public:
    cSolar();
    
    cTempSensor TempLead, TempReturn, Intensity;
    
    cValve Valve;
    cPump Pump;
    
    void Probe(boolean bFlame);
    //float TempLead();
    //float TempReturn();
    //float Intensity();
};

#endif
