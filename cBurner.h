#ifndef cBurner_h
#define cBurner_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include <ArduinoJson.h>

#define DefaultMinBurnTimeMinutes 20
#define DefaultMaxTempOperation 80

class cBurner
{
	
	public:
	cBurner(void)
	:Valve(PinValveHeatSource1Open,PinValveHeatSource1Close),
	TempLead(SystemMultiplexer,MultiplexTempHeatSource1Lead,OffsetTempHeatSource1Lead),
	TempReturn(SystemMultiplexer,MultiplexTempHeatSource1Return,OffsetTempHeatSource1Return),
	TempOperation(SystemMultiplexer,MultiplexTempHeatSource1Operation,OffsetTempHeatSource1Operation)
	{
		pinMode(PinStartHeatSource1, OUTPUT);
		
		bFlame = false;
		sufficientHeat = true;
		StartTime =  millis();
		MaxTempOperation = DefaultMaxTempOperation;
		//MinBurnTime = 240000;// 20 Minutes in Milliseconds
		// 60*1000 Milliseconds = 1 Minute
		MinBurnTime = static_cast<unsigned long>(DefaultMinBurnTimeMinutes)* static_cast<unsigned long>(60000);
	}
	
	boolean burn(boolean bShallBurn, double SpTempSource)
	{
		
		// Start Burner flame and residual heat sequence
		if (bShallBurn && !bFlame) {
			StartTime = millis();
			bFlame = true;
		}
		
		// Stop Burner flame sequence if it does not need to burn and minimum burn time elapsed
		if (!bShallBurn && (millis() - StartTime > MinBurnTime))
			bFlame = false;
		
		// Execute state, check for overheating
		if (TempOperation.get() < MaxTempOperation)
			digitalWrite(PinStartHeatSource1, !bFlame); // Start Burner (start on Low), Stop Burner (stop on High)
		else
			digitalWrite(PinStartHeatSource1, !false); // Overheating: shut off flame
		
		// Check for residual/sufficient heat with hysteresis
		if (TempOperation.get() < SpTempSource-2)
			sufficientHeat = false;
		if ((TempOperation.get() > SpTempSource+5) || bFlame)
			sufficientHeat = true;

		Valve.set(sufficientHeat);
		
		// Return if there is residual heat so that charging of boiler continues
		return sufficientHeat;
	}
	
	boolean isBurning(void) {return bFlame;};
	
	cTempSensor TempLead;
	cTempSensor TempOperation;
	cTempSensor TempReturn;
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	
	private:
	cValve Valve;
	
	boolean sufficientHeat;
	boolean bFlame;
	unsigned long StartTime;
	
	unsigned long MinBurnTime;
	double MaxTempOperation;
};

#endif
