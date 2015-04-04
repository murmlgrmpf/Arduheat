#include "cWarmWater.h"

void cWarmWater::Control(void){
	// If Warm Water is needed, execute the pump
	// with the power determined by pid controller.
	// Else switch pid to manual and stop pump.
	if(!digitalRead(PinWarmWaterSwitch))
		Pump.run(SpTemp, IsTemp.get());
	else
		Pump.run();
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