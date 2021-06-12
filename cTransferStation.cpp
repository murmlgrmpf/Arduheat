#include "cTransferStation.h"


void cTransferStation::getData( JsonObject& root )
{
	root["TsTfromTele"] =  TempFromTeleheating.getRaw();
//	root["TsTtoTele"] =  TempToTeleheating.getRaw();
	root["TsTOp"] =  TempOperation.getRaw();
	root["TsPower"] =  Power;
	root["TsV"] =  static_cast<int>( Valve.get());
	root["Tssuht"] = static_cast<int>( sufficientHeat);
}
