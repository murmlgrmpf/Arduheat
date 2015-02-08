#include "cSolar.h"


void cSolar::getData( JsonObject& root )
{
	root["STfromCol"] =  TempFromCollector.get();
	root["STtoCol"] =  TempToCollector.get();
	root["STfromSys"] =  TempFromSystem.get();
	root["STtoSys"] =  TempToSystem.get();
	//root["SIntensity"] =  Intensity.get();
	root["SP"] = Pump.get();
	root["SV"] =  static_cast<int>( Valve.get());
	root["Ssuht"] = static_cast<int>( sufficientHeat);
	root["Sprobing"] = static_cast<int>( probing);
}