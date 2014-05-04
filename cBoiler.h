#ifndef cBoiler_h
#define cBoiler_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cPID.h"

class cBoiler
{
	private:

	cPID PIDBoilerCharge;
	
	int _PinPump;
	
	boolean _needChargeWarmWater;
	boolean _needChargeHeating;
	boolean _bDischarging;
	boolean _bCharging;
	double _SpTempHeatingLead;
	double _SpTempCharge;
	
	void setValve(void);
	
	public:
	cValve Valve;
	// May become private again (debug)
	cTempSensor TempCharge;
	cTempSensor TempTop;
	cTempSensor TempHead;
	cTempSensor TempReserve2;
	cTempSensor TempReserve1;
	
	cPump PumpBoilerCharge;

	/// Creates a Boiler object
	cBoiler(void);
	
	void charge(boolean);
	double SpTempChargeWarmWater(void);
	double SpTempChargeHeating(void);
	void setSpTempHeatingLead(double SpTempHeatingLead);
	/// Creates a pump object with a certain pin.
	/** \param PinPump the pin used to drive the Pump*/
	/** \param fMaxMassFlowRate the maximum mass flow rate of the pump*/
	boolean HeatingEmpty(void);
	boolean HeatingFull(void);
	double WarmWaterEmpty(void);
	double WarmWaterFull(void);
	void discharge( boolean bNeedSourceBoiler );
	boolean needChargeWarmWater(void);
	double getSpTempCharge(void);
	boolean needChargeHeating(boolean needHeating);
	boolean needCharge( boolean _bneedHeating );
	void triggerChargeWarmWater();
	
};

#endif
