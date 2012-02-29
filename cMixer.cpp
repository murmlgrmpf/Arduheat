#include "cMixer.h"


cMixer::cMixer(void)
{
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  cMixer::set(0);
}

cMixer::cMixer(float Power)
{
  //Set the time to switch on and off the Mixer
  _LastTimePeriod = millis();
  cMixer::set(Power);
}

/* Switches the Pump on and off corresponding to the power level
   After each Period the length of the period gets adjusted.
*/
void cMixer::run(void)
{
  // Calculate the Period
  cMixer::calcPeriod();
  // Set the end time of the current Period
  _TimePeriod = round(_LastTimePeriod + _Period);
  // Set the next switching Time
  _TimeSwitch = round(_LastTimePeriod + _Power*_Period);
  // TimeSwitch reached
  
  if (_Power>=0)
  {
    digitalWrite(PinMixerClose, HIGH); // Do not drive in negative direction
    
    if(millis()>=_TimeSwitch)
    {
      digitalWrite(PinMixerOpen, LOW); // Pump switched off
    }
    // The Switching time got readjusted and the Pump 
    // needs to be switched on again
    else
    {
      digitalWrite(PinMixerOpen, HIGH); // Pump switched on
    }
    // TimePeriod is over and new period starts
    if(millis()>=_TimePeriod)
    {
      //Set the next Last Period Time
      _LastTimePeriod = millis();
    }
  }
  else
  {
    digitalWrite(PinMixerOpen, HIGH); // Do not drive in negative direction
    
    if(millis()>=_TimeSwitch)
    {
      digitalWrite(PinMixerClose, LOW); // Pump switched off
    }
    // The Switching time got readjusted and the Pump 
    // needs to be switched on again
    else
    {
      digitalWrite(PinMixerClose, HIGH); // Pump switched on
    }
    // TimePeriod is over and new period starts
    if(millis()>=_TimePeriod)
    {
      //Set the next Last Period Time
      _LastTimePeriod = millis();
    }
  }
}

float cMixer::get()
{
  return _Power;
}

void cMixer::set(float Power)
{
  //this->_Value = Value;
  _Power = Power;  
  cMixer::run();

}

void cMixer::calcPeriod()
{
  //Default _Period = 10 sec = 10000ms
  _Period = 10000;
}
