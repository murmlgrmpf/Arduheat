#ifndef cPool_h
#define cPool_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cBoiler.h"
#include "cValve.h"
#include "cPump.h"
#include "cLFPWM.h"
#include <cPID.h>
#include <ArduinoJson.h>

#define spTempPoolMax 22.0
#define PoolMargin 5.0
#define PoolHysteresis 1.0

class cPool
{
	public:
	
	cPool(cBoiler* Boiler_);
	
	double SpTemp(void) {
		return(TempPool.get()+PoolMargin);
	}
	
	boolean shouldCharge(void){
		if (TempPool.get()<spTempPoolMax)
			bmayChargePool = true;
		else if (TempPool.get()>=spTempPoolMax+PoolHysteresis)
			bmayChargePool = false;
			
		return(bmayChargePool && poolActive());
	}
	
	boolean charge(boolean mayCharge, double TempSource);

	void getData(JsonObject& root);

	cBoiler* Boiler;

	private:
	
	boolean bmayChargePool;
	cTempSensor TempPool;
	cValve Valve;
//	cBoiler* Boiler;
	
	boolean poolActive(void)
	{
		return(!digitalRead(PinPoolSwitch));
	}
	
};




#endif
