#include "PinDefinitions.h"

void PinInitialization(void)
{
  // Multiplexer Initialization
  pinMode(MultiplexInput1, INPUT);
  pinMode(MultiplexInput2, INPUT);
  pinMode(MultiplexInput3, INPUT);
  pinMode(MultiplexControl1, OUTPUT);
  pinMode(MultiplexControl2, OUTPUT);
  pinMode(MultiplexControl3, OUTPUT);
  pinMode(MultiplexControl4, OUTPUT);

 // Pumps and Valves intialization
  pinMode(PinPumpCirculation, OUTPUT);
  pinMode(PinValveWarmWaterOpen, OUTPUT);
  pinMode(PinValveBoilerOpen, OUTPUT);
  pinMode(PinValveSolarOpen, OUTPUT);
  pinMode(PinValveHeatSource1Open, OUTPUT);
  pinMode(PinStartHeatSource1, OUTPUT);
  pinMode(PinValveWarmWaterClose, OUTPUT);
  pinMode(PinValveBoilerClose, OUTPUT);
  pinMode(PinValveSolarClose, OUTPUT);
  pinMode(PinValveHeatSource1Close, OUTPUT);
  pinMode(PinPumpWarmWater, OUTPUT);
  pinMode(PinPumpBoiler, OUTPUT);
  pinMode(PinPumpSolar, OUTPUT);
  pinMode(PinPumpHeating, OUTPUT);
  pinMode(PinMixerOpen, OUTPUT);
  pinMode(PinMixerClose, OUTPUT);
  pinMode(PinWarmWaterSwitch, INPUT);
  pinMode(PinHeatControl, OUTPUT);

  digitalWrite(PinPumpCirculation, HIGH);
  digitalWrite(PinStartHeatSource1, HIGH);
  digitalWrite(PinPumpWarmWater, HIGH);
  digitalWrite(PinPumpBoiler, HIGH);
  digitalWrite(PinPumpSolar, HIGH);
  digitalWrite(PinPumpHeating, HIGH);
  digitalWrite(PinMixerOpen, HIGH);
  digitalWrite(PinMixerClose, HIGH);

  // Room Heating intialization
  for (int i=0; i<16; i++)
  {
	  pinMode(RoomValvePin[i], OUTPUT);
	  digitalWrite(RoomValvePin[i], HIGH);
  }
}
