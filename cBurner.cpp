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
	//_MinBurnTime = 240000;// 20 Minutes in Milliseconds
	_MinBurnTime = MinBurnTimeMinutes*60*1000;// 5 Minutes in Milliseconds
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
	if (TempOperation.get()<MaxTempOperation)
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