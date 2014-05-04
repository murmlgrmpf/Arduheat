#ifndef cValve_h
#define cValve_h

#include "Arduino.h"
#define iDefaultPeriod 2000

class cValve
{
  private:
    int _iPinOpen;
    int _iPinClose;
    boolean _bsetOpen;
    boolean _bisOpen;
    unsigned long _iTimeStart;
    unsigned long _iTimePeriod;
    
    void run(void);
    
  public:
    cValve(int iPinOpen,int iPinClose, unsigned long iTimePeriod = iDefaultPeriod);

    boolean get();
    void set(boolean bsetOpen);
};

#endif
