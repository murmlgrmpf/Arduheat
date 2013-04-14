#include "cBoiler.h"


cBoiler::cBoiler(void)
:Valve(PinValveBoilerOpen,PinValveBoilerClose),
PIDBoilerCharge(&_TempBoilerCharge, &PumpBoilerCharge.Power, &_SpTempBoilerCharge, 0.01, 0.001, 0.05, DIRECT)
{
  // Set minimal Pump Power to about 10%
  PIDBoilerCharge.SetOutputLimits(0.1, 1);
  
  Temperatures;
  PumpBoilerCharge.setPinPump(PinPumpBoiler);
  PumpBoilerCharge.setMaxMassFlowRate(PumpBoilerChargeMaxMassFlowRate);
  _needHeating =false;
}


double cBoiler::TempChargeWarmWater(void)
{
  _TempChargeWarmWater = SpTempWarmWater+2;
  
  return _TempChargeWarmWater;
}


double cBoiler::TempChargeHeating(void)
{
  _TempChargeHeating = max(SpTempHeatingLead+2, TempBoilerReserve2()+4);
  
  return _TempChargeHeating;
}


void cBoiler::charge(double SpCharge)
{
  // Open Charge valve
  Valve.set(true);
  
  // Set setpoint for PID Controller
  _SpTempBoilerCharge = SpCharge;

  // Run PID for Pump
  PIDBoilerCharge.SetMode(AUTOMATIC);
  PIDBoilerCharge.Compute();
  
  // Run Pump
  PumpBoilerCharge.setMassFlowRate(PumpBoilerCharge.Power*PumpBoilerCharge.getMaxMassFlowRate());
}


void cBoiler::stop(void)
{
  // Stop PID
  PIDBoilerCharge.SetMode(MANUAL);
  
  // Stop Pump
  PumpBoilerCharge.setMassFlowRate(0.0);
  
  // Close Charge valve
  Valve.set(false);
  
}


/* Switches the Pump on and off corresponding to the power level
   After each Period the length of the period gets adjusted.
*/
double cBoiler::needHeating(void)
{
  // need = Setpoint - Actual Value
  _needHeating = SpTempHeatingLead -TempBoilerReserve2();
  
  return _needHeating;
}


double cBoiler::haveHeating(void)
{
  // need = Setpoint - Actual Value
  double have = -(SpTempHeatingLead - TempBoilerReserve2());
  
  return have;
}


double cBoiler::needWarmWater(void)
{
  // need = Setpoint - Actual Value
  _needWarmWater = SpTempWarmWater - TempBoilerTop();
  
  return _needWarmWater;
}


double cBoiler::haveWarmWater(void)
{
  // need = Setpoint - Actual Value
  double have = -(SpTempWarmWater - TempBoilerHead());
  
  return have;
}


double cBoiler::TempBoilerCharge(){
    return (OffsetTempBoilerCharge + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerCharge));}
double cBoiler::TempBoilerReserve1(){
    return (OffsetTempBoilerReserve1 + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerReserve1));}
double cBoiler::TempBoilerReserve2(){
    return (OffsetTempBoilerReserve2 + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerReserve2));}
double cBoiler::TempBoilerHead(){
    return (OffsetTempBoilerHead + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerHead));}
double cBoiler::TempBoilerTop(){
    return (OffsetTempBoilerTop + Temperatures.getTemp(SystempMultiplexer,MultiplexTempBoilerTop));}