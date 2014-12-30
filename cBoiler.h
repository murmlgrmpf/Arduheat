#ifndef cBoiler_h
#define cBoiler_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRooms.h"
#include "cWarmWater.h"
#include <cPID.h>
#include <ArduinoJson.h>

// Setpoint System Temperatures
#define DefaultChargeMargin	4.0

class cBoiler
{
	public:
	/// Creates a Boiler object
	cBoiler(cRooms* Rooms_,cWarmWater* WarmWater_)
	:Valve(PinValveBoilerOpen,PinValveBoilerClose),
	PIDBoilerCharge( 0.2, 0.1, 0.05, REVERSE),
	TempCharge(SystemMultiplexer,MultiplexTempBoilerCharge,OffsetTempBoilerCharge),
	TempReserve1(SystemMultiplexer,MultiplexTempBoilerReserve1,OffsetTempBoilerReserve1),
	TempReserve2(SystemMultiplexer,MultiplexTempBoilerReserve2,OffsetTempBoilerReserve2),
	TempHead(SystemMultiplexer,MultiplexTempBoilerHead,OffsetTempBoilerHead),
	TempTop(SystemMultiplexer,MultiplexTempBoilerTop,OffsetTempBoilerTop),
	PumpBoilerCharge(PinPumpBoiler)
	{
		Rooms = Rooms_;
		WarmWater = WarmWater_;
		ChargeMargin = DefaultChargeMargin;
		// Set minimal Pump Power to 10%
		PIDBoilerCharge.SetOutputLimits(0.1, 1);
	}
	
	double getSpTempCharge(void)
	{
		// SpTempCharge is the maximum of what the rooms need and what is inside the boiler.
		// In case the boiler is full (e.g. because solar is available, Rooms dont need heat), 
		// it will continously overcharge the boiler.
		double SpTempCharge = max(Rooms->getSpHeating(), TempReserve1.get());
		
		// If warm water needs to be charged, choose maximum charging temperature.
		if (needChargeWarmWater())
			SpTempCharge = max(WarmWater->SpTemp(), SpTempCharge);
		
		// Add a margin such that the setpoint for charging is higher than what is needed.
		return (SpTempCharge+ChargeMargin);
	}
	
	boolean needChargeWarmWater(void)
	{
		// Hysteresis by top and head temperature sensors.
		// If top falls below setpoint: charge. If head gets above setpoint: dont charge.
		if (WarmWater->SpTemp() > TempTop.get())  bneedChargeWarmWater = true;
		if (WarmWater->SpTemp() < TempHead.get()) bneedChargeWarmWater = false;
		
		return bneedChargeWarmWater;
	}
	
	boolean needChargeHeating(boolean needHeating)
	{
		// Hysteresis by TempReserve1 and 2 or no need for heating the rooms.
		// If TempReserve1 falls below setpoint and heating of the rooms is needed: charge.
		// If TempReserve2 comes above setpoint or heating of the rooms not needed: dont charge.
		if ((Rooms->getSpHeating() > TempReserve1.get()) && needHeating)  bneedChargeHeating = true;
		if ((Rooms->getSpHeating() < TempReserve2.get()) || !needHeating) bneedChargeHeating = false;
		
		return bneedChargeHeating;
	}
	
	void charge(boolean bCharge)
	{
		bCharging = bCharge;
		
		Valve.set((bDischarging || bCharging)); // Open Valve if charging or discharging
		
		if (bCharge) // Run Pump
			PumpBoilerCharge.setPower(PIDBoilerCharge.run(getSpTempCharge(), TempCharge.get()));
		else // Stop Charging: Stop PID and Pump
			PumpBoilerCharge.setPower(PIDBoilerCharge.run());
	}
	
	void discharge( boolean bNeedSourceBoiler )
	{
		bDischarging = bNeedSourceBoiler;
		Valve.set((bDischarging || bCharging));
	}
	
	void triggerChargeWarmWater() {bneedChargeWarmWater = true;};
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	
	cValve Valve;
	cPump PumpBoilerCharge;
	cPID PIDBoilerCharge;
	
	// May become private again (debug)
	cTempSensor TempCharge;
	cTempSensor TempTop;
	cTempSensor TempHead;
	cTempSensor TempReserve2;
	cTempSensor TempReserve1;
	
	private:
	cRooms* Rooms;
	cWarmWater* WarmWater;
	
	double ChargeMargin;
	
	boolean bneedChargeWarmWater;
	boolean bneedChargeHeating;
	boolean bDischarging;
	boolean bCharging;
};

#endif
