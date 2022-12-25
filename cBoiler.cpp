#include "cBoiler.h"

cBoiler::cBoiler(cRooms* Rooms_,cWarmWater* WarmWater_):
Valve(PinValveBoiler),
Pump(PinPumpBoiler, 0.025, 0.0, 0.0, REVERSE),
TempCharge((&MPNumSys[0]),(&MPChanSys[idxTempBoilerCharge]),(&SysTempOffset[idxTempBoilerCharge])),
TempTop((&MPNumSys[0]),&MPChanSys[idxTempBoilerTop],(&SysTempOffset[idxTempBoilerTop])),
TempHead((&MPNumSys[0]),&MPChanSys[idxTempBoilerHead],(&SysTempOffset[idxTempBoilerHead])),
TempReserve1((&MPNumSys[0]),(&MPChanSys[idxTempBoilerReserve1]),(&SysTempOffset[idxTempBoilerReserve1])),
TempReserve2((&MPNumSys[0]),(&MPChanSys[idxTempBoilerReserve2]),(&SysTempOffset[idxTempBoilerReserve2]))
{
	Rooms = Rooms_;
	WarmWater = WarmWater_;
	Pump.SetOutputLimits(0.0, 1.0);
	bRelPool = false;
}

void cBoiler::getData( JsonObject& root )
{
	root["BV"] =  static_cast<int>( Valve.get());
	root["BT0"] = TempCharge.getRaw();
	root["BT1"] = TempTop.get();
	root["BT2"] = TempHead.get();
	root["BT3"] = TempReserve1.get();
	root["BT4"] = TempReserve2.get();
	
	root["BP"] =  Pump.get();
	root["BncW"] = static_cast<int>( bneedChargeWarmWater);
	root["BncH"] = static_cast<int>( bneedChargeHeating);
	root["Bdisc"] = static_cast<int>( bDischarging);
	root["Bc"] = static_cast<int>( bCharging);
	root["BTsW"] = WarmWater->SpTemp;
	root["BTsc"] = SpTemp();
}
