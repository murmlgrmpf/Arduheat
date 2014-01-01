#include "cBurner.h"

cBurner::cBurner()
:Valve(PinValveHeatSource1Open,PinValveHeatSource1Close),
TempLead(SystemMultiplexer,MultiplexTempHeatSource1Lead,OffsetTempHeatSource1Lead),
TempReturn(SystemMultiplexer,MultiplexTempHeatSource1Return,OffsetTempHeatSource1Return),
TempOperation(SystemMultiplexer,MultiplexTempHeatSource1Operation,OffsetTempHeatSource1Operation)
{
	_bFlame = false;
	_bResidualHeat = true;
	_StartTime =  millis();
	//_MinBurnTime = 240000;// 20 Minutes in Milliseconds
	_MinBurnTime = 5*60*1000;// 5 Minutes in Milliseconds
}


boolean cBurner::burn(boolean bShallBurn, double SpTempBoilerCharge)
{
	boolean bReady;
	
	// Start Burner flame and residual heat sequence
	if (bShallBurn && !_bFlame)
	{
		_StartTime = millis();
		_bFlame = true;
		_bResidualHeat = true;
	}
	
	// Stop Burner flame sequence
	if (!bShallBurn && (millis() > _StartTime + _MinBurnTime))
	{
		_bFlame = false;
	}
	
	// Execute State
	// Start Burner (start on Low), Stop Burner (stop on High)
	digitalWrite(PinStartHeatSource1, !_bFlame);
	
	// Is there residual heat?
	if (TempOperation.get() < SpTempBoilerCharge-2)
	{
		_bResidualHeat = false;
	}
	else if (TempOperation.get() > SpTempBoilerCharge+5)
	{
		_bResidualHeat = true;
	}
	
	bReady = (_bFlame || _bResidualHeat);
	
	// If yes, open valve
	Valve.set(bReady);
	//if (bReady)
	//{
		//Valve.set(true);
	//}
	//else
	//{
		//Valve.set(false);
	//}
	
	// Return if there is residual heat so that charging of boiler continues
	return bReady;
}
