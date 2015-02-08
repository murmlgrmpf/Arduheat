#ifndef cHeating_h
#define cHeating_h

#include "Arduino.h"

#include "PinDefinitions.h"
#include "cBoiler.h"
#include "cRooms.h"
#include "cBurner.h"
#include "cWarmWater.h"
#include "cSolar.h"

#include <ArduinoJson.h>

class cHeating
{	
	public:
	
	cWarmWater WarmWater;
	cRooms Rooms;
	cBoiler Boiler;
	cBurner Burner;
	cSolar Solar;
	
	cHeating(void);
	
	void Control(void);
	void checkSinks(void);
	void checkSources(void);
	void selectSink( int Sink );
	void selectSource( int Source );
	
	void getData(JsonObject& root);
	
	private:
	
	float SpTempSource;
	float TempSource;
	
	boolean needSource;
	boolean needSink;
	
	enum Sinks {SiChargeWarmWater, SiChargeHeating, SiChargeRooms, SiOff} Sink;
	enum Sources {SoBurner, SoBurnerResHeat, SoSolar, SoBoiler, SoOff} Source;
	
};

#endif
