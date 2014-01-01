#ifndef cBurner_h
#define cBurner_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"

class cBurner
{
  private:
  cValve Valve;
  cTempSingle TempLead;
  cTempSingle TempReturn;
  cTempSingle TempOperation;// 80 Grad Notabschaltung
  
  boolean _bResidualHeat;

  
  public:
    boolean _bFlame;
    unsigned long _StartTime;
    unsigned long _MinBurnTime;
  
  cBurner(void);
  boolean burn(boolean, double); 
  
};

#endif
