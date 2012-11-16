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
 
 // TempHeatControl Initialization
  pinMode(TempHeatControl, INPUT);

  // Motor Sense Initialization
  pinMode(PinValveWarmWaterMotorSense, INPUT);
  pinMode(PinValveBoilerMotorSense, INPUT);
  pinMode(PinValveSolarMotorSense, INPUT);
  pinMode(PinValveHeatSource1MotorSense, INPUT);

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
  pinMode(PinPulseCounter, INPUT);
  pinMode(PinHeatControl, OUTPUT);

  digitalWrite(PinPumpCirculation, HIGH);
  digitalWrite(PinStartHeatSource1, HIGH);
  digitalWrite(PinPumpWarmWater, HIGH);
  digitalWrite(PinPumpBoiler, HIGH);
  digitalWrite(PinPumpSolar, HIGH);
  digitalWrite(PinPumpHeating, HIGH);
  digitalWrite(PinMixerOpen, HIGH);
  digitalWrite(PinMixerClose, HIGH);
  //digitalWrite(PinPulseCounter, HIGH);

  // Room Heating intialization  
  pinMode(Room1, OUTPUT);
  pinMode(Room2, OUTPUT);
  pinMode(Room3, OUTPUT);
  pinMode(Room4, OUTPUT);
  pinMode(Room5, OUTPUT);
  pinMode(Room6, OUTPUT);
  pinMode(Room7, OUTPUT);
  pinMode(Room8, OUTPUT);
  pinMode(Room9, OUTPUT);
  pinMode(Room10, OUTPUT);
  pinMode(Room11, OUTPUT);
  pinMode(Room12, OUTPUT);
  pinMode(Room13, OUTPUT);
  pinMode(Room14, OUTPUT);
  pinMode(Room15, OUTPUT);
  pinMode(Room16, OUTPUT);

  digitalWrite(Room1, HIGH);
  digitalWrite(Room2, HIGH);
  digitalWrite(Room3, HIGH);
  digitalWrite(Room4, HIGH);
  digitalWrite(Room5, HIGH);
  digitalWrite(Room6, HIGH);
  digitalWrite(Room7, HIGH);
  digitalWrite(Room8, HIGH);
  digitalWrite(Room9, HIGH);
  digitalWrite(Room10, HIGH);
  digitalWrite(Room11, HIGH);
  digitalWrite(Room12, HIGH);
  digitalWrite(Room13, HIGH);
  digitalWrite(Room14, HIGH);
  digitalWrite(Room15, HIGH);
  digitalWrite(Room16, HIGH);


}
