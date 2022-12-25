#ifndef cBoiler_h
#define cBoiler_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cValve.h"
#include "cRooms.h"
#include "cWarmWater.h"
#include <ArduinoJson.h>

// Charge Margin WarmWater for staged SetPoint
#define WMargin  12.0
#define SMargin  4.0
#define TempSufficientWarmWater 49.0

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
		SpTempCharge = 45 + WMargin; // charge WarmWater
	else if (bneedChargeHeating)
		SpTempCharge = (Rooms->SpTemp()); // charge Rooms
	else
		SpTempCharge = TempReserve1.get() + SMargin; // Solar Charge

	return (SpTempCharge);
	}
	
	boolean needChargeWarmWater(void)
	{
	if (WarmWater->Period.get()) {
		// Hysteresis by top and head temperature sensors.
		// If top falls below setpoint: charge. If head gets above setpoint: dont charge.
		if (TempTop.get() < 50)
			bneedChargeWarmWater = true;
		if (TempHead.get() > 50)
			bneedChargeWarmWater = false;
	}
	else
	bneedChargeWarmWater = false;
		
	return bneedChargeWarmWater;
	}
	
	boolean shouldCharge(void){
		if (TempTop.get() <= TempSufficientWarmWater)
			bshouldChargeWarmWater = true;
		else if (TempHead.get() > TempSufficientWarmWater)
			bshouldChargeWarmWater = false;
		return bshouldChargeWarmWater;
	}
	
	boolean charge(boolean mayCharge, float TempHeatSource, boolean bneedSink = false){
		double SpTempCharge = SpTemp();

		boolean need = (needChargeWarmWater() || bneedSink);
		
		// if charging is needed, charging is fixed to true	
		if (!mayCharge)
			bCharging = false;
		else if (need)
			bCharging = true;
		else // Hysteresis for Simultaneous/residual heat charging of boiler
		{
			if ((bCharging == false) && (TempHeatSource > (TempReserve1.get() + 4)))
				bCharging = true;
			if ((bCharging == true) && (TempHeatSource < (TempReserve1.get() + 3)))
				bCharging = false;
		}

		if (bCharging){
			double BoilerPumpMin = 0.0;

			if (needChargeWarmWater()) //starting condition for charging of WarmWater
				BoilerPumpMin = 0.2;
			else //minimal Pump Power as function of Boiler Temp
				BoilerPumpMin = max(0.03,((0.1-0.03)/(80-50)*(TempReserve1.get()-50)+0.03)); 

			Pump.SetOutputLimits(BoilerPumpMin, 1.0);
	
			if (bneedChargeWarmWater) {
				TempHeatSource = TempHeatSource + 45; //Manipulation of TempHeatSource in order to achieve 100% BoilerPump Power
				SpTempCharge = 56;
			}

			// Run Pump
			Pump.run(SpTempCharge, TempHeatSource);
		}
		setValve(); // Open Valve if charge pump running (charging)
		return bCharging;
	}

boolean Hot(void){
		// introduce FlipFlop for Hysteresis of Boiler Discharge
		if ((bHot == false) && (TempReserve1.get() > (Rooms->SpTemp() + 3)))
			bHot = true;
		if ((bHot == true) && (TempReserve1.get() < Rooms->SpTemp()))
			bHot = false;

		return bHot;
	}

boolean RelPool(void){
		// introduce Hysteresis for Change of Pool and Boiler charging
		if (TempHead.get() > 54)
			bRelPool = true;
		if (TempTop.get() < 56)
			bRelPool = false;

	return bRelPool;
	}

	void discharge(boolean bNeedSourceBoiler){
		bDischarging = bNeedSourceBoiler;
		setValve(); // Open Valve if discharging
	}
	
	void setValve(){
		Valve.set(bCharging || bDischarging);
	}
		
	void getData(JsonObject& root);
	
	cValve Valve;
	cPump Pump;
	
	// May become private again (debug)
	cTempSensor TempCharge;
	cTempSensor TempTop;
	cTempSensor TempHead;
	cTempSensor TempReserve1;
	cTempSensor TempReserve2;
	
	private:
	cRooms* Rooms;
	cWarmWater* WarmWater;
	
	double PumpPeriod;
	
	boolean bneedChargeWarmWater;
	boolean bneedChargeHeating;
	boolean bshouldChargeWarmWater;
	boolean bDischarging;
	boolean bCharging;
	boolean bHot;
	boolean bRelPool;
};

#endif
