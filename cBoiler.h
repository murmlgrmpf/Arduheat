#ifndef cBoiler_h
#define cBoiler_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRoom.h"
#include "cWarmWater.h"
#include <cPID.h>
#include <ArduinoJson.h>

class cBoiler
{
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
	cBoiler(cRooms* _Rooms, cWarmWater* _WarmWater);
	
	void charge(boolean);

	void setSpTempHeatingLead(double SpTempHeatingLead);
	/// Creates a pump object with a certain pin.
	/** \param PinPump the pin used to drive the Pump*/
	/** \param fMaxMassFlowRate the maximum mass flow rate of the pump*/
	void discharge( boolean bNeedSourceBoiler ){_bDischarging = bNeedSourceBoiler;	setValve();};
	double getSpTempCharge(void);
	boolean needChargeWarmWater(void);
	boolean needChargeHeating(boolean needHeating);
	void triggerChargeWarmWater() {_needChargeWarmWater = true;};
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	
	private:
	cRooms* Rooms;
	cWarmWater* WarmWater;
	
	double _ChargeHeatingAddon;
	double _ChargeWarmWaterAddon;
	cPID PIDBoilerCharge;
	
	int _PinPump;
	
	boolean _needChargeWarmWater;
	boolean _needChargeHeating;
	boolean _bDischarging;
	boolean _bCharging;
	double _SpTempCharge;
	
	void setValve(void){Valve.set((_bDischarging || _bCharging));};
};

#endif
