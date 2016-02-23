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
#define WMargin  12.0  // 13
#define HMargin  4.0

class cBoiler
{
	public:
	/// Creates a Boiler object
	cBoiler(cRooms* Rooms_,cWarmWater* WarmWater_);
	
	/// SpTemp is the minimum temperature that is needed to charge the boiler.
	double SpTemp(void)
	{
		double SpTempCharge = 0.0;

    if (needChargeWarmWater())
      SpTempCharge = WarmWater->SpTemp + WMargin;
    else if (bneedChargeHeating)
      SpTempCharge = Rooms->getSpHeating() + HMargin;
    else {
      if (Rooms->HeatingPeriod.get())
        // If heating period: Charge floating for room heating
        SpTempCharge = TempReserve1.get()+HMargin;
      else
        // If not heating period: charge only (floating) for warm water
        SpTempCharge = max(WarmWater->SpTemp, TempReserve2.get()) + WMargin;
    }
    
		return (SpTempCharge);
	}
	
	boolean needChargeWarmWater(void)
	{
    if (WarmWater->Period.get()) {
  		// Hysteresis by top and head temperature sensors.
  		// If top falls below setpoint: charge. If head gets above setpoint: dont charge.
  		if (WarmWater->SpTemp+HMargin > TempTop.get())  bneedChargeWarmWater = true;
  		if (WarmWater->SpTemp+HMargin+4 < TempHead.get()) bneedChargeWarmWater = false;
      }
    else
      bneedChargeWarmWater = false;
		
		return bneedChargeWarmWater;
	}
	
	boolean needChargeHeating(boolean needHeating)
	{
		// Hysteresis by TempReserve1 and 2 or no need for heating the rooms.
		// If TempReserve1 falls below setpoint and heating of the rooms is needed: charge.
		// If TempReserve2 comes above setpoint or heating of the rooms not needed: dont charge.
		if ((Rooms->getSpHeating() > TempReserve1.get()) && needHeating)  bneedChargeHeating = true;
		if ((Rooms->getSpHeating() < TempReserve2.get()) || !needHeating) bneedChargeHeating = false;
    
    // Trigger charge warm water
    // if (bneedChargeHeating)
    //     bneedChargeWarmWater = true;
    
		return bneedChargeHeating;
	}
	
	void charge(float TempHeatSource, boolean bneedSink = false)
	{
    double SpTempCharge = SpTemp();
    
		boolean need = (needChargeWarmWater() || bneedChargeHeating || bneedSink);
		
		if (need) // if charging is needed, charging is fixed to true
			bCharging = true;
		else // Hysteresis for Simultaneous charging of boiler
		{
			if (SpTempCharge+2 < TempHeatSource)
				bCharging = true;
			if (SpTempCharge-2 > TempHeatSource)
				bCharging = false;
		}
		
		if (bCharging) // Run Pump
			Pump.run(SpTempCharge, TempHeatSource);
		else // Stop Charging: Stop PID and Pump
			Pump.run();
                
    // Open Valve if charge pump running (charging) or discharging
    setValve();
	}
	
	void discharge( boolean bNeedSourceBoiler )
	{
		bDischarging = bNeedSourceBoiler;
		setValve(); // Open Valve if charging or discharging
	}
	
	void setValve() {Valve.set(bDischarging || (Pump.get()>0.0));};
	
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
