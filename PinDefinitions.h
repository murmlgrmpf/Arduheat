#ifndef PinDefinitions_h
#define PinDefinitions_h

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
#define PinPumpWarmWater         2
#define PinPumpBoiler            11
#define PinPumpSolar             12
#define PinPumpHeating            8
#define PinMixerOpen              6
#define PinMixerClose             7
#define PinWarmWaterSwitch       19
#define PinHeatControl            3

// System Temperatures on Multiplexer 3
#define SystemMultiplexer                 2
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

// Multiplexer Control Pins
const int MultiplexControl[] = {48, 47, 46, 49};

// Multiplexer Temperature Input Pins A12 = 66; A13 = 67; A14 = 68
const int MultiplexInput[] = {66, 67, 68};

const int MultiplexChannelRoomsIs[] = {14,12,10,8,6,4,2,0,14,12,10,8,6,4,2,0};
const int MultiplexChannelRoomsSp[] = {15,13,11,9,7,5,3,1,15,13,11,9,7,5,3,1};
const int MultiplexNumberRooms[] =    { 0, 0, 0,0,0,0,0,0, 1, 1, 1,1,1,1,1,1};


#endif
