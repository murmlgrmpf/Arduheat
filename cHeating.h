#ifndef cHeating_h
#define cHeating_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRoom.h"
#include "cFlowMeter.h"
#include "cHeatExchanger.h"
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
    double _TempBoilerCharge;
    double _TempBoilerReserve1;
    double _TempBoilerReserve2;
    double _TempBoilerHead;
    double _TempBoilerTop;
    double _TempWarmWater;
    double _TempCirculationReturn;
    double _IntensitySolar;
    double _TempOutside;
    
    double Input;
    PID PIDWarmWater;
  
    
  public:
    cRoom Rooms[16];
  
    cPump PumpWarmWater;
    cPump PumpBoiler;
    cPump PumpSolar;
    cPump PumpHeating;
    cPump PumpCirculation;
    
    cValve ValveWarmWater;
    cValve ValveBoiler;
    cValve ValveHeatSource1;
    
    cHeatExchanger HxWarmWater;
    
    cFlowMeter FlowMeter;
    
    cTemp Temperatures;
    
    double SpTempWarmWater;
    
    cHeating(void);
    
    double TempHeatingLead();
    double TempHeatingReturn();
    double TempHeatSource1Lead();
    double TempHeatSource1Return();
    double TempHeatSource1Operation();
    double TempSolarReturn();
    double TempSolarLead();
    double TempBoilerCharge();
    double TempBoilerReserve1();
    double TempBoilerReserve2();
    double TempBoilerHead();
    double TempBoilerTop();
    double TempWarmWater();
    double TempCirculationReturn();
    double IntensitySolar();
    double TempOutside();
    
    void ControlWarmWater();
    void BurnerControl();
};

#endif
