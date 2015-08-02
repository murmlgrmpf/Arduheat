#include "cWarmWater.h"

void cWarmWater::Control(void){
	// If Warm Water is needed, execute the pump
	// with the power determined by pid controller.
	// Else switch pid to manual and stop pump.
	if(!digitalRead(PinWarmWaterSwitch))
		Pump.run(SpTemp, IsTemp.get());
	else
		Pump.run();
	
	if(Trigger.get())
		setParam();
}

void cWarmWater::setParam()
{
	double P = min(0.5,max(0.1, 1.127-0.011*IsTempBoilerTop.get()));
	double D = min(7.0,max(2.6,17.9-0.19*IsTempBoilerTop.get()));
	SpTemp = max(45, 0.143*IsTempBoilerTop.get() + 37.1);
	Pump.SetTunings(P, Pump.GetKi(), D);
}

void cWarmWater::getSP( JsonObject& root )
{
	root["WWTs"] = SpTemp;
}

int cWarmWater::setSP( JsonObject& root )
{
	if(root.containsKey("WWTs")) {
		if(root["WWTs"].is<double>()) {
			SpTemp = root["WWTs"].as<double>();
			return true;
		}
	}
	
	return false;
}

void cWarmWater::getData( JsonObject& root )
{
	root["WP"] = Pump.get();
	root["WTi"] =  IsTemp.get();
	
	root["Wn"] = static_cast<int>(!digitalRead(PinWarmWaterSwitch));
	root["WTitoB"] = IsTempWarmWaterToBoiler.get();
}