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
    cValve Valve;
    PID PIDBoilerCharge;
	
	int _PinPump;
    double _SpTempBoilerCharge;
    double _TempBoilerCharge;
	double _SpTempHeatingLead;
    
  public:
    // May become private again (debug)
  	cTempSingle TempBoilerCharge;
  	cTempSingle TempBoilerReserve1;
  	cTempSingle TempBoilerReserve2;
  	cTempSingle TempBoilerHead;
  	cTempSingle TempBoilerTop;
	cPump PumpBoilerCharge;

    /// Creates a Boiler object
    cBoiler(void);
	
    void charge(boolean, double);
    double SpTempChargeWarmWater(void);
    double SpTempChargeHeating(void);
	void setSpTempHeatingLead(double SpTempHeatingLead);
    /// Creates a pump object with a certain pin.
    /** \param PinPump the pin used to drive the Pump*/
    /** \param fMaxMassFlowRate the maximum mass flow rate of the pump*/
    double needHeating(void);
    double haveHeating(void);
    double needWarmWater(void);
    double haveWarmWater(void);

};

#endif
