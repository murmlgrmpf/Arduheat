#include "cBurner.h"

cBurner::cBurner()
:Valve(PinValveHeatSource1Open,PinValveHeatSource1Close),
TempLead(SystemMultiplexer,MultiplexTempHeatSource1Lead,OffsetTempHeatSource1Lead),
TempReturn(SystemMultiplexer,MultiplexTempHeatSource1Return,OffsetTempHeatSource1Return),
TempOperation(SystemMultiplexer,MultiplexTempHeatSource1Operation,OffsetTempHeatSource1Operation)
{
	_bFlame = false;
	_sufficientHeat = true;
	_StartTime =  millis();
	_MaxTempOperation = MaxTempOperation;
	//_MinBurnTime = 240000;// 20 Minutes in Milliseconds
	_MinBurnTime = static_cast<unsigned long>(MinBurnTimeMinutes)* static_cast<unsigned long>(60000);// 5 Minutes in Milliseconds 60*1000 = 1Minute
}


boolean cBurner::burn(boolean bShallBurn, double SpTempSource)
{
	
	// Start Burner flame and residual heat sequence
	if (bShallBurn && !_bFlame)
	{
		_StartTime = millis();
		_bFlame = true;
	}
	
	// Stop Burner flame sequence
	if (!bShallBurn && (millis() > _StartTime + _MinBurnTime))
	{
		_bFlame = false;
	}
	
	// Execute State, check for overheating
	if (TempOperation.get()<_MaxTempOperation)
	{
		// Start Burner (start on Low), Stop Burner (stop on High)
		digitalWrite(PinStartHeatSource1, !_bFlame);
	}
	else
	{
		// Overheating: shut off flame
		digitalWrite(PinStartHeatSource1, !false);
	}
	
	// Is there residual/sufficient heat?
	if (TempOperation.get() < SpTempSource-2)
	{
		_sufficientHeat = false;
	}
	if ((TempOperation.get() > SpTempSource+5) || _bFlame)
	{
		_sufficientHeat = true;
	}

	Valve.set(_sufficientHeat);
	
	
	// Return if there is residual heat so that charging of boiler continues
	return _sufficientHeat;
}

boolean cBurner::isBurning( void )
{
	return _bFlame;
}

void cBurner::getSP( JsonObject& root )
{
	root["BurnerMaxTempOperation"] = _MaxTempOperation;
	root["BurnerMinBurnTime"] = _MinBurnTime;
}

int cBurner::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("BurnerMaxTempOperation")) {
		if(root["BurnerMaxTempOperation"].is<double>()) {
			_MaxTempOperation =  root["BurnerMaxTempOperation"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if(root.containsKey("BurnerMinBurnTime")) {
		if(root["BurnerMinBurnTime"].is<long>()) {
			_MinBurnTime =  static_cast<unsigned long>(root["BurnerMinBurnTime"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if (fail == 0) { // If all three parameter objects were successfully filled
		return posReturn;
	}
	else
	{
		return 0;
	}
}

void cBurner::getData( JsonObject& root )
{
	root["BurnerValve"] =  static_cast<int>(Valve.get());
	
	root["BurnerStartTime"] =  _StartTime;
	root["BurnerMinBurnTime"] =  _MinBurnTime;
	
	root["BurnersufficientHeat"] =  static_cast<int>(_sufficientHeat);
	root["BurnerbFlame"] =  static_cast<int>(_bFlame);
	root["BurnerTempLead"] =  TempLead.get();
	root["BurnerTempReturn"] =  TempReturn.get();
	root["BurnerTempOperation"] =  TempOperation.get();
}
