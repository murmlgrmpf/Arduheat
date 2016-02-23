#include "cHeating.h"


cHeating::cHeating(void):
WarmWater(),
Rooms(),
Boiler(&Rooms, &WarmWater),
Solar()
{
	// Initialize and deactivate WarmWater Circulation Heating system
	pinMode(PinPumpCirculation, OUTPUT);
	digitalWrite(PinPumpCirculation,HIGH);
	// Initialize Control line for Remote Heating system
	pinMode(PinHeatControl, OUTPUT);
	SpTempSource=0.0;
	TempSource=0.0;
}

void cHeating::Control(void){
	checkSinks();
	checkSources();
	
	if(needSource||needSink) {
		selectSink(Sink);
		selectSource(Source);
	}
	else {
		selectSink(SiOff);
		selectSource(SoOff);
	}
}


void cHeating::checkSinks(void)
{
	// Aggregate needs of heat sinks according to priority
	// #1 Determine need to charge warm water
	if(Boiler.needChargeWarmWater()) {
		Sink = SiChargeWarmWater;
		SpTempSource = Boiler.SpTemp();
		needSource = true;
	}
	// #2 Determine whether there is need for heating the rooms
	else if (Rooms.need()) {
		Sink = SiChargeRooms;
		SpTempSource = Rooms.getSpHeating();
		needSource = true;
	}
	// Else charge into boiler
	else {
		Sink = SiChargeHeating;
		SpTempSource = Boiler.SpTemp();
		needSource = false;
	}
}

void cHeating::checkSources(void)
{
	// Priority of Heat sources:
	// #1 If burner is burning: execute burner
	if(Burner.isBurning()) {
		Source = SoBurner;
		TempSource = Burner.TempOperation.get();
		needSink = true;
	}
	// #2 Solar
	else if (Solar.harvest(SpTempSource, true)) {
		Source = SoSolar;
		TempSource = Solar.TempSource();
		needSink = true;
	}
	// #3 If burner is not burning: Burner residual heat: true if temperature high enough
	else if (Burner.burn(SpTempSource, false)) {
		Source = SoBurnerResHeat;
		TempSource = Burner.TempOperation.get();
		needSink = true;
	}
	// #4 Boiler
	else if (! (Boiler.needChargeWarmWater() || Boiler.needChargeHeating(Rooms.need())) ) {
		Source = SoBoiler;
		TempSource = 0.0;
		needSink = false;
	}
	// #5 Start Burner
	else if (needSource) {
		Source = SoBurner;
		TempSource = Burner.TempOperation.get();
		needSink = true;
	}
}

void cHeating::selectSource( int Source )
{
	switch (Source) {
		case SoBurner: {
			// Check hysteresis for charging the boiler
			if( Boiler.needChargeWarmWater() || Boiler.needChargeHeating(Rooms.need()) )
				Burner.burn(SpTempSource, true);
			else
				Burner.burn(SpTempSource, false);
			
			// Deactivate other heat sources
			Solar.harvest(SpTempSource, false);
			Boiler.discharge(false);
			break;
		}
		case SoBurnerResHeat: {
			// Residual heat mode
			// Deactivate other heat sources
			Burner.burn(SpTempSource, false);
			Solar.harvest(SpTempSource, true);
			Boiler.discharge(false);
			break;
		}
		case SoSolar: {
			// Solar mode
			Solar.harvest(SpTempSource, true);
			// Deactivate other heat sources
			Burner.burn(TempSource, false);
			Boiler.discharge(false);
			break;
		}
		case SoBoiler: {
			// Boiler source mode
			Boiler.discharge(true);
			Solar.harvest(SpTempSource, true);
			// Deactivate other heat sources
			Burner.burn(SpTempSource, false);
			break;
		}
		case SoOff: default: {
			Solar.harvest(SpTempSource, true);
			// Deactivate all heat sources
			Boiler.discharge(false);
			Burner.burn(SpTempSource, false);
			break;
		}
	}
}

void cHeating::selectSink( int Sink )
{
	switch (Sink) {
		case SiChargeWarmWater: {
			Rooms.ChargeRooms(false);
			Boiler.charge(TempSource);
			break;
		}
		case SiChargeRooms: {
			Rooms.ChargeRooms(true);
			Boiler.charge(TempSource);
			break;
		}
		case SiChargeHeating: {
			Rooms.ChargeRooms(false);
			Boiler.charge(TempSource, needSink);
			break;
		}
		case SiOff: default: {
			Rooms.ChargeRooms(false);
			Boiler.charge(TempSource);
			break;
		}
	}
}

void cHeating::getData( JsonObject& root )
{
	root["SOTs"] = SpTempSource;
	
	root["nSO"] = static_cast<int>(needSource);
	root["nSI"] = static_cast<int>(needSink);
	
	root["SO"] = static_cast<int>(Source);
	root["SI"] = static_cast<int>(Sink);
}
