#include "cHeating.h"


cHeating::cHeating(void):
WarmWater(),
Rooms(),
PumpSolar(PinPumpSolar),
Boiler(&Rooms, &WarmWater)
{
	_SpTempSource=0.0;
}

void cHeating::Control(void){
	checkSinks();
	checkSources();
	
	if(needSource||needSink)
	{
		selectSink(Sink);
		selectSource(Source);
	}
	else
	{
		selectSink(SiOff);
		selectSource(SoOff);
	}
	
	// Run solar pump to avoid boiling
	PumpSolar.setPower(0.0);
}


void cHeating::checkSinks(void)
{
	// Aggregate needs of heat sinks according to priority
	// #1 Determine need to charge warm water
	if(Boiler.needChargeWarmWater())
	{
		Sink = SiChargeWarmWater;
		_SpTempSource = Boiler.getSpTempCharge();
		needSource = true;
	}
	// #2 Determine whether there is need for heating the rooms
	else if (Rooms.need())
	{
		// Trigger hysteresis to keep burner burning
		Boiler.needChargeHeating(Rooms.need());
		Sink = SiChargeRooms;
		_SpTempSource = Rooms.getSpHeating();
		needSource = true;
	}
	//// #3 Determine need to charge heating
	//else if (Boiler.needChargeHeating(Rooms.need()))
	//{
		//// Boiler Heating needs charging and rooms need heat
		//Sink = SiChargeHeating;
		//_SpTempSource = Boiler.getSpTempCharge();
		//needSource = true;
	//}
	else
	{
		Sink = SiChargeHeating;
		_SpTempSource = Boiler.getSpTempCharge();
		needSource = false;
	}
	
}

void cHeating::checkSources(void)
{
	// Priority of Heat sources:
	// #1 If burner is burning: execute burner
	if(Burner.isBurning())
	{
		Source = SoBurner;
		needSink = true;
	}
	// #2 If burner is not burning: Burner residual heat: true if temperature high enough
	else if (Burner.burn(false,_SpTempSource))
	{
		Source = SoBurnerResHeat;
		needSink = true;
	}
	// #3 Solar
	else if (false)
	{
		Source = SoSolar;
		needSink = true;
	}
	// #4 Boiler
	else if (! (Boiler.needChargeWarmWater() || Boiler.needChargeHeating(Rooms.need())) )
	{
		Source = SoBoiler;
		needSink = false;
	}
	// #5 Start Burner
	else if (needSource)
	{
		Source = SoBurner;
		needSink = true;
		Boiler.triggerChargeWarmWater();
	}
	
}

void cHeating::selectSource( int Source )
{
	switch (Source)
	{
		case SoBurner:
		{
			// Check hysteresis for charging the boiler
			if( Boiler.needChargeWarmWater() || Boiler.needChargeHeating(Rooms.need()) )
			{
				Burner.burn(true, _SpTempSource);
			}
			else
			{
				Burner.burn(false, _SpTempSource);
			}
			
			// Deactivate other heat sources
			Boiler.discharge(false);
			break;
		}
		case SoBurnerResHeat:
		{
			// Residual heat mode
			// Deactivate other heat sources
			Burner.burn(false, _SpTempSource);
			Boiler.discharge(false);
			break;
		}
		case SoSolar:
		{
			// Solar mode
			// Deactivate other heat sources
			Burner.burn(false, _SpTempSource);
			Boiler.discharge(false);
			break;
		}
		case SoBoiler:
		{
			// Boiler source mode
			Boiler.discharge(true);
			// Deactivate other heat sources
			Burner.burn(false, _SpTempSource);
			break;
		}
		case SoOff: default:
		{
			// Deactivate all heat sources
			// Solar
			Boiler.discharge(false);
			Burner.burn(false, _SpTempSource);
			break;
		}
	}
}

void cHeating::selectSink( int Sink )
{
	boolean ChargeBoilerAndRooms = false;
	switch (Sink)
	{
		case SiChargeWarmWater:
		{
			Boiler.charge(true);
			Rooms.ChargeRooms(false);
			break;
		}
		case SiChargeRooms:
		{
			// TODO: Solar überschuss abholen
			ChargeBoilerAndRooms = (Boiler.getSpTempCharge() < Burner.TempOperation.get());
			Rooms.ChargeRooms(true, ChargeBoilerAndRooms);
			Boiler.charge(ChargeBoilerAndRooms);
			break;
		}
		case SiChargeHeating:
		{
			Rooms.ChargeRooms(true);
			Boiler.charge(true);
			break;
		}
		case SiOff: default:
		{
			Rooms.ChargeRooms(false);
			Boiler.charge(false);
			break;
		}
	}
}

void cHeating::getData( JsonObject& root )
{
	root["TempSolarReturn"] = _TempSolarReturn;
	root["TempSolarLead"] = _TempSolarLead;
	root["IntensitySolar"] = _IntensitySolar;
	
	root["SpTempSource"] = _SpTempSource;
	
	root["needSource"] = static_cast<int>(needSource);
	root["needSink"] = static_cast<int>(needSink);
	
	root["Source"] = static_cast<int>(Source);
	root["Sink"] = static_cast<int>(Sink);
}
