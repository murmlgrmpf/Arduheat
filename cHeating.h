#ifndef cHeating_h
#define cHeating_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cBoiler.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRoom.h"
#include "cFlowMeter.h"
#include "cHeatExchanger.h"
#include "cBurner.h"
#include <PID_v1.h>

class cHeating 
{
  private:
    double _TempHeatingLead;
    double _TempHeatingReturn;
    double _TempHeatSource1Lead;
    double _TempHeatSource1Return;
    double _TempHeatSource1Operation;
    double _TempSolarReturn;
    double _TempSolarLead;
    double _TempWarmWater;
    double _TempCirculationReturn;
    double _IntensitySolar;
    double _TempOutside;
    double _SpTempWarmWater;
    double Input;
    double _needHeating;
    double _needWarmWater;
    
    boolean _chargeWarmWater;
    boolean _chargeHeating;
    
    PID PIDWarmWater;
  
    
  public:
    cRoom Rooms[16];
  
    cPump PumpWarmWater;
//     cPump PumpBoiler;
    cPump PumpSolar;
    cPump PumpHeating;
    cPump PumpCirculation;
    
    cValve ValveWarmWater;
//     cValve ValveBoiler;
    cValve ValveHeatSource1;
    
    cHeatExchanger HxWarmWater;
    
    cFlowMeter FlowMeter;
    cBoiler Boiler;
    cBurner Burner;
    cTemp Temperatures;
//    double SpTempWarmWater;
    
    cHeating(void);
    
    double TempHeatingLead();
    double TempHeatingReturn();
    double TempHeatSource1Lead();
    double TempHeatSource1Return();
    double TempHeatSource1Operation();
    double TempSolarReturn();
    double TempSolarLead();
    double TempWarmWater();
    double TempCirculationReturn();
    double IntensitySolar();
    double TempOutside();
    
    void ControlWarmWater();
    void Control(void);
};

#endif
