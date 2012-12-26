#ifndef cHeatSource_h
#define cHeatSource_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include "cPump.h"

class cSolar
{
  private:
  boolean _bFlame;
  void run(void);

    
  public:
    cSolar();
    
    cValve Valve;
    cPump Pump;
    
    void Probe(boolean bFlame);
    float TempLead();
    float TempReturn();
    float Intensity();
};

#endif
