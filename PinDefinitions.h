#ifndef PinDefinitions_h
#define PinDefinitions_h

#include "Arduino.h"


void PinInitialization(void);

// Universal Constants
#define DensityWater 0.99820 // [kg/l]
#define CWater 4184 // [J/(kg·K)]
//#define PumpWarmWaterMaxMassFlowRate 0.53 //[kg/s /power]
#define PumpWarmWaterMaxMassFlowRate 0.9 //[kg/s /power]

// Pin mapping Arduino
#define PinPumpCirculation        9
#define PinValveWarmWaterOpen    39
#define PinValveBoilerOpen       41
#define PinValveSolarOpen        43
#define PinValveHeatSource1Open  45
#define PinStartHeatSource1      13
#define PinValveWarmWaterClose   38
#define PinValveBoilerClose      40
#define PinValveSolarClose       42
#define PinValveHeatSource1Close 44
#define PinPumpWarmWater         10
#define PinPumpBoiler            11
#define PinPumpSolar             12
#define PinPumpHeating            8
#define PinMixerOpen              6
#define PinMixerClose             7
#define PinPulseCounter          19
#define PinHeatControl            3
// Valve Motor Sense Pins
#define PinValveWarmWaterMotorSense   65 // A11 = 65
#define PinValveBoilerMotorSense      64 // A10 = 64
#define PinValveSolarMotorSense       63 // A9 = 63
#define PinValveHeatSource1MotorSense 62 // A8 = 62
// Room Heating
#define Room1                    23
#define Room2                    24
#define Room3                    22
#define Room4                    25
#define Room5                    15
#define Room6                    26
#define Room7                    14
#define Room8                    27
#define Room9                     5
#define Room10                   28
#define Room11                   34
#define Room12                   29
#define Room13                   33
#define Room14                   30
#define Room15                   32
#define Room16                   31

// System Temperatures on Multiplexer 3
#define SystempMultiplexer                3
#define MultiplexTempHeatingLead          1
#define MultiplexTempHeatingReturn        0
#define MultiplexTempHeatSource1Lead      2 
#define MultiplexTempHeatSource1Return    3
#define MultiplexTempHeatSource1Operation 4
#define MultiplexTempSolarReturn          5
#define MultiplexTempSolarLead            6
#define MultiplexTempBoilerCharge         7
#define MultiplexTempBoilerReserve1       8
#define MultiplexTempBoilerReserve2       9
#define MultiplexTempBoilerHead          10
#define MultiplexTempBoilerTop           11
#define MultiplexTempWarmWater           12
#define MultiplexTempCirculationReturn   13
#define MultiplexIntensitySolar          14
#define MultiplexTempOutside             15
// System Temperatures 
#define OffsetTempHeatingLead             0
#define OffsetTempHeatingReturn           0
#define OffsetTempHeatSource1Lead         0 
#define OffsetTempHeatSource1Return       0
#define OffsetTempHeatSource1Operation    0
#define OffsetTempSolarReturn             0
#define OffsetTempSolarLead               0
#define OffsetTempBoilerCharge            0
#define OffsetTempBoilerReserve1          0
#define OffsetTempBoilerReserve2          0
#define OffsetTempBoilerHead              0
#define OffsetTempBoilerTop               1
#define OffsetTempWarmWater               2.35
#define OffsetTempCirculationReturn       0
#define OffsetIntensitySolar              0
#define OffsetTempOutside                 0 
#define OffsetTempHeatControl             0

// Multiplexer Control Pins
#define MultiplexControl1 48
#define MultiplexControl2 47
#define MultiplexControl3 46
#define MultiplexControl4 49
// Multiplexer Temperature Input Pins
#define MultiplexInput1 66 // A12 = 66
#define MultiplexInput2 67 // A13 = 67
#define MultiplexInput3 68 // A14 = 68

// PT1000 Sensor Pin
#define TempHeatControl 69 // A15 = 69

// Filter Coefficient
#define AlphaFilter 0


#endif
