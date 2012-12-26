#ifndef cRoom_h
#define cRoom_h

#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"
//#include "cHeating.h"
// Highest and lowest value for manual reference override
#define TempLow 5
#define TempHigh 30

class cRoom
{
  private:
    int _iRoomNumber;
    float _fTempActual;
    float _fTempRef;
    int _iMultiplexNumber;
    int _iMultiplexConnectorActual;
    int _iMultiplexConnectorRef;
    //int _iPinValve;
    float _Heatflow;
    float _fTempRefSchedule;
    
  public:
    /// Creates a room.
    /** As the rooms get created in an array, only the default constructor can be used */
    cRoom(void);
    //~cRoom();
    /// Each room has a valve
    cRoomValve Valve;
    
    void setRoomNumber(int iRoomNumber);
    void setTempRefSchedule(float fTempRefSchedule);
    float getTempActual(void);
    float getTempRef(void);
    float getHeatflow(void);
};

#endif
