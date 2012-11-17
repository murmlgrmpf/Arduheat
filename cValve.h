#ifndef cValve_h
#define cValve_h

#include "Arduino.h"

#define SenseThreshold 250 // 0.6V threshold 5V max, 1023 resolution

class cValve
{
  private:
    int _iPinOpen;
    int _iPinClose;
    boolean _bsetOpen;
    boolean _bisOpen;
    boolean _bisDriving;
    unsigned long _iTimeStart;
    unsigned long _iTimePeriod;
    
    void run(void);
    
  public:
    cValve(int iPinOpen, int iPinClose);
    cValve(int iPinOpen,int iPinClose, unsigned long iTimePeriod);

    boolean get();
    void set(boolean bsetOpen);
};

#endif
