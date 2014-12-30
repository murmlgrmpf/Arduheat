#include "cRooms.h"

cRooms::cRooms( void ):
IsTempHeatingLead(SystemMultiplexer,MultiplexTempHeatingLead,OffsetTempHeatingLead),
IsTempHeatingReturn(SystemMultiplexer,MultiplexTempHeatingReturn,OffsetTempHeatingReturn),
TempOutside(SystemMultiplexer,MultiplexTempOutside,OffsetTempOutside),
PIDPump( 0.2, 0.1, 0.05, DIRECT),
PIDMixer( 0.5, 0.0, 0.005, DIRECT),
Pump(PinPumpHeating),
Mixer(PinMixerOpen,PinMixerClose)
{
	SetType = Normal;
	// Initialize PID controllers for pumps
	PIDPump.SetOutputLimits(0.4, 1.0);
	PIDMixer.SetOutputLimits(-1.0, 1.0);
	PIDMixer.SetSampleTime(500);
	
	dMaxDiff =0;
	dMaxSp = 0;
	
	initDefaultSetpoint();
}

void cRooms::initDefaultSetpoint()
{
	RoomTemps[Living] = 20.0;//15;//
	RoomTemps[Sleeping] =20.0;//15;//
	RoomTemps[Hallway] = 19.0;//15;//
	RoomTemps[Bath] = 22.0;//15;//
	RoomTemps[Side] = 16.0;//15;//
	
	double temp[nSwitch];
	temp[0] = 0.0;
	temp[1] = -1.0;//0.0;//
	temp[2] = 0.0;
	temp[3] = -1.0;//0.0;//
	TimeSpan switchtime[nSwitch];
	switchtime[0].set(0,6,0,0);
	switchtime[1].set(0,8,0,0);
	switchtime[2].set(0,18,0,0);
	switchtime[3].set(0,21,00,0);
	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSetTypes; iSet++)
	{
		for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
		{
			for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
			{
				// Discriminate between Rooms that do not reduce temperature during the day and those who do
				if((iSwitch==1)){
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].time.set(switchtime[iSwitch]);
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].temp = temp[iSwitch-1];
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].time.set(switchtime[iSwitch]);
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].temp = temp[iSwitch-1];
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].time.set(switchtime[iSwitch]);
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].temp = temp[iSwitch-1];
				}
				else {
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].time.set(switchtime[iSwitch]);
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].temp = temp[iSwitch];
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].time.set(switchtime[iSwitch]);
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].temp = temp[iSwitch];
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].time.set(switchtime[iSwitch]);
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].temp = temp[iSwitch];
				}
				
				TempOffsetSchedule[iSet][Sleeping][iDayType][iSwitch].time.set(switchtime[iSwitch]);
				TempOffsetSchedule[iSet][Sleeping][iDayType][iSwitch].temp = temp[iSwitch];
				TempOffsetSchedule[iSet][Bath][iDayType][iSwitch].time.set(switchtime[iSwitch]);
				TempOffsetSchedule[iSet][Bath][iDayType][iSwitch].temp = temp[iSwitch];
			}
		}
	}
}


void cRooms::ChargeRooms( boolean ChargeRooms, boolean BoilerCharges )
{
	dSpTempHeatingLead = getSpHeating();
	dIsTempHeatingLead = IsTempHeatingLead.get();
	dSpTempHeatingReturn = dIsTempHeatingLead-DiffTempHeatingLeadReturn;
	dIsTempHeatingReturn = IsTempHeatingReturn.get();
	
	if (ChargeRooms)
	{
		
		// Run Mixer and PID
		Mixer.run(PIDMixer.run(dSpTempHeatingLead, dIsTempHeatingLead));
		
		// Run heating pump and PID
		if (BoilerCharges)
		{
			// If Boiler is charged, run heating pump at full speed.
			// The Boiler is collecting the remaining heat from the source.
			PIDPump.run();
			Pump.setPower(1.0);
		}
		else
		{
			Pump.setPower(PIDPump.run(dSpTempHeatingReturn, dIsTempHeatingReturn));
		}
	}
	else
	{
		// Stop Pump Heating and PID
		PIDPump.run();
		Pump.setPower(0.0);
		// Stop mixer PID and run Mixer to closed position
		PIDMixer.run();
		Mixer.run(-1.0);
	}
}

boolean cRooms::need(void)
{
	// Reset Values
	double dneedChargeRooms = 0.0;
	dMaxSp = 0.0;
	dMaxDiff = 0.0;
	
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
		// Store maximum set point
		if (Room[i].getSpTemp()>dMaxSp)
			dMaxSp = Room[i].getSpTemp();
		// Store maximum sp-is difference
		if ((Room[i].getSpTemp() - Room[i].IsTemp.get()) > dMaxDiff)
			dMaxDiff = Room[i].getSpTemp() - Room[i].IsTemp.get();
		// Check for rooms needing heat
		if (Room[i].getNeed()>dneedChargeRooms)
			dneedChargeRooms = Room[i].getNeed();
	}
	
	// Hysteresis for charging the rooms
	if (dneedChargeRooms >  RoomMargin) needCharge = true;
	if (dneedChargeRooms < -RoomMargin) needCharge = false;
	
	return needCharge;
}

double cRooms::getSpHeating(void)
{
	// Update dMaxDiff and dMaxSp
	need();
	
	double t1 = (TempOutside.get()/(320-4*TempOutside.get()));
	double t2 = pow(dMaxSp,t1);
	double swhk = 0.55*dsteil*t2*(-TempOutside.get()+20)*2+dMaxSp+dkh+dMaxDiff*dverst;
	double swhkLimit = min(max(swhk,dminvl),dmaxvl);
	
	return swhkLimit;
}

