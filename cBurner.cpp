#include "cBurner.h"

void cBurner::getSP( JsonObject& root )
{
	root["BuTmax"] = MaxTempOperation;
	root["ButMin"] = MinBurnTime;
}

int cBurner::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("BuTmax")) {
		if(root["BuTmax"].is<double>()) {
			MaxTempOperation =  root["BuTmax"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if(root.containsKey("ButMin")) {
		if(root["ButMin"].is<long>()) {
			MinBurnTime =  static_cast<unsigned long>(root["ButMin"].as<long>());
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
	root["BuV"] =  static_cast<int>(Valve.get());
	root["BuT"] =  TempOperation.get();
	// Variables
	root["BuStartt"] =  StartTime;
	root["Busuht"] =  static_cast<int>(sufficientHeat);
	root["BuBurns"] =  static_cast<int>(bFlame);
}
