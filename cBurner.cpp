#include "cBurner.h"

void cBurner::getSP( JsonObject& root )
{
	root["BuTmax"] = MaxTempOperation;
	root["ButMin"] = MinBurnTime;
}

int cBurner::setSP( JsonObject& root )
{
	int posReturn =0;
	
	if(root.containsKey("BuTmax")) {
		if(root["BuTmax"].is<double>()) {
			double MaxTempOperation_ =  root["BuTmax"].as<double>();
                        if ((MaxTempOperation_>80.0)&&(MaxTempOperation_<91.0)&&(MaxTempOperation_!=MaxTempOperation)){
                            MaxTempOperation = MaxTempOperation_;
                            posReturn++;
                        }
		}
	}
	
	if(root.containsKey("ButMin")) {
		if(root["ButMin"].is<long>()) {
			double MinBurnTime_ =  static_cast<unsigned long>(root["ButMin"].as<long>());
                        if((MinBurnTime_>15*60000)&&(MinBurnTime_<25*60000)&&(MinBurnTime_!=MinBurnTime)) {
                            MinBurnTime = MinBurnTime_;
                            posReturn++;
                        }
		}
	}
	
        return posReturn;
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
