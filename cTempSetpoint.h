#ifndef cTempSetpoint_h
#define cTempSetpoint_h

#include "Arduino.h"
#include "PinDefinitions.h"


class cTemp
{
  private:
    float _TempFilt[16][3];
    
    void run(void);
    
    float _alphaT;

    unsigned long _TimePeriod;
    unsigned long _LastTime;
    
  public:
    cTemp(void);
    float getTemp(int iMultiplexNumber,int iMultiplexConnector);
    
};

float readTemperature(int iMultiplexNumber,int iMultiplexConnector);
void setMultiplexer(int i);
#endif
