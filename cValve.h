#ifndef cValve_h
#define cValve_h

#include "Arduino.h"

#define SenseThreshold 180 // 0.6V threshold 5V max, 1023 resolution

class cValve
{
  private:
    int _iPinOpen;
    int _iPinClose;
    int _iPinSense;
    boolean _bsetOpen;
    boolean _bisOpen;
    boolean _bisDriving;
    unsigned long _iTimeStart;
    unsigned long _iTimePeriod;
    
    void run(void);
    
  public:
    cValve(void);
    cValve(int iPinOpen,int iPinClose, int iPinSense);
    cValve(int iPinOpen,int iPinClose, int iPinSense, boolean bsetOpen);

    void setPinOpen(int iPinOpen);
    void setPinClose(int iPinClose);
    void setPinSense(int iPinSense);
    boolean get();
    void set(boolean bsetOpen);
};

#endif
