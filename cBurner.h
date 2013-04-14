#ifndef cBurner_h
#define cBurner_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"

class cBurner
{
  private:
    boolean _bFlame;
    boolean _bRemainingHeat;
    double _fTempHave;
    
    
    
    unsigned long _StartTime;
    unsigned long _MinBurnTime;
    
  public:
    cBurner(void);
    
    cValve Valve;
    void run(boolean);
    double haveTemp(void);
    boolean Burn(boolean bFlame);
    void stop(void);
    float TempLead();
    float TempReturn();
    float TempOperation();// 80 Grad Notabschaltung
    // Probing: Valveopen, Pulsen, Heizungladen, Vorrang, regeln!
    
};

#endif
