#ifndef cHeating_h
#define cHeating_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "PinDefinitions.h"
#include "cBoiler.h"
#include "cRoom.h"
#include "cBurner.h"

#include "cPump.h"

class cHeating
{
	private:
	double _TempSolarReturn;
	double _TempSolarLead;
	double _IntensitySolar;
	
	double _SpTempSource;
	
	boolean needSource;
	boolean needSink;
	
	enum Sinks {SiChargeWarmWater=1, SiChargeHeating=2, SiChargeRooms=3, SiOff=5} Sink;
	enum Sources {SoBurner=1, SoBurnerResHeat=2, SoSolar=3, SoBoiler=4, SoOff=5} Source;
	
	public:
	cPump PumpSolar;
	
	cRooms Rooms;
	cBoiler Boiler;
	cBurner Burner;
	
	cHeating(void);
	
	void Control(void);
	void checkSinks(void);
	void checkSources(void);
	void selectSink( int Sink );
	void selectSource( int Source );
};

#endif
