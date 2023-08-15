#include "cRooms.h"

cRooms::cRooms( cWarmWater* WarmWater_ ):
IsTempHeatingLead((&MPNumSys[0]),(&MPChanSys[idxTempHeatingLead]),(&SysTempOffset[idxTempHeatingLead])),
IsTempHeatingReturn((&MPNumSys[0]),(&MPChanSys[idxTempHeatingReturn]),(&SysTempOffset[idxTempHeatingReturn])),
TempOutside((&MPNumSys[0]),(&MPChanSys[idxTempOutside]),(&SysTempOffset[idxTempOutside]),TRoomInit, AlphaTRoom),
Pump(PinPumpHeating,0.5, 0.0, 0.0, DIRECT, 0.0),
Mixer(PinMixerOpen,PinMixerClose, 0.09, 0.0, 2.3, DIRECT),
PWM(30000) // 0.25(min)*60(s/min)*1000(ms/s) = 15000 ; 15 seconds room heating interval
//PWM(120000) // 2(min)*60(s/min)*1000(ms/s) = 120000 ; 2 minutes room heating interval
{
	SetType = Normal;
	// Initialize PID controllers for pumps
	Pump.SetOutputLimits(0.3, 1.0);
	Mixer.SetOutputLimits(-1.0, 1.0);
	Mixer.SetSampleTime(1000);
	
	MaxNeed = 0;
	dMaxSp = 0;
	
	WarmWater = WarmWater_;
	
	initDefaultSetpoint();
}

void cRooms::initDefaultSetpoint()
{
//   TempOffsetSchedule =20.0;

	MasterSpTemps[Living] = 21.2;
	MasterSpTemps[Sleeping] = 20.0;
	MasterSpTemps[Hallway] = 19.0;
	MasterSpTemps[Bath] = 22.0;
	MasterSpTemps[Side] = 13.8;
	MasterSpTemps[Trim] = 18.0;
	MasterSpTemps[Work] = 20.3;
        
	double temp[nSwitch];
	temp[0] = 0.0;
	temp[1] = -1.2;
	temp[2] = 0.0;
	temp[3] = -1.2;

	time_t switchtime[nSwitch];
	switchtime[0] = hoursToTime_t(5) + minutesToTime_t(45);
	switchtime[1] = hoursToTime_t(8);
	switchtime[2] = hoursToTime_t(18);
	switchtime[3] = hoursToTime_t(21);

	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSetTypes; iSet++)
	{
		for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
		{
			for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
			{
				// Discriminate between Rooms that do not reduce temperature during the day and those which do

				if((iSwitch==1)){
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].temp = temp[iSwitch-1];
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].temp = temp[iSwitch-1];
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].temp = temp[iSwitch-1];
					TempOffsetSchedule[iSet][Work][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Work][iDayType][iSwitch].temp = temp[iSwitch-1];				
					
				}
				else {
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Living][iDayType][iSwitch].temp = temp[iSwitch];
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Hallway][iDayType][iSwitch].temp = temp[iSwitch];
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Side][iDayType][iSwitch].temp = temp[iSwitch];
					TempOffsetSchedule[iSet][Work][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Work][iDayType][iSwitch].temp = temp[iSwitch];					
				}
				
				if((iSwitch==2)){
					TempOffsetSchedule[iSet][Trim][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Trim][iDayType][iSwitch].temp = temp[iSwitch-1];					
				}
				else {
					TempOffsetSchedule[iSet][Trim][iDayType][iSwitch].time = switchtime[iSwitch];
					TempOffsetSchedule[iSet][Trim][iDayType][iSwitch].temp = temp[iSwitch];			
				}
				
				TempOffsetSchedule[iSet][Sleeping][iDayType][iSwitch].time = switchtime[iSwitch];
				TempOffsetSchedule[iSet][Sleeping][iDayType][iSwitch].temp = temp[iSwitch];
				TempOffsetSchedule[iSet][Bath][iDayType][iSwitch].time = switchtime[iSwitch];
				TempOffsetSchedule[iSet][Bath][iDayType][iSwitch].temp = temp[iSwitch];
			}
		}
	}
}


