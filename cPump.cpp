#include "cPump.h"

//#define PinPump 51

cPump::cPump(void)
{
  _PinPump = 0;
  _fMaxMassFlowRate = 1.0;
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  
  cPump::setMassFlowRate(0.0);

}

cPump::cPump(int PinPump, float fMaxMassFlowRate)
{
  _PinPump = PinPump;
  _fMaxMassFlowRate = fMaxMassFlowRate;
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  
  cPump::setMassFlowRate(0.0);
}

cPump::cPump(int PinPump, float fMaxMassFlowRate, float fMassFlowRate)
{
  _PinPump = PinPump;
  _fMaxMassFlowRate = fMaxMassFlowRate;
  //Set the time to switch on and off the Pump
  _LastTimePeriod = millis();
  
  cPump::setMassFlowRate(fMassFlowRate);
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
    digitalWrite(_PinPump, HIGH); // Pump switched off
  }
  // The Switching time got readjusted and the Pump 
  // needs to be switched on again
  else
  {
    digitalWrite(_PinPump, LOW); // Pump switched on
  }
  // TimePeriod is over and new period starts
  if(millis()>=_TimePeriod)
  {
    //Set the next Last Period Time
    _LastTimePeriod = millis();
  }
}

float cPump::getMassFlowRate()
{
  return _Power*_fMaxMassFlowRate;
}

/* Calculates the pwm _Power and then executes the pwm
*/
void cPump::setMassFlowRate(float fMassFlowRate)
{
  //Linear approximation of mass flow rate over high percentage of pwm.
  _Power = fMassFlowRate/_fMaxMassFlowRate;
  
  // Limit _Power 
  if (_Power>1.0)
  {
    _Power = 1.0;
  }
  else if(_Power<0.0)
  {
    _Power = 0.0;
  }
  
  // Execute pwm 
  cPump::run();

}

// May be needed to limit the regulator
float cPump::getMaxMassFlowRate()
{
  return _fMaxMassFlowRate;
}

// Needed for calibration of pwm
void cPump::setMaxMassFlowRate(float fMaxMassFlowRate)
{
  _fMaxMassFlowRate = fMaxMassFlowRate;
}

void cPump::setPinPump(int PinPump)
{
  _LastTimePeriod = millis();
  _PinPump = PinPump;
  cPump::run();

}

void cPump::calcPeriod()
{
  //Default _Period = 10 sec = 10000ms
  _Period = 10000;
}
