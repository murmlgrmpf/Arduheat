#ifndef PinDefinitions_h
#define PinDefinitions_h

#include "Arduino.h"


void PinInitialization(void);

// Pin mapping Arduino
#define PinPumpCirculation    14 //
#define PinValveWarmWaterOpen 15 //
#define PinValveBoilerOpen     17 //
#define PinValveSolarOpen     24 //
#define PinValveHeatSource1Open 22 //
#define PinStartHeatSource1   6
#define PinValveWarmWaterClose 16 //
#define PinValveBoilerClose    18 //
#define PinValveSolarClose    25 //
#define PinValveHeatSource1Close 23 //
#define PinPumpWarmWater      14 //
#define PinPumpBoiler         42 //
#define PinPumpSolar          7 //
#define PinPumpHeating        3 //
#define PinMixerOpen          4
#define PinMixerClose         5
#define PinPulseCounter       19
// System Temperatures on Multiplexer 3
#define SystempMultiplexer             3
#define MultiplexTempHeatingLead       0
#define MultiplexTempHeatingReturn     1
#define MultiplexTempHeatSource1Lead   2 
#define MultiplexTempHeatSource1Return 3
#define MultiplexTempHeatSource1Operation 4
#define MultiplexTempSolarReturn       5
#define MultiplexTempSolarLead         6
#define MultiplexTempBoilerCharge      7
#define MultiplexTempBoilerReserve1    8
#define MultiplexTempBoilerReserve2    9
#define MultiplexTempBoilerHead       10
#define MultiplexTempBoilerTop        11
#define MultiplexTempWarmWater        12
#define MultiplexTempCirculationReturn 13
#define MultiplexIntensitySolar       14
#define MultiplexTempOutside          15
// System Temperatures 
#define OffsetTempHeatingLead       0
#define OffsetTempHeatingReturn     0
#define OffsetTempHeatSource1Lead   0 
#define OffsetTempHeatSource1Return 0
#define OffsetTempHeatSource1Operation 0
#define OffsetTempSolarReturn       0
#define OffsetTempSolarLead         0
#define OffsetTempBoilerCharge      0
#define OffsetTempBoilerReserve1    0
#define OffsetTempBoilerReserve2    0
#define OffsetTempBoilerHead       0
#define OffsetTempBoilerTop        1
#define OffsetTempWarmWater        2.35
#define OffsetTempCirculationReturn 0
#define OffsetIntensitySolar       0
#define OffsetTempOutside          0 

// Multiplexer Control Pins
#define MultiplexControl1 46
#define MultiplexControl2 47
#define MultiplexControl3 48
#define MultiplexControl4 49
// Multiplexer Temperature Input Pins
#define MultiplexInput1 66
// A12 = 66
#define MultiplexInput2 67
// A13 = 67
#define MultiplexInput3 68
// A14 = 68

// Filter Coefficient
#define AlphaFilter 0.7


#endif