void cRooms::charge( boolean ChargeRooms , boolean bControlbyMixer)
{	
	if (ChargeRooms)
	{
		// Run Pump and Mixer
		if (bControlbyMixer)
			Mixer.run(SpTemp(), IsTempHeatingLead.getRaw());
		else
			Mixer.run(1.0);

		// New intelligent energy saving Pump
		Pump.run(1.0);

	}
	else
	{
		// Stop Pump and Mixer Heating
		Pump.run(0.0);
		Mixer.run(-1.0);
	}
}

double cRooms::getNeed(void)
{
    dMaxSp = 0.0;
	MaxNeed = 0.0;
        
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
		// Store maximum set point
        dMaxSp = max(dMaxSp, Room[i].getSpTemp());
		// Store maximum sp-is difference
        MaxNeed = max(MaxNeed, Room[i].getNeed());
	}
    
        return MaxNeed;
}

boolean cRooms::active(void)
{
	// introduce FlipFlop for Hysteresis of MaxNeed
	if ((bAct == false) && (MaxNeed > 0.8))
	bAct = true;
	if ((bAct == true) && (MaxNeed < 0.6)) // was 0.35
	bAct = false;
	
	return bAct;
}

double cRooms::SpTemp(void)
{
	// Update MaxNeed and dMaxSp
	getNeed();
	
	double t1 = (TempOutside.get()/(320-4*TempOutside.get()));
	double t2 = pow(dMaxSp,t1);
	double swhk = 0.55*dsteil*t2*(-TempOutside.get()+20)*2+dMaxSp+dkh+MaxNeed*dverst;
	double swhkLimit = min(max(swhk,dminvl),dmaxvl);
	
	return swhkLimit;
}

void cRooms::getOffsetTime( JsonObject& root )
{
	JsonArray&  times  = root.createNestedArray("Rt");
	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSetTypes; iSet++)
	{
		for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side, trim)
		{
			for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
			{
				for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
				{
					times.add(TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time);
				}
			}
		}
	}
}

int cRooms::setOffsetTime( JsonObject& root )
{
        int fail=0;
        int succes=0;
	if(root.containsKey("Rt")) {
		if(root["Rt"].is<JsonArray&>()){
			JsonArray& times = root["Rt"];
			
			if (times.size()==(nSetTypes*nRoomTypes*nDayTypes*nSwitch))
			{
				for(int iSet = 0; iSet<nSetTypes; iSet++)
				{
					for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side, trim)
					{
						for(int iDayType = 0; iDayType<nDayTypes; iDayType++)
						{
							for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
							{
								int idx = iSwitch + iDayType * (nSwitch) + iRoomType * (nDayTypes) * (nSwitch) + iSet * (nRoomTypes) * (nDayTypes) * (nSwitch);
								if (times[idx].is<long>())
								{
									unsigned long oldTime = TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time;
									unsigned long newTime = times[idx].as<unsigned long>();
									if ((newTime >= (unsigned long)0) && ((unsigned long)24 * (unsigned long)60 * (unsigned long)60 - newTime >= (unsigned long)0) && (newTime != oldTime))
									{
										TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time = newTime;
										succes = 1;
									}
								}
								else
									fail = 1;
							}
						}
					}
				}
			}
		}
	}
 
	return (!fail)&&(succes);
}

void cRooms::getOffsetTemp(JsonObject& root) {
	JsonArray& temps = root.createNestedArray("RTo");
	for (int iSet = 0; iSet < nSetTypes; iSet++) {                      // Iterate over all sets (At home, away)
		for (int iRoomType = 0; iRoomType < nRoomTypes; iRoomType++) {  // Iterate over all Roomtypes (Living, sleeping, hallway, bath, side, trim)
			for (int iDayType = 0; iDayType < nDayTypes; iDayType++) {  // Weekend, Workday
				for (int iSwitch = 0; iSwitch < nSwitch; iSwitch++) {   // 4 switch times
					temps.add(TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp);
				}
			}
		}
	}
}

int cRooms::setOffsetTemp(JsonObject& root) {
	int fail = 0;
	int succes = 0;
	if (root.containsKey("RTo")) {
		if (root["RTo"].is<JsonArray&>()) {
			JsonArray& temps = root["RTo"];
			if (temps.size() == (nSetTypes * nRoomTypes * nDayTypes * nSwitch)) {
				for (int iSet = 0; iSet < nSetTypes; iSet++) {                      // Iterate over all sets (At home, away)
					for (int iRoomType = 0; iRoomType < nRoomTypes; iRoomType++) {  // (Living, sleeping, hallway, bath, side, trim)
						for (int iDayType = 0; iDayType < nDayTypes; iDayType++) {  // Weekend, Workday
							for (int iSwitch = 0; iSwitch < nSwitch; iSwitch++) {   // 4 switch times
								// Last iteration
								int idx = iSwitch + iDayType * (nSwitch) + iRoomType * (nDayTypes) * (nSwitch) + iSet * (nRoomTypes) * (nDayTypes) * (nSwitch);
								if (temps[idx].is<double>()) {
									double temp_ = temps[idx].as<double>();
									if ((temp_ > -15.0) && (temp_ < 15.0) && (temp_ != TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp)) {
										TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp = temp_;
										succes = 1;
									}
								} else
									fail = 1;
							}
						}
					}
				}
			}
		}
	}
	return ((!fail) && (succes));
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

int cRooms::setRooms(JsonObject& root) {
	int posReturn = 0;
	int fail = 0;

	if (root.containsKey("RTypes")) {
		if (root["RTypes"].is<JsonArray&>()) {
			JsonArray& RoomsTypes = root["RTypes"];
			if (RoomsTypes.size() == nRooms) {
				for (int i = 0; i < nRooms; i++) {
					if (RoomsTypes[i].is<long>()) {
						RoomTypes RoomType_ = static_cast<RoomTypes>(RoomsTypes[i].as<long>());
						if ((RoomType_ >= 0) && (RoomType_ < nRoomTypes) && (RoomType_ != Room[i].RoomType)) {
							Room[i].RoomType = RoomType_;
						}
					} else
						fail = 1;
				}
			} else
				fail = 1;
		} else
			fail = 1;
		posReturn++;
	}

	if (root.containsKey("RTs")) {
		if (root["RTs"].is<JsonArray&>()) {
			JsonArray& RoomsTemps = root["RTs"];
			if (RoomsTemps.size() == nRoomTypes) {
				for (int i = 0; i < nRoomTypes; i++) {
					if (RoomsTemps[i].is<double>()) {
						double MasterSpTemps_ = RoomsTemps[i].as<double>();
						if ((MasterSpTemps_ > 10.0) && (MasterSpTemps_ < 25.0) && (MasterSpTemps_ != MasterSpTemps[i])) {
							MasterSpTemps[i] = MasterSpTemps_;
						}
					} else
						fail = 1;
				}
			} else
				fail = 1;
		} else
			fail = 1;
		posReturn++;
	}

	if (root.containsKey("SetType")) {
		if (root["SetType"].is<long>()) {
			SetTypes SetType_ = static_cast<SetTypes>(root["SetType"].as<long>());
			if ((SetType_ >= 0) && (SetType_ < nSetTypes) && (SetType_ != SetType)) {
				SetType = SetType_;
				posReturn++;
			}
		} else
			fail = 1;
	}

	if (fail == 0) {  // If all three parameter objects were successfully filled
		return posReturn;
	} else {
		return 0;
	}
}

void cRooms::getData( JsonObject& root )
{
	JsonArray& RoomsIsTemps = root.createNestedArray("RTi");
	JsonArray& RoomsSPTemps = root.createNestedArray("RTs");
        JsonArray& RoomsNeeds = root.createNestedArray("Rneed");
	
	for (int i = 0; i<nRooms;i++){
		RoomsIsTemps.add(Room[i].IsTemp.get());
		RoomsSPTemps.add(Room[i].getSpTemp());
		RoomsNeeds.add(Room[i].getNeed());
	}
	
	root["Toutside"] = TempOutside.get();
	root["Rn"] = static_cast<int>( active());
	root["RTsHeating"] = SpTemp();
	
	root["RTitoR"] = IsTempHeatingLead.get();
	root["RTitoSys"] = IsTempHeatingReturn.get();
	root["RP"] = Pump.get();
	root["RM"] = Mixer.get();
}
