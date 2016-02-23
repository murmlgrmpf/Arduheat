#ifndef PinDefinitions_h
#define PinDefinitions_h

// Pin mapping Arduino
#define PinPumpCirculation       6 // 9
#define PinValveBoilerOpen       41
#define PinValveSolarClose       42
#define PinValveSolarOpen        43
#define PinValveHeatSource1Close 44
#define PinValveHeatSource1Open  45
#define PinStartHeatSource1      8  //13
#define PinValveBoilerClose      40
#define PinPumpWarmWater         5  // 2
#define PinPumpBoiler            7  //11
#define PinPumpSolar             14 //12
#define PinPumpHeating           15 // 8
#define PinMixerOpen             9  // 6
#define PinMixerClose            16 // 7
#define PinWarmWaterSwitch       19
#define PinHeatControl           12 // 3
#define PinSolarIntensity	       A3

// System Temperatures
#define idxTempHeatingReturn        0
#define idxTempHeatingLead          1
#define idxTempSolarFromCollector   3 //2 
#define idxTempSolarFromSystem      2 //3
#define idxTempHeatSource1Operation 4
#define idxTempSolarToCollector     5
#define idxTempSolarToSystem        6
#define idxTempBoilerCharge         7
#define idxTempBoilerReserve2       8
#define idxTempBoilerReserve1       9
#define idxTempBoilerHead          10
#define idxTempBoilerTop           11
#define idxTempWarmWater           12
#define idxTempWarmWaterToBoiler   13 // Not Used
// #define idxSolarIntensity          14
#define idxTempOutside             15

#define P(str) (strcpy_P(p_buffer, PSTR(str)), p_buffer)

const PROGMEM int MPChanSys[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const PROGMEM float SysTempOffset[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1.66};

// On system Multiplexer 2
const PROGMEM int MPNumSys[] = {2};
// Multiplexer Control Pins
const PROGMEM int MPControl[] = {48, 49, 47, 46};
// Multiplexer Temperature Input Pins A01 = 97; A02 = 96; A03 = 95
const PROGMEM int MPInput[] = {A0, A1, A2};

#define Vcc 5.04  //! Voltage of power supply [V]

// Wrapper function for digital outputs
static void digitalWrite_wrap(uint8_t pin, boolean state)
{
  digitalWrite(pin, state);
}

static void pinMode_wrap(uint8_t pin, uint8_t mode)
{
  pinMode(pin, mode);
}


#endif
/*
Encoding scheme for variable names

Objects

Boiler B
WarmWater W
Burner Bu
Solar S
Source SO
Sink SI
Rooms R
Heating H
Mixer M
Pump P
Valve V
Tempsensor/Temperature T

time t

Postfix
is i
setpoint s
margin m

need n
charge c
sufficientHeat suht

to    to
from  from

*/
