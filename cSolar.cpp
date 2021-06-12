#include "cSolar.h"


void cSolar::getData( JsonObject& root )
{
	root["STfromCol"] =  TempFromCollector.getRaw();
	root["STtoSys"] =  TempToSystem.getRaw();
	root["SP"] = Pump.get();
	root["SV"] =  static_cast<int>( Valve.get());
	root["Ssuht"] = static_cast<int>( sufficientHeat);
	root["Sprobing"] = static_cast<int>( Probing.get());
	root["Sint"] = SolarIntensity.get();
}