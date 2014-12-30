#include "cBurner.h"

void cBurner::getSP( JsonObject& root )
{
	root["BurnerMaxTempOperation"] = MaxTempOperation;
	root["BurnerMinBurnTime"] = MinBurnTime;
}

int cBurner::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("BurnerMaxTempOperation")) {
		if(root["BurnerMaxTempOperation"].is<double>()) {
			MaxTempOperation =  root["BurnerMaxTempOperation"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if(root.containsKey("BurnerMinBurnTime")) {
		if(root["BurnerMinBurnTime"].is<long>()) {
			MinBurnTime =  static_cast<unsigned long>(root["BurnerMinBurnTime"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if (fail == 0) { // If all three parameter objects were successfully filled
		return posReturn;
	}
	else {
		return 0;
	}
}

void cBurner::getData( JsonObject& root )
{
	// Objects
	root["BurnerValve"] =  static_cast<int>(Valve.get());
	root["BurnerTempLead"] =  TempLead.get();
	root["BurnerTempReturn"] =  TempReturn.get();
	root["BurnerTempOperation"] =  TempOperation.get();
	// Variables
	root["BurnerStartTime"] =  StartTime;
	root["BurnersufficientHeat"] =  static_cast<int>(sufficientHeat);
	root["BurnerbFlame"] =  static_cast<int>(bFlame);
}
