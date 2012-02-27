#include "cPump.h"

//#define PinPump 51

cPump::cPump(void)
{
  _Power =0;
  _PinPump = 0;
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  //cPump::set(_Power);
}

cPump::cPump(int PinPump)
{
  _Power =0;
  _PinPump = PinPump;
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  cPump::set(_Power);
}

cPump::cPump(int PinPump, float Value)
{
  _Power = Value;
  _PinPump = PinPump;
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  cPump::set(Value);
}

/* Switches the Pump on and off corresponding to the power level
   After each Period the length of the period gets adjusted.
*/
void cPump::run(void)
{
  // Calculate the Period
  cPump::calcPeriod();
  // Set the end time of the current Period
  _TimePeriod = round(_LastTimePeriod + _Period);
  // Set the next switching Time
  _TimeSwitch = round(_LastTimePeriod + _Power*_Period);
  // TimeSwitch reached
  if(millis()>=_TimeSwitch)
  {
    //digitalWrite(PinPumpWarmWater, HIGH); // Pump switched off
    digitalWrite(_PinPump, HIGH); // Pump switched off
  }
  // The Switching time got readjusted and the Pump 
  // needs to be switched on again
  else
  {
    //digitalWrite(PinPumpWarmWater, LOW); // Pump switched off
    digitalWrite(_PinPump, LOW); // Pump switched on
  }
  // TimePeriod is over and new period starts
  if(millis()>=_TimePeriod)
  {
    //Set the next Last Period Time
    _LastTimePeriod = millis();
  }
}

float cPump::get()
{
  return _Power;
}

void cPump::set(float Value)
{
  //this->_Value = Value;
  _Power = Value;  
  cPump::run();

}

void cPump::setPinPump(int PinPump)
{
  //this->_Value = Value;
  _LastTimePeriod = millis();
  _PinPump = PinPump;
  cPump::run();

}

void cPump::calcPeriod()
{
  //Default _Period = 10 sec = 10000ms
  _Period = 10000;
}
