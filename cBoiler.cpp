#include "cBoiler.h"


void cBoiler::getSP( JsonObject& root )
{
	root["BoilerChargeMargin"] = ChargeMargin;
}

int cBoiler::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("BoilerChargeMargin")) {
		if(root["BoilerChargeMargin"].is<double>()) {
			ChargeMargin = root["BoilerChargeMargin"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if (fail == 0) // If all three parameter objects were successfully filled
		return posReturn;
	else
		return 0;
}

void cBoiler::getData( JsonObject& root )
{
	root["BoilerValve"] =  static_cast<int>( Valve.get());

	root["BoilerTempCharge"] = TempCharge.get();
	root["BoilerTempTop"] = TempTop.get();
	root["BoilerTempHead"] = TempHead.get();
	root["BoilerTempReserve1"] = TempReserve1.get();
	root["BoilerTempReserve2"] = TempReserve2.get();
	
	root["BoilerPumpBoilerCharge"] =  PumpBoilerCharge.getPower();
	root["BoilerneedChargeWarmWater"] =  static_cast<int>( bneedChargeWarmWater);
	root["BoilerneedChargeHeating"] =  static_cast<int>( bneedChargeHeating);
	root["BoilerbDischarging"] =  static_cast<int>( bDischarging);
	root["BoilerbCharging"] =  static_cast<int>( bCharging);
	root["BoilerSpTempWarmWater"] =  WarmWater->SpTemp();
	root["BoilerSpTempCharge"] =   getSpTempCharge();
}
