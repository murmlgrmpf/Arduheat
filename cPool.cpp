#include "cPool.h"

cPool::cPool(cPump* BoilerPump_):
TempPool(&MPNumSys[0], &MPChanSys[idxTempPool], &SysTempOffset[idxTempPool]),
Valve(PinValvePool)
{
	BoilerPump = BoilerPump_;
	pinMode(PinPoolSwitch, INPUT_PULLUP);
	pinMode(PinFeedPoolSwitch, INPUT_PULLUP);
}

boolean cPool::charge(boolean mayCharge, double TempSource) {
	double SpTempCharge = SpTemp();
	if (mayCharge) {
		BoilerPump->SetOutputLimits(0.2, 1.0);
		BoilerPump->run(SpTempCharge, TempSource);
	}

	Valve.set(mayCharge);

	return mayCharge;
}

void cPool::getData( JsonObject& root )	{
		root["TPool"] = TempPool.get();
		root["bPoolActive"] = static_cast<int>(poolCirculationPumpActive());
		root["PV"] = static_cast<int>(Valve.get());
		root["bFeedPool"] = static_cast<int>(forceChargePool());
	}
