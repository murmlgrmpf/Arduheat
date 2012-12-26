#ifndef cHeatSource_h
#define cHeatSource_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"

class cHeatSource
{
  private:
  boolean _bFlame;
  void run(void);

    
  public:
    cHeatSource();
    
    cValve Valve;
    
    
    void Flame(boolean bFlame);
    float TempLead();
    float TempReturn();
    float TempOperation();// 80 Grad Notabschaltung
    // Probing: Valveopen, Pulsen, Heizungladen, Vorrang, regeln!
    
};

#endif
