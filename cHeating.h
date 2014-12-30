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
	
	double SpTempSource;
	
	boolean needSource;
	boolean needSink;
	
	enum Sinks {SiChargeWarmWater=1, SiChargeHeating=2, SiChargeRooms=3, SiOff=5} Sink;
	enum Sources {SoBurner=1, SoBurnerResHeat=2, SoSolar=3, SoBoiler=4, SoOff=5} Source;
	
};

#endif
