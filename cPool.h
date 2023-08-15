#ifndef cPool_h
#define cPool_h

#include <ArduinoJson.h>

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cPump.h"
#include "cTemp.h"
#include "cValve.h"

#define spTempPoolMax 27.0
#define PoolMargin 12.0
#define PoolHysteresis 1.0

class cPool {
   public:
	cPool(cPump* BoilerPump_);

	double SpTemp(void) {
		double SpTemp_ = 0.0;
		if (forceChargePool())
			SpTemp_ = 55;
		else
			SpTemp_ = TempPool.get() + PoolMargin;

		return (SpTemp_);
	}

	boolean shouldCharge(void) {
		if ((bmayChargePool == false) && (TempPool.get() < spTempPoolMax))
			bmayChargePool = true;
		else if ((bmayChargePool == true) && (TempPool.get() >= spTempPoolMax + PoolHysteresis))
			bmayChargePool = false;

		return (bmayChargePool && poolCirculationPumpActive());
	}

	boolean forceChargePool(void) { return (!digitalRead(PinFeedPoolSwitch)); }

	boolean charge(boolean mayCharge, double TempSource);

	void getData(JsonObject& root);

   private:
	boolean bFeedPool;
	boolean bmayChargePool;
	cTempSensor TempPool;
	cValve Valve;
	cPump* BoilerPump;

	boolean poolCirculationPumpActive(void) { return (!digitalRead(PinPoolSwitch)); }
};

#endif
