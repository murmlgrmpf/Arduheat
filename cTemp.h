#ifndef cTemp_h
#define cTemp_h

#include "Arduino.h"
#include "PinDefinitions.h"


//! Resistor Values in Ohm
#define R 2000.0
//! Resistor Values in Ohm
#define R1 12000.0
//! Resistor Values in Ohm
#define R2 11000.0
//! Resistor Values in Ohm
#define R3 10000.0
//! Voltage of power supply
#define Vcc 5.05
//! Factors for Temperature Sensors
#define Alpha 0.00788
//! Factors for Temperature Sensors
#define Beta 0.00001937
//! Factors for Temperature Sensors
//1980-2020
#define R25 2000.0
//!Resistance of Multiplexer 1
#define RM1 57.3248//331.606//358.21//265.45
//!Resistance of Multiplexer 2
#define RM2 55.3797//37.647//312.258
//!Resistance of Multiplexer 3
#define RM3 50.309//330.853

#define _AlphaT 0.1

//extern float TemperaturesArray[16][3] ={0};
class cTemp
{
  private:
    float _TempRaw[16][3];
    float _TempFilt[16][3];
    
    void run(void);
    
//     void getTemperatures(float TemperatureArray[][3]);
    void getTemperatures(void);
    
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