void cRooms::getOffsetTime( JsonObject& root )
{
	JsonArray&  times  = root.createNestedArray("times");
	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSetTypes; iSet++)
	{
		for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
		{
			for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
			{
				for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
				{
					times.add(TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time.totalseconds());
				}
			}
		}
	}
}

int cRooms::setOffsetTime( JsonObject& root )
{
	if(root.containsKey("times")) {
		if(root["times"].is<JsonArray&>()){
			JsonArray& times = root["times"];
			
			if (times.size()==(nSetTypes*nRoomTypes*nDayTypes*nSwitch))
			{
				for(int iSet = 0; iSet<nSetTypes; iSet++)
				{
					for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
					{
						for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
						{
							for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
							{
								// Last iteration
								//int idx = (nSwitch-1)+(nDayTypes-1)*(nSwitch)+(nRoomTypes-1)*(nDayTypes)*(nSwitch)+(nSetTypes-1)*(nRoomTypes)*(nDayTypes)*(nSwitch);
								int idx = iSwitch+iDayType*(nSwitch)+iRoomType*(nDayTypes)*(nSwitch)+iSet*(nRoomTypes)*(nDayTypes)*(nSwitch);
								TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time.set(times[idx].as<long>());
							}
						}
					}
				}
				return 1;
			}
		}
	}
	return 0;
}


void cRooms::getOffsetTemp( JsonObject& root )
{
	JsonArray&  temps  = root.createNestedArray("temps");
	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSetTypes; iSet++)
	{
		for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
		{
			for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
			{
				for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
				{
					temps.add(TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp);
				}
			}
		}
	}
}


int cRooms::setOffsetTemp( JsonObject& root )
{
	if(root.containsKey("temps")) {
		if(root["temps"].is<JsonArray&>()){
			JsonArray& temps = root["temps"];
			
			if (temps.size()==(nSetTypes*nRoomTypes*nDayTypes*nSwitch))
			{
				for(int iSet = 0; iSet<nSetTypes; iSet++)
				{
					for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
					{
						for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
						{
							for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
							{
								// Last iteration
								//int idx = (nSwitch-1)+(nDayTypes-1)*(nSwitch)+(nRoomTypes-1)*(nDayTypes)*(nSwitch)+(nSetTypes-1)*(nRoomTypes)*(nDayTypes)*(nSwitch);
								int idx = iSwitch+iDayType*(nSwitch)+iRoomType*(nDayTypes)*(nSwitch)+iSet*(nRoomTypes)*(nDayTypes)*(nSwitch);
								TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp = temps[idx].as<double>();
							}
						}
					}
				}
				return 1;
			}
		}
	}
	return 0;
}

void cRooms::getRooms( JsonObject& root )
{
	JsonArray& RoomsTypes = root.createNestedArray("RoomTypes");
	JsonArray& RoomsTemps = root.createNestedArray("RoomTemps");
	
	for (int i = 0; i<nRooms;i++){
		RoomsTypes.add(Room[i].RoomType);
	}
	
	for (int i = 0; i<nRoomTypes;i++){
		RoomsTemps.add(RoomTemps[i]);
	}
	
	root["SetType"] = SetType;
}

int cRooms::setRooms( JsonObject& root )
{
	int posReturn = 0;
	int fail = 0;
	
	if(root.containsKey("RoomTypes")) {
		if(root["RoomTypes"].is<JsonArray&>()){
			JsonArray& RoomsTypes = root["RoomTypes"];
			if (RoomsTypes.size()==nRooms) {
				for (int i = 0; i<nRooms;i++){
					if ( RoomsTypes[i].is<long>())	Room[i].RoomType = static_cast<RoomTypes>(RoomsTypes[i].as<long>());
					else fail=1;
				}
			}
			else fail=1;
		}
		else fail=1;
		posReturn++;
	}
	
	if(root.containsKey("RoomTemps")) {
		if(root["RoomTemps"].is<JsonArray&>()){
			JsonArray& RoomsTemps = root["RoomTemps"];
			if (RoomsTemps.size()==nRoomTypes) {
				for (int i = 0; i<nRoomTypes;i++){
					if ( RoomsTemps[i].is<double>())	RoomTemps[i] = RoomsTemps[i].as<double>();
					else fail=1;
				}
			}
			else fail=1;
		}
		else fail=1;
		posReturn++;
	}
	
	if(root.containsKey("SetType")) {
		if(root["SetType"].is<long>()) {
			SetType =  static_cast<SetTypes>(root["SetType"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if (fail == 0) { // If all three parameter objects were successfully filled
		return posReturn;
	}
	else {
		return 0;
	}
}

void cRooms::getData( JsonObject& root )
{
	JsonArray& RoomsIsTemps = root.createNestedArray("RoomIs");
	JsonArray& RoomsSPTemps = root.createNestedArray("RoomSP");
	
	for (int i = 0; i<nRooms;i++){
		RoomsIsTemps.add(Room[i].IsTemp.get());
		RoomsSPTemps.add(Room[i].getSpTemp());
	}
	
	root["TempOutside"] = TempOutside.get();
	root["RoomNeed"] = need();
	root["SpHeating"] = getSpHeating();
	
	root["IsTempHeatingLead"] = IsTempHeatingLead.get();
	root["IsTempHeatingReturn"] = IsTempHeatingReturn.get();
	root["RoomsPump"] = Pump.getPower();
}