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

// Charge Margins Warmwater and Heating
#define WMargin  13.0  // 6
#define HMargin   4.0

#define HeatingPeriodHorizon  1000*60*60*36

class cBoiler
{
	public:
	/// Creates a Boiler object
	cBoiler(cRooms* Rooms_,cWarmWater* WarmWater_);
	
	/// SpTemp is the minimum temperature that is needed to charge the boiler.
	double SpTemp(void)
	{
		double SpTempCharge = TempReserve1.get()+HMargin;
		
		if (bneedChargeHeating)
			SpTempCharge = Rooms->getSpHeating() + HMargin;
		if (needChargeWarmWater())
			SpTempCharge = WarmWater->SpTemp + WMargin;
		
		return (SpTempCharge);
	}
	
	boolean needChargeWarmWater(void)
	{
		// Hysteresis by top and head temperature sensors.
		// If top falls below setpoint: charge. If head gets above setpoint: dont charge.
		if (WarmWater->SpTemp+HMargin > TempTop.get())  bneedChargeWarmWater = true;
		if (WarmWater->SpTemp+HMargin < TempHead.get()) bneedChargeWarmWater = false;
		
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
	
	void charge(float TempHeatSource, boolean bneedSink = false)
	{
		boolean need = (bneedChargeWarmWater || bneedChargeHeating || bneedSink);
		
		if (need) // if charging is needed, charging is fixed to true
			bCharging = true;
		else // Hysteresis
		{
			if (SpTemp() < TempHeatSource)
				bCharging = true;
			if (SpTemp()-2 > TempHeatSource)
				bCharging = false;
		}
		
		Valve.set((bDischarging || bCharging)); // Open Valve if charging or discharging
		
		double SpTempCharge = 0.0;
		boolean HeatingPeriod = (millis()- Rooms->lastHeating < HeatingPeriodHorizon);
		
		if (HeatingPeriod)
			SpTempCharge = max(SpTemp(), Rooms->getSpHeating()+1.5*HMargin);
		else
		{
			SpTempCharge = max(SpTemp(), WarmWater->SpTemp + WMargin);
			Rooms->lastHeating = millis()-HeatingPeriodHorizon; // Rolling horizon for overflow of millis
		}
		
		if (bCharging) // Run Pump
			Pump.run(SpTempCharge, TempHeatSource);
		else // Stop Charging: Stop PID and Pump
			Pump.run();
	}
	
	void discharge( boolean bNeedSourceBoiler )
	{
		bDischarging = bNeedSourceBoiler;
		Valve.set((bDischarging || bCharging)); // Open Valve if charging or discharging
	}
	
	void triggerChargeWarmWater() {bneedChargeWarmWater = true;};
	
	void getData(JsonObject& root);
	
	cValve Valve;
	cPump Pump;
	
	// May become private again (debug)
	cTempSensor TempCharge;
	cTempSensor TempTop;
	cTempSensor TempHead;
	cTempSensor TempReserve2;
	cTempSensor TempReserve1;
	
	private:
	cRooms* Rooms;
	cWarmWater* WarmWater;
	
	boolean bneedChargeWarmWater;
	boolean bneedChargeHeating;
	boolean bDischarging;
	boolean bCharging;
};

#endif