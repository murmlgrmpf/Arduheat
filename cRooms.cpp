#include "cRooms.h"

cRooms::cRooms( void ):
IsTempHeatingLead((&MPNumSys[0]),(&MPChanSys[idxTempHeatingLead]),(&SysTempOffset[idxTempHeatingLead])),
IsTempHeatingReturn((&MPNumSys[0]),(&MPChanSys[idxTempHeatingReturn]),(&SysTempOffset[idxTempHeatingReturn])),
TempOutside((&MPNumSys[0]),(&MPChanSys[idxTempOutside]),(&SysTempOffset[idxTempOutside])),
Pump(PinPumpHeating,0.5, 0.0, 0.0, DIRECT, 0.0),
Mixer(PinMixerOpen,PinMixerClose, 0.2, 0.0, 2.0, DIRECT)
{
	SetType = Normal;
	// Initialize PID controllers for pumps
	Pump.SetOutputLimits(0.3, 1.0);
	Mixer.SetOutputLimits(-1.0, 1.0);
	Mixer.SetSampleTime(500);
	
	dMaxDiff =0;
	dMaxSp = 0;
	
	initDefaultSetpoint();
}

void cRooms::initDefaultSetpoint()
{
//   TempOffsetSchedule =20.0;
	MasterSpTemps[Living] = 20.0;//15;//
	MasterSpTemps[Sleeping] =20.0;//15;//
	MasterSpTemps[Hallway] = 19.0;//15;//
	MasterSpTemps[Bath] = 22.0;//15;//
	MasterSpTemps[Side] = 15.0;//15;//
	
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


void cRooms::ChargeRooms( boolean ChargeRooms )
{
	double SpTempHeatingReturn = getSpHeating()-DiffTempHeatingLeadReturn;
	
	if (ChargeRooms)
	{
		// Run Pump and Mixer
		Mixer.run(getSpHeating(), IsTempHeatingLead.get());
		Pump.run(SpTempHeatingReturn, IsTempHeatingReturn.get());
	}
	else
	{
		// Stop Pump Heating
		Pump.run(0.0);
		// Close Mixer
		Mixer.run(-1.0);
	}
}

boolean cRooms::need(void)
{
	// Reset Values
	dMaxSp = 0.0;
	dMaxDiff = -10.0;
	
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
		// Store maximum set point
		if (Room[i].getSpTemp()>dMaxSp)
			dMaxSp = Room[i].getSpTemp();
		// Store maximum sp-is difference
// 		if ((Room[i].getSpTemp() - Room[i].IsTemp.get()) > dMaxDiff)
// 			dMaxDiff = Room[i].getSpTemp() - Room[i].IsTemp.get();
		// Check for rooms needing heat
		if (Room[i].getNeed()>dMaxDiff)
			dMaxDiff = Room[i].getNeed();
	}
	
	// Hysteresis for charging the rooms
	if (dMaxDiff-RoomMargin > 0) needCharge = true;
	if (dMaxDiff+RoomMargin < 0) needCharge = false;
	
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
	JsonArray&  times  = root.createNestedArray("Rt");
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
	if(root.containsKey("Rt")) {
		if(root["Rt"].is<JsonArray&>()){
			JsonArray& times = root["Rt"];
			
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
	JsonArray&  temps  = root.createNestedArray("RTs");
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
	if(root.containsKey("RTs")) {
		if(root["RTs"].is<JsonArray&>()){
			JsonArray& temps = root["RTs"];
			
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
	JsonArray& RoomsTypes = root.createNestedArray("RTypes");
	JsonArray& RoomsTemps = root.createNestedArray("RTs");
	
	for (int i = 0; i<nRooms;i++){
		RoomsTypes.add(Room[i].RoomType);
	}
	
	for (int i = 0; i<nRoomTypes;i++){
		RoomsTemps.add(MasterSpTemps[i]);
	}
	
	root["SetType"] = SetType;
}

int cRooms::setRooms( JsonObject& root )
{
	int posReturn = 0;
	int fail = 0;
	
	if(root.containsKey("RTypes")) {
		if(root["RTypes"].is<JsonArray&>()){
			JsonArray& RoomsTypes = root["RTypes"];
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
	
	if(root.containsKey("RTs")) {
		if(root["RTs"].is<JsonArray&>()){
			JsonArray& RoomsTemps = root["RTs"];
			if (RoomsTemps.size()==nRoomTypes) {
				for (int i = 0; i<nRoomTypes;i++){
					if ( RoomsTemps[i].is<double>())	MasterSpTemps[i] = RoomsTemps[i].as<double>();
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
	JsonArray& RoomsIsTemps = root.createNestedArray("RTi");
	JsonArray& RoomsSPTemps = root.createNestedArray("RTs");
	
	for (int i = 0; i<nRooms;i++){
		RoomsIsTemps.add(Room[i].IsTemp.get());
		RoomsSPTemps.add(Room[i].getSpTemp());
	}
	
	root["Toutside"] = TempOutside.get();
	root["Rn"] = need();
	root["RTsHeating"] = getSpHeating();
	
	root["RTitoR"] = IsTempHeatingLead.get();
	root["RTitoSys"] = IsTempHeatingReturn.get();
	root["RP"] = Pump.get();
	root["RM"] = Mixer.get();
}