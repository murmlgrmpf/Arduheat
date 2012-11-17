#ifndef cHeating_h
#define cHeating_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "Temperatures.h"
#include "cPump.h"
#include "cValve.h"
#include "cRoom.h"
#include "cFlowMeter.h"

class cHeating 
{
  private:
    float TempHeatingLeadFilter;
    float TempHeatingReturnFilter;
    float TempHeatSource1LeadFilter;
    float TempHeatSource1ReturnFilter;
    float TempHeatSource1OperationFilter;
    float TempSolarReturnFilter;
    float TempSolarLeadFilter;
    float TempBoilerChargeFilter;
    float TempBoilerReserve1Filter;
    float TempBoilerReserve2Filter;
    float TempBoilerHeadFilter;
    float TempBoilerTopFilter;
    float TempWarmWaterFilter;
    float TempCirculationReturnFilter;
    float IntensitySolarFilter;
    float TempOutsideFilter;
  
    
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
    
    cFlowMeter FlowMeter;
    
    float SetpointTempWarmWater;
    
    cHeating(void);
    
    float TempHeatingLead();
    float TempHeatingReturn();
    float TempHeatSource1Lead();
    float TempHeatSource1Return();
    float TempHeatSource1Operation();
    float TempSolarReturn();
    float TempSolarLead();
    float TempBoilerCharge();
    float TempBoilerReserve1();
    float TempBoilerReserve2();
    float TempBoilerHead();
    float TempBoilerTop();
    float TempWarmWater();
    float TempCirculationReturn();
    float IntensitySolar();
    float TempOutside();
};

#endif
