#include "cFlowHeater.h"


void cFlowHeater::getData( JsonObject& root )
{
	root["FHTtoSys"] =  TempToSystem.getRaw();
	root["FHV"] =  static_cast<int>( Valve.get());
	root["FHsuht"] = static_cast<int>( sufficientHeat);
	root["FHactive"] = static_cast<int>(not(digitalRead(PinFlowHeaterSwitch)));
}
