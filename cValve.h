#ifndef cValve_h
#define cValve_h

#include "Arduino.h"


class cValve
{
  private:
    int _iPinOpen;
    int _iPinClose;
    boolean _bState;
    
    void run(void);
    
  public:
    cValve(void);
    cValve(int iPinOpen,int iPinClose);
    cValve(int iPinOpen,int iPinClose, boolean bState);
    //~Pump();
    void setPinOpen(int iPinOpen);
    void setPinClose(int iPinClose);
    boolean get();
    void set(boolean bState);
};

#endif
