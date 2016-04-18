#include "cRooms.h"

cRooms::cRooms( void ):
IsTempHeatingLead((&MPNumSys[0]),(&MPChanSys[idxTempHeatingLead]),(&SysTempOffset[idxTempHeatingLead])),
IsTempHeatingReturn((&MPNumSys[0]),(&MPChanSys[idxTempHeatingReturn]),(&SysTempOffset[idxTempHeatingReturn])),
TempOutside((&MPNumSys[0]),(&MPChanSys[idxTempOutside]),(&SysTempOffset[idxTempOutside]),TRoomInit, AlphaTRoom),
Pump(PinPumpHeating,0.5, 0.0, 0.0, DIRECT, 0.0),
Mixer(PinMixerOpen,PinMixerClose, 0.1, 0.0008, 3.9, DIRECT),
HeatingPeriod(129600000), // 36(h)*60(min/h)*60(s/min)*1000(ms/s)
PWM(600000) // 10(min)*60(s/min)*1000(ms/s) = 600000 ; 10 minute room heating interval
{
	SetType = Normal;
	// Initialize PID controllers for pumps
	Pump.SetOutputLimits(0.3, 1.0);
	Mixer.SetOutputLimits(-1.0, 1.0);
	Mixer.SetSampleTime(5000);
	
	MaxNeed =0;
	dMaxSp = 0;
	
	initDefaultSetpoint();
}

void cRooms::initDefaultSetpoint()
{
//   TempOffsetSchedule =20.0;
	MasterSpTemps[Living] = 20.2;//15;//
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
	switchtime[0].set(0,5,45,0);
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
    // Restart Heating Period timer
    HeatingPeriod.restart();
	}
	else
	{
		// Stop Pump and Mixer Heating
		Pump.run(0.0);
		Mixer.run(0.0);
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
	// Reset Values
        boolean active = false;
        
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
                // Check for rooms needing heat
                active = active||Room[i].getValve();
	}
	
	return active;
}

double cRooms::getSpHeating(void)
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
        int fail=0;
        int succes=0;
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
								int idx = iSwitch+iDayType*(nSwitch)+iRoomType*(nDayTypes)*(nSwitch)+iSet*(nRoomTypes)*(nDayTypes)*(nSwitch);
                                                                if (times[idx].is<long>()) {
                                                                  unsigned long oldTime = TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time.totalseconds(); 
                                                                  unsigned long newTime = times[idx].as<long>();
                                                                  if((newTime>=0)&&(24*60*60-newTime>=0)&&(newTime!=oldTime)){
                                                                      TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].time.set(newTime);
                                                                      succes=1;
                                                                  }
                                                                }
                                                                else fail=1;
							}
						}
					}
				}
			}
		}
	}
 
	return (!fail)&&(succes);
}

void cRooms::getOffsetTemp( JsonObject& root )
{
	JsonArray&  temps  = root.createNestedArray("RTo");
	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSetTypes; iSet++)
	{
		for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, bath, side)
		{
			for(int iDayType = 0; iDayType<nDayTypes; iDayType++) // Weekend, Workday
			{
				for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++) // 4 switch times
				{
					temps.add(TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp);
				}
			}
		}
	}
}

int cRooms::setOffsetTemp( JsonObject& root )
{
        int fail=0;
        int succes=0;
	if(root.containsKey("RTo")) {
		if(root["RTo"].is<JsonArray&>()){
			JsonArray& temps = root["RTo"];
			
			if (temps.size()==(nSetTypes*nRoomTypes*nDayTypes*nSwitch))
			{
				for(int iSet = 0; iSet<nSetTypes; iSet++) // Normal
				{
					for(int iRoomType = 0; iRoomType<nRoomTypes; iRoomType++) // (Living, sleeping, hallway, bath, side)
					{
						for(int iDayType = 0; iDayType<nDayTypes; iDayType++) // Weekend, Workday
						{
							for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++) // 4 switch times
							{
								// Last iteration
								int idx = iSwitch+iDayType*(nSwitch)+iRoomType*(nDayTypes)*(nSwitch)+iSet*(nRoomTypes)*(nDayTypes)*(nSwitch);
               if (temps[idx].is<double>()){
                                                                double temp_ =  temps[idx].as<double>();
                                                                if((temp_>-15.0)&&(temp_<15.0)&&(temp_!=TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp)){
                                                                    TempOffsetSchedule[iSet][iRoomType][iDayType][iSwitch].temp = temp_;
                                                                    succes = 1;
                                                                }
               }
               else fail=1;
							}
						}
					}
				}
			}
		}
	}
 
	return ((!fail)&&(succes));
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
					if ( RoomsTypes[i].is<long>()){
                                            RoomTypes RoomType_ = static_cast<RoomTypes>(RoomsTypes[i].as<long>());
                                            if((RoomType_>=0)&&(RoomType_<nRoomTypes)&&(RoomType_!=Room[i].RoomType)){
                                                Room[i].RoomType = RoomType_;
                                            }
                                        }
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
					if ( RoomsTemps[i].is<double>()){
                                            double MasterSpTemps_ = RoomsTemps[i].as<double>();
                                            if ((MasterSpTemps_>10.0)&&(MasterSpTemps_<25.0)&&(MasterSpTemps_!=MasterSpTemps[i])){
                                                MasterSpTemps[i] = MasterSpTemps_;
                                            }
                                        }
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
			SetTypes SetType_ =  static_cast<SetTypes>(root["SetType"].as<long>());
                        if((SetType_>=0)&&(SetType_<nSetTypes)&&(SetType_!=SetType)){
                            SetType = SetType_;
                            posReturn++;
                        }
		}
		else fail=1;
	}
	
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
        JsonArray& RoomsNeeds = root.createNestedArray("Rneed");
	
	for (int i = 0; i<nRooms;i++){
		RoomsIsTemps.add(Room[i].IsTemp.get());
		RoomsSPTemps.add(Room[i].getSpTemp());
                RoomsNeeds.add(Room[i].getNeed());
	}
	
	root["Toutside"] = TempOutside.get();
	root["Rn"] = static_cast<int>( active());
	root["RTsHeating"] = getSpHeating();
	
	root["RTitoR"] = IsTempHeatingLead.get();
	root["RTitoSys"] = IsTempHeatingReturn.get();
	root["RP"] = Pump.get();
	root["RM"] = Mixer.get();
}
