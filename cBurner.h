#ifndef cBurner_h
#define cBurner_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include <ArduinoJson.h>

#define DefaultMinBurnTimeMinutes 15 //20
#define DefaultMaxTempOperation 80

class cBurner
{
	
	public:
	cBurner(void)
	:Valve(PinValveHeatSource1Open,PinValveHeatSource1Close),
	TempOperation((&MPNumSys[0]),(&MPChanSys[idxTempHeatSource1Operation]),(&SysTempOffset[idxTempHeatSource1Operation]))
	{
		pinMode_wrap(PinStartHeatSource1, OUTPUT);
		
		bFlame = false;
		sufficientHeat = true;
		StartTime =  millis();
		MaxTempOperation = DefaultMaxTempOperation;
		//MinBurnTime = 240000;// 20 Minutes in Milliseconds
		// 60*1000 Milliseconds = 1 Minute
		MinBurnTime = static_cast<unsigned long>(DefaultMinBurnTimeMinutes)* static_cast<unsigned long>(60000);
	}
	
	boolean burn(double SpTempSource, boolean bShallBurn = false)
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
			digitalWrite_wrap(PinStartHeatSource1, !bFlame); // Start Burner (start on Low), Stop Burner (stop on High)
		else
			digitalWrite_wrap(PinStartHeatSource1, !false); // Overheating: shut off flame
		
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
	
	cTempSensor TempOperation;
	
	void getSP(JsonObject& root);
	int setSP(JsonObject& root);
	
	void getData(JsonObject& root);
	
	private:
	cValve Valve;
	
	boolean sufficientHeat;
	boolean bFlame;
	unsigned long StartTime;
	
	unsigned long MinBurnTime;
	float MaxTempOperation;
};

#endif
