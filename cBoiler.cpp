#include "cBoiler.h"

cBoiler::cBoiler(cRooms* Rooms_,cWarmWater* WarmWater_):
Valve(PinValveBoilerOpen,PinValveBoilerClose),
TempCharge((&MPNumSys[0]),(&MPChanSys[idxTempBoilerCharge]),(&SysTempOffset[idxTempBoilerCharge])),
TempReserve1((&MPNumSys[0]),(&MPChanSys[idxTempBoilerReserve1]),(&SysTempOffset[idxTempBoilerReserve1])),
TempReserve2((&MPNumSys[0]),(&MPChanSys[idxTempBoilerReserve2]),(&SysTempOffset[idxTempBoilerReserve2])),
TempHead((&MPNumSys[0]),&MPChanSys[idxTempBoilerHead],(&SysTempOffset[idxTempBoilerHead])),
TempTop((&MPNumSys[0]),&MPChanSys[idxTempBoilerTop],(&SysTempOffset[idxTempBoilerTop])),

Pump(PinPumpBoiler, 0.025, 0.0, 0.0, REVERSE)
{
	Rooms = Rooms_;
	WarmWater = WarmWater_;
	Pump.SetOutputLimits(0.0, 1.0);
	
	pinMode(PinValveBoiler, OUTPUT);
	digitalWrite(PinValveBoiler, LOW);
}

void cBoiler::getData( JsonObject& root )
{
	//char buffer[30];
	//PROGMEM prog_char  BoilerValve[]  = "BoilerValve";
	//strcpy_P(buffer, (char*)pgm_read_word(&(sBoilerValve)));
	root["BV"] =  static_cast<int>( Valve.get());

	root["BT0"] = TempCharge.getRaw();
	root["BT1"] = TempTop.get();
	root["BT2"] = TempHead.get();
	root["BT3"] = TempReserve1.get();
	root["BT4"] = TempReserve2.get();
	
	root["BP"] =  Pump.get();
	root["BncW"] =  static_cast<int>( bneedChargeWarmWater);
	root["BncH"] =  static_cast<int>( bneedChargeHeating);
	root["Bdisc"] =  static_cast<int>( bDischarging);
	root["Bc"] =  static_cast<int>( bCharging);
	root["BTsW"] =  WarmWater->SpTemp;
	root["BTsc"] =  SpTemp();
}
