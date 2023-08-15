#ifndef cHeating_h
#define cHeating_h

#include "Arduino.h"

#include "PinDefinitions.h"
#include "cBoiler.h"
#include "cRooms.h"
#include "cTransferStation.h"
#include "cWarmWater.h"
#include "cSolar.h"
#include "cPool.h"

#include <TimeLib.h>

#include <ArduinoJson.h>


class cHeating
{	
	public:

	cWarmWater WarmWater;
	cRooms Rooms;
	cBoiler Boiler;
	cTransferStation TransferStation;
	cSolar Solar;
	cPool Pool;
	
	cHeating(void);
	
	void Control(void);
	void checkSinks(void);
	void checkSources(void);
	void selectSink( int Sink );
	void selectSource( int Source );
	
	void Circulation(void);
	
	void getData(JsonObject& root);
	
	private:
	
	float SpTempSource;
	float TempSource;
	
	boolean needSource;
	boolean needSink;
	
	enum Sinks {SiCombined, SiChargeWarmWater, SiChargeHeating, SiChargeRooms, SiChargePool, SiOff} Sink;
	enum Sources {SoTransferStation, SoSolar, SoBoiler, SoOff} Source;
	
};

#endif
