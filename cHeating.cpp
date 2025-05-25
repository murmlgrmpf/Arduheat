#include "cHeating.h"


cHeating::cHeating(void):
WarmWater(),
Rooms(&WarmWater),
Boiler(&Rooms, &WarmWater),
TransferStation(),
FlowHeater(),
Pool(&(Boiler.Pump))
// Initialize and activate WarmWater Circulation Heating system
{
	pinMode(PinPumpCirculation, OUTPUT);
	digitalWrite(PinPumpCirculation, LOW);
}


void cHeating::Circulation(void){
	time_t dt  = elapsedSecsToday(now());
	time_t dt1 = hoursToTime_t(7);
	time_t dt2 = hoursToTime_t(9) + minutesToTime_t(20);
	time_t dt3 = hoursToTime_t(18);
	time_t dt4 = hoursToTime_t(19) + minutesToTime_t(55);
	
	if ((( (dt1<dt) && (dt<dt2) ) || ( (dt3<dt) && (dt<dt4) ))&&(WarmWater.Period.get()))
		digitalWrite(PinPumpCirculation, HIGH);
	else
		digitalWrite(PinPumpCirculation, LOW);

	//		digitalWrite(PinPumpCirculation, HIGH); //activate, if permanent Circulation required
}


void cHeating::Control(void) {
	WarmWater.Control();
	Circulation();
	checkSinks();
	checkSources();

	if (needSource || needSink) {
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
	// #0 Determine need to charge warm water and Rooms Heating
	if (Boiler.needChargeWarmWater() && Rooms.bAct) {
		Sink = SiCombined;
		SpTempSource = Boiler.SpTemp();
		needSource = true;
	}
	// #1 Determine need to charge warm water
	else if (Boiler.needChargeWarmWater()) {
		Sink = SiChargeWarmWater;
		SpTempSource = Boiler.SpTemp();
		needSource = true;
	}
	// #2 Determine whether Rooms Heating is requested
	else if (Rooms.bAct) {
		Sink = SiChargeRooms;
		SpTempSource = Rooms.SpTemp();
		needSource = true;
	}
	// #3 Charge into boiler
	else if (Boiler.shouldCharge()) {
		Sink = SiChargeHeating;
		SpTempSource = Boiler.SpTemp();
		needSource = false;
	}
	// #4 Charge the pool
	else if ((Pool.shouldCharge()) && (Boiler.hasMinHeat() || Pool.forceChargePool())) {
		Sink = SiChargePool;
		SpTempSource = Pool.SpTemp();
		if (SpTempSource >= 55)
			needSource = true;
		else
			needSource = false;
	}
	// Else Charge into boiler anyway
	else {
		Sink = SiChargeHeating;
		SpTempSource = Boiler.SpTemp();
		needSource = false;
	}
}

void cHeating::checkSources(void)
{
	// Priority of Heat sources:
	// #1 FlowHeater
	if (FlowHeater.hasHeat(SpTempSource)) {
		Source = SoSolar;
		TempSource = FlowHeater.TempSource();
		needSink = true;
	}
	// #3 Boiler
	else if ((! Boiler.needChargeWarmWater()) && (Boiler.Hot()) && (! Pool.shouldCharge())) {
		Source = SoBoiler;
		TempSource = Boiler.TempReserve1.get();
		needSink = false;
	}
	// #4 TransferStation
	else if (needSource) {
		Source = SoTransferStation;
		TempSource = TransferStation.TempSource();
		// Check for sufficient/residual heat
		needSink = false;
	}
	// #5 Off
	else {
		Source = SoOff;
		TempSource = (0.0);
		needSink = false;
	}
}

void cHeating::selectSource(int Source)
{
	switch (Source) {
		case SoSolar: {
			TransferStation.tap(SpTempSource, false, true);
			Boiler.discharge(false);
			break;
		}
		case SoBoiler: {
			if (TransferStation.tap(SpTempSource, false)) //remaining heat modus
				Boiler.discharge(false);
			else
				Boiler.discharge(true);
			break;
		}
		case SoTransferStation: {
			TransferStation.tap(SpTempSource, needSource);
			Boiler.discharge(false);
			break;
		}
		case SoOff: default: {
			TransferStation.tap(SpTempSource, false);
			Boiler.discharge(false);
			break;
		}
	}

	FlowHeater.harvest(SpTempSource);
}

void cHeating::selectSink( int Sink )
{
	boolean boilerMayCharge = false;
	boolean poolMayCharge = false;
	boolean roomsMayCharge = false;
	
	switch (Sink) {
		case SiCombined: {
			roomsMayCharge = true;
			boilerMayCharge = true;
			poolMayCharge = false;
			break;
		}
		case SiChargeWarmWater: {
			roomsMayCharge = false;
			boilerMayCharge = true;
			poolMayCharge = false;
			break;
		}
		case SiChargeRooms: {
			roomsMayCharge = true;
			if (FlowHeater.sufficientHeat && (FlowHeater.TempToSystem.getRaw() > Boiler.TempReserve1.get()))
				boilerMayCharge = true;
			else
				boilerMayCharge = false;
			poolMayCharge = false;
			break;
		}
		case SiChargeHeating: {
			roomsMayCharge = false;
			boilerMayCharge = true;
			poolMayCharge = false;
			break;
		}
		case SiChargePool: {
			roomsMayCharge = false;
			boilerMayCharge = false;
			poolMayCharge = true;
			break;
		}
		case SiOff: default: {
			roomsMayCharge = false;
			boilerMayCharge = false;
			poolMayCharge = false;
			break;
		}
	}
	
	boolean controlByRoomsMixer = ((TransferStation.GetMode()!=AUTOMATIC) || (Boiler.needChargeWarmWater() && Rooms.bAct));
	Rooms.charge(roomsMayCharge,controlByRoomsMixer);

	boolean bBoilerCharging = Boiler.charge(boilerMayCharge,TempSource,needSink);
	boolean bPoolCharging = Pool.charge(poolMayCharge, TempSource);
	if (!bBoilerCharging && !bPoolCharging)
		Boiler.Pump.run(); //stop charge pump
}

void cHeating::getData( JsonObject& root )
{
	root["SOTs"] = SpTempSource;

	root["nSO"] = static_cast<int>(needSource);
	root["nSI"] = static_cast<int>(needSink);

	root["SO"] = static_cast<int>(Source);
	root["SI"] = static_cast<int>(Sink);
}
