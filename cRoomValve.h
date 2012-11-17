#ifndef cRoomValve_h
#define cRoomValve_h

#include "Arduino.h"


class cRoomValve
{
  private:
    int _iPinOpen;
    int _iPinClose;
    boolean _bState;
    
    void run(void);
    
  public:
    cRoomValve(void);
    cRoomValve(int iPinOpen,int iPinClose);
    cRoomValve(int iPinOpen,int iPinClose, boolean bState);
    //~Pump();
    void setPinOpen(int iPinOpen);
    void setPinClose(int iPinClose);
    boolean get();
    void set(boolean bState);
};

#endif
