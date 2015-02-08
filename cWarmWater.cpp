#include "cWarmWater.h"

void cWarmWater::Control(void){
	// If Warm Water is needed, execute the pump
	// with the power determined by pid controller.
	// Else switch pid to manual and stop pump.
	if(!digitalRead(PinWarmWaterSwitch))
	Pump.setPower(pid.run(SpTemp(), IsTemp.get()));
	else
	Pump.setPower(pid.run());
}

void cWarmWater::getSP( JsonObject& root )
{
	root["Wts0"] = TempSchedule[0].time.totalseconds();
	root["WTs0"] = TempSchedule[0].temp;
	root["Wts1"] = TempSchedule[1].time.totalseconds();
	root["WTs1"] = TempSchedule[1].temp;
}

int cWarmWater::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("Wts0")) {
		if(root["WTs0t"].is<long>()) {
			TempSchedule[0].time.set(root["WTs0t"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WTs0")) {
		if(root["WTs0T"].is<double>()) {
			TempSchedule[0].temp = root["WTs0T"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("Wts1")) {
		if(root["Wts1"].is<long>()) {
			TempSchedule[1].time.set(root["Wts1"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WTs1")) {
		if(root["WTs1"].is<double>()) {
			TempSchedule[1].temp = root["WTs1"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	// If all three parameter objects were successfully filled
	if (fail == 0) return posReturn;
	else return 0;
}

void cWarmWater::getData( JsonObject& root )
{
	root["WP"] = pid.get();
	root["WTi"] =  IsTemp.get();
	root["Wn"] = !digitalRead(PinWarmWaterSwitch);
	root["WTitoB"] = IsTempWarmWaterToBoiler.get();
}