#ifndef cPool_h
#define cPool_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cBoiler.h"
#include "cPump.h"
#include "cValve.h"
#include "cLFPWM.h"
#include <cPID.h>
#include <ArduinoJson.h>

#define spTempPoolMax 26.0
#define PoolMargin 12.0
#define PoolHysteresis 1.0

class cPool
{
	public:
	
	cPool(cBoiler* Boiler_);
	
	double SpTemp(void) {
		double SpTemp_ = 0.0;
		if (forceChargePool())
			SpTemp_ = 55;
		else
			SpTemp_ = TempPool.get() + PoolMargin;
		
		return(SpTemp_);
	}
	
	boolean shouldCharge(void){
		if ((bmayChargePool ==false) && (TempPool.get() < spTempPoolMax))
		bmayChargePool = true;
		else if ((bmayChargePool ==true) && (TempPool.get() >= spTempPoolMax + PoolHysteresis))
		bmayChargePool = false;

		return(bmayChargePool && poolCirculationPumpActive());
	}

	boolean forceChargePool(void) {return(!digitalRead(PinFeedPoolSwitch));}

	boolean charge(boolean mayCharge, double TempSource);

	void getData(JsonObject& root);


	private:
	
	boolean bFeedPool;
	boolean bmayChargePool;
	cTempSensor TempPool;
	cValve Valve;
	cBoiler* Boiler;
	
	boolean poolCirculationPumpActive(void)
	{
		return(!digitalRead(PinPoolSwitch));
	}

};

#endif
