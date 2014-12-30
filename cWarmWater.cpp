#include "cWarmWater.h"

void cWarmWater::getSP( JsonObject& root )
{
	root["WWTempSchedule0time"] = TempSchedule[0].time.totalseconds();
	root["WWTempSchedule0temp"] = TempSchedule[0].temp;
	root["WWTempSchedule1time"] = TempSchedule[1].time.totalseconds();
	root["WWTempSchedule1temp"] = TempSchedule[1].temp;
}

int cWarmWater::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("WWTempSchedule0time")) {
		if(root["WWTempSchedule0time"].is<long>()) {
			TempSchedule[0].time.set(root["WWTempSchedule0time"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WWTempSchedule0temp")) {
		if(root["WWTempSchedule0temp"].is<double>()) {
			TempSchedule[0].temp = root["WWTempSchedule0temp"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WWTempSchedule1time")) {
		if(root["WWTempSchedule1time"].is<long>()) {
			TempSchedule[1].time.set(root["WWTempSchedule1time"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WWTempSchedule1temp")) {
		if(root["WWTempSchedule1temp"].is<double>()) {
			TempSchedule[1].temp = root["WWTempSchedule1temp"].as<double>();
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
	root["WarmWaterPump"] = Pump.getPower();
	root["WarmWaterIsTemp"] =  IsTemp.get();
}