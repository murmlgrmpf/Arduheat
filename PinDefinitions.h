#ifndef PinDefinitions_h
#define PinDefinitions_h

#include "Arduino.h"


void PinInitialization(void);

// Universal Constants
#define DensityWater 0.99820 // [kg/l]
#define CWater 4184 // [J/(kg·K)]
//#define PumpWarmWaterMaxMassFlowRate 0.53 //[kg/s /power]
// #define PumpWarmWaterMaxMassFlowRate 0.9 //[kg/s /power]
#define PumpWarmWaterMaxMassFlowRate 0.626 //[kg/s /power]

//#define PumpBoilerChargeMaxMassFlowRate 0.626 //[kg/s /power] Not measured yet!!
#define PumpBoilerChargeMaxMassFlowRate 1

// Pin mapping Arduino
#define PinPumpCirculation        9
#define PinValveWarmWaterOpen    39
#define PinValveBoilerOpen       41
#define PinValveSolarClose       42
#define PinValveSolarOpen        43
#define PinValveHeatSource1Close 44
#define PinValveHeatSource1Open  45
#define PinStartHeatSource1      13
#define PinValveWarmWaterClose   38
#define PinValveBoilerClose      40
#define PinPumpWarmWater         10
#define PinPumpBoiler            11
#define PinPumpSolar             12
#define PinPumpHeating            8
#define PinMixerOpen              6
#define PinMixerClose             7
#define PinPulseCounter          19
#define PinHeatControl            3

// Room Heating
#define nRooms					16

// Calibration needed!
const double RoomIsOffset[] = {2, 1.5, 1.0, 1.0, 0.0, 4.0, 0.5, 0.5, 1.0, 1.0, 2.0, 2.5, 2.0, 0.0, 1.0, 2.0};
// Calibration by measurements
const double RoomSpOffset[] = {0, -0.61, 0.62, 0.31, -0.3, 1.07, 1.99, -0.61, -4.43, -0.27, -0.12, 0.95, 0.95, 0.64, 1.25, 0.18};

// Pinout definition
const int RoomValvePin[] = {23, 24, 22, 25, 15, 26, 14, 27, 5, 28, 34, 29, 33, 30, 32, 31};
//#define Room1                    23
//#define Room2                    24
//#define Room3                    22
//#define Room4                    25
//#define Room5                    15
//#define Room6                    26
//#define Room7                    14
//#define Room8                    27
//#define Room9                     5
//#define Room10                   28
//#define Room11                   34
//#define Room12                   29
//#define Room13                   33
//#define Room14                   30
//#define Room15                   32
//#define Room16                   31

// System Temperatures on Multiplexer 3
#define SystemMultiplexer                3
#define MultiplexTempHeatingReturn        0
#define MultiplexTempHeatingLead          1
#define MultiplexTempHeatSource1Lead      3 //2 
#define MultiplexTempHeatSource1Return    2 //3
#define MultiplexTempHeatSource1Operation 4
#define MultiplexTempSolarReturn          5
#define MultiplexTempSolarLead            6
#define MultiplexTempBoilerCharge         7
#define MultiplexTempBoilerReserve2       8
#define MultiplexTempBoilerReserve1       9
#define MultiplexTempBoilerHead          10
#define MultiplexTempBoilerTop           11
#define MultiplexTempWarmWater           12
#define MultiplexTempCirculationReturn   13
#define MultiplexSolarIntensity          14
#define MultiplexTempOutside             15
// System Temperatures 
#define OffsetTempHeatingLead             0 //6
#define OffsetTempHeatingReturn           0 //-1
#define OffsetTempHeatSource1Lead         0 
#define OffsetTempHeatSource1Return       0
#define OffsetTempHeatSource1Operation    0
#define OffsetTempSolarReturn             0
#define OffsetTempSolarLead               0
#define OffsetSolarIntensity              0
#define OffsetTempBoilerCharge            0 //7
#define OffsetTempBoilerReserve1          0
#define OffsetTempBoilerReserve2          0
#define OffsetTempBoilerHead              0
#define OffsetTempBoilerTop               0 //1
#define OffsetTempWarmWater               0 //2.35
#define OffsetTempCirculationReturn       0
#define OffsetTempOutside                 0 
#define OffsetTempHeatControl             0
// Setpoint System Temperatures
#define ChargeHeatingOffset				4.0
#define ChargeWarmWaterOffset			4.0
#define SpTempWarmWater                  42.0

// Multiplexer Control Pins
#define MultiplexControl1 48
#define MultiplexControl2 47
#define MultiplexControl3 46
#define MultiplexControl4 49
// Multiplexer Temperature Input Pins
#define MultiplexInput1 66 // A12 = 66
#define MultiplexInput2 67 // A13 = 67
#define MultiplexInput3 68 // A14 = 68

// Filter Coefficient
#define AlphaFilter 0


#endif
