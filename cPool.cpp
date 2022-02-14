#include "cPool.h"

cPool::cPool(cBoiler* Boiler_):
Valve(PinValvePoolOpen,PinValvePoolClose),
TempPool(&MPNumSys[0], &MPChanSys[idxTempPool], &SysTempOffset[idxTempPool])
{
	Boiler = Boiler_;
	pinMode(PinPoolSwitch, INPUT_PULLUP);

	pinMode(PinValvePool, OUTPUT);
	digitalWrite(PinValvePool, LOW);
}

boolean cPool::charge(boolean mayCharge, double TempSource){
	double SpTempCharge = SpTemp();
	if (mayCharge) {
		Boiler->Pump.SetOutputLimits(1.0, 1.0);
//		Boiler->Pump.run(SpTempCharge, TempSource);
		Boiler->Pump.run(1.0);
		digitalWrite(PinValvePool, HIGH);
	}
	Valve.set(mayCharge);

	return mayCharge;
}



void cPool::getData( JsonObject& root )	
	{
	root["TPool"] = TempPool.get();
	root["bPoolActive"] = static_cast<int>(poolActive());
	//root["BP"] = Boiler.Pump.get();
	root["PV"] = static_cast<int>(Valve.get());
	root["bFeedPool"] =  static_cast<int>(FeedPool());
	}
