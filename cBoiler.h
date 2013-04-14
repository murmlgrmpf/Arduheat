#ifndef cBoiler_h
#define cBoiler_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include <PID_v1.h>

class cBoiler
{
  private:
    int _PinPump;
    
    /// Execute the pump. 
    /** Each time, it is checked whether the period is over or whether the switch time is reached. 
        If the TimeSwitch is reached, the pump is switched off. When the TimePeriod is reached, the pump is switched on 
        and the new TimePeriod, TimeSwitch and LastTimePeriod get set. */ 
    void run(void);
    /// Calculate the Period
    /** A more elaborate way to calculate the Period of the "slow" PWM could be implemented in order
    to prevent the pump from being switched on and off for only short times in some race conditions.*/
    void calcPeriod(void);
    cTemp Temperatures;
    cPump PumpBoilerCharge;
    cValve Valve;
    PID PIDBoilerCharge;
  
    double _SpTempBoilerCharge;
    double _TempBoilerCharge;
    double _TempBoilerReserve1;
    double _TempBoilerReserve2;
    double _TempBoilerHead;
    double _TempBoilerTop;
    double _TempChargeWarmWater;
    double _TempChargeHeating;
    boolean _needHeating;
    double _needWarmWater;

    
  public:
    double TempBoilerCharge();
    double TempBoilerReserve1();
    double TempBoilerReserve2();
    double TempBoilerHead();
    double TempBoilerTop();
    double TempChargeWarmWater(void);
    double TempChargeHeating(void);
    float _Power;
    double Power;
    /// Creates a Boiler object
    cBoiler(void);
    /// Creates a pump object with a certain pin.
    /** \param PinPump the pin used to drive the Pump*/
    /** \param fMaxMassFlowRate the maximum mass flow rate of the pump*/
    double needHeating(void);
    double haveHeating(void);
    double needWarmWater(void);
    double haveWarmWater(void);
    void charge(double);
    void stop(void);
};

#endif