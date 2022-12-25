#ifndef PinDefinitions_h
#define PinDefinitions_h

// Pin mapping Arduino Board Version V8.7:
/*
#define PinPumpCirculation           17
#define PinPumpWarmWater             18
#define PinPumpBoiler                16
#define PinPumpSolar                 23
#define PinPumpHeating               22
#define PinMixerOpen                 14
#define PinMixerClose                25
#define PinPoolSwitch                47 //Pin 2 on Connector 4x Control
#define PinFeedPoolSwitch            49 //Pin 4 on Connector 4x Control
//#define PinReserve                   51 //Pin 6 on Connector 4x Control
//#define PinReserve                   53 //Pin 8 on Connector 4x Control
#define PinSolarIntensity            A3
#define PinWarmWaterFlow             A4
#define PinFlowRegulator              3
#define PinValveTransferStation      15
#define PinValveBoiler                2
#define PinValveSolar                24
#define PinValvePool                 44 //Pin 10 on Connector 3
//#define PinReserve                      37 //Pin 9 on Connector 3
//#define PinReserve                      45 //Pin 8 on Connector 3
//#define PinReserve                      36 //Pin 7 on Connector 3
//#define PinPumpWarmWaterPWM         7 //Future Eco Pumps
//#define PinPumpBoilerPWM            6 //Future Eco Pumps
//#define PinPumpHeatingPWM           5 //Future Eco Pumps
//#define PinPumpSolarPWM             4 //Future Eco Pumps
*/

// Pin mapping Arduino Board Version V8.6:

#define PinPumpCirculation           17
#define PinPumpWarmWater             18
#define PinPumpBoiler                16
#define PinPumpSolar                 23
#define PinPumpHeating               22
#define PinMixerOpen                 14
#define PinMixerClose                25
#define PinPoolSwitch                47 //Pin 2 on Connector 4x Control
#define PinFeedPoolSwitch            49 //Pin 4 on Connector 4x Control
//#define PinReserve                   51 //Pin 6 on Connector 4x Control
//#define PinReserve                   53 //Pin 8 on Connector 4x Control
#define PinSolarIntensity            A3
#define PinWarmWaterFlow             A4
#define PinFlowRegulator              3
#define PinValveTransferStation      15
#define PinValveBoiler                2
#define PinValveSolar                24
#define PinValvePool                 42 // Pin 10 on Connector 3
//#define PinReserve                      43 //Pin 9 on Connector 3
//#define PinReserve                      45 //Pin 8 on Connector 3
//#define PinReserve                      44 //Pin 7 on Connector 3
//#define PinPumpWarmWaterPWM         7 //Future Eco Pumps
//#define PinPumpBoilerPWM            6 //Future Eco Pumps
//#define PinPumpHeatingPWM           5 //Future Eco Pumps
//#define PinPumpSolarPWM             4 //Future Eco Pumps

// Pin mapping Arduino Board Version V8.4:
/*
#define PinPumpCirculation           16
#define PinPumpWarmWater             17
#define PinPumpBoiler                15
#define PinPumpSolar                 18
#define PinPumpHeating               23
#define PinMixerOpen                  2
#define PinMixerClose                22
#define PinPoolSwitch                45 //Pin 2 on Connector 4x Control
#define PinFeedPoolSwitch            43 //Pin 4 on Connector 4x Control
//#define PinReserve                   47 //Pin 6 on Connector 4x Control
//#define PinReserve                   49 //Pin 8 on Connector 4x Control
#define PinSolarIntensity            A3
#define PinWarmWaterFlow             A4
#define PinFlowRegulator              6
#define PinValveTransferStation      14
#define PinValveBoiler                3
#define PinValveSolar                 5
#define PinValvePool                 50 //Pin 10 on Connector 3
//#define PinReserve                      41 //Pin 9 on Connector 3
//#define PinReserve                      51 //Pin 8 on Connector 3
//#define PinReserve                      40 //Pin 7 on Connector 3
*/
// Pin mapping Arduino Board Version V8:
/*
#define PinPumpCirculation            6
#define PinValveBoilerOpen           39
#define PinValveSolarClose           42
#define PinValveSolarOpen            43
#define PinValveTransferStationClose 44
#define PinValveTransferStationOpen  45
#define PinStartHeatSource1           8
#define PinValveBoilerClose          38
#define PinPumpWarmWater              5
#define PinPumpBoiler                 7
#define PinPumpSolar                 14
#define PinPumpHeating               15
#define PinMixerOpen                  9
#define PinMixerClose                16
#define PinWarmWaterSwitch           19
#define PinSolarIntensity	         A3
#define PinWarmWaterFlow	         A4
#define PinFlowRegulator	         12
*/
//System Temperatures
#define idxTempHeatingReturn                   0
#define idxTempHeatingLead                     1
#define idxTempSolarFromCollector              3
#define idxTempWarmWaterFromBoiler             2
#define idxTempSolarToSystem                   6
#define idxTempBoilerCharge                    7
#define idxTempBoilerReserve2                  8
#define idxTempBoilerReserve1                  9
#define idxTempBoilerHead                     10
#define idxTempBoilerTop                      11
#define idxTempWarmWater                      12
#define idxTempWaterFromOutside                5
#define idxTempOutside                        15
#define idxTempTransferStationFromTeleheating 13
#define idxTempPool                           14
#define idxTempTransferStationOperation        4

#define P(str) (strcpy_P(p_buffer, PSTR(str)), p_buffer)

const PROGMEM int MPChanSys[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const PROGMEM float SysTempOffset[] = {-1.4,-1.2,-0.1,-0.47,-0.6,-0.4,-0.6,-0.9,0,0,0,0,-1.65,0.0,-0.6,-0.76}; //InputBoard1
//const PROGMEM float SysTempOffset[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1.66}; //InputBoard1_old

// On system Multiplexer 2
const PROGMEM int MPNumSys[] = {2};

	const PROGMEM int MPControl[] = {48, 50, 52, 46}; //Board_V8.6, 8.7
//	const PROGMEM int MPControl[] = {44, 46, 48, 42}; //Board_V8.4
//	const PROGMEM int MPControl[] = {48, 49, 47, 46}; //Board_V8

// Multiplexer Temperature Input Pins A01 = 97; A02 = 96; A03 = 95
const PROGMEM int MPInput[] = {A0, A1, A2};

#define Vcc 5.0 //! Voltage of power supply [V]

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
