#include "cRoom.h"

cRoom::cRoom( int RoomNumber_ )
//:pid(0.1, 0, 0, DIRECT)
{
	RoomType = Living;
	SpTemp = 20.0;
	init(RoomNumber_);
}

void cRoom::init( int RoomNumber_ )
{
	setRoomNumber(RoomNumber_);
	//Set the pin of the valve according to pinout scheme.
	Valve.setPinOpen(RoomValvePin[RoomNumber_]);
	
	//pid.SetOutputLimits(0, 1);
}

void cRoom::setRoomNumber(int RoomNumber_)
{
	// Set RoomNumber
	RoomNumber = RoomNumber_;
	
// 	//**********************
// 	// Calculate pinout
// 	int MultiplexNumber = 0;
// 	int MultiplexChannelIs = 2*(RoomNumber+1);
// 	if (MultiplexChannelIs>16)
// 	{
// 		MultiplexChannelIs = MultiplexChannelIs -16;
// 		MultiplexNumber =1;
// 	}
// 	// Adaption to multiplexer pinout
// 	MultiplexChannelIs = 16-MultiplexChannelIs;
// 	int MultiplexChannelSp = MultiplexChannelIs +1;
// 	//**********************
	
	IsTemp.set(MultiplexNumberRooms[RoomNumber],MultiplexChannelRoomsIs[RoomNumber],RoomIsOffset[RoomNumber]);
	SpTempOverride.set(MultiplexNumberRooms[RoomNumber],MultiplexChannelRoomsSp[RoomNumber],RoomSpOffset[RoomNumber]);
}

double cRoom::getNeed(void)
{
	// Compute need
	//double _need = pid.run(getSpTemp(),IsTemp.get());
	double _need = getSpTemp()-IsTemp.get();
	// Open Valve if heat is needed
	Valve.set((_need>0));
	
	return _need;
}


///////////////////////////////////////////////
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
	
	// Initialize room numbers and Pins
	for(int i = 0; i<16; i++)
	{
		// The rooms know their multiplexers and pin out by the room number
		Room[i].init(i);
	}
	
	initDefaultSetpoint();
	initDefaultRoomtypes();
	
}

void cRooms::initDefaultRoomtypes()
{
	Room[0].RoomType = Side;
	Room[1].RoomType = Side;
	Room[2].RoomType = Side;
	Room[3].RoomType = Side;
	Room[4].RoomType = Living;
	Room[5].RoomType = Living;
	Room[6].RoomType = Living;
	Room[7].RoomType = Hallway;
	Room[8].RoomType = Hallway;
	Room[9].RoomType = Hallway;
	Room[10].RoomType = Sleeping;
	Room[11].RoomType = Sleeping;
	Room[12].RoomType = Bath;
	Room[13].RoomType = Side;
	Room[14].RoomType = Side;
	Room[15].RoomType = Hallway;
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
	for(int iSet = 0; iSet<nSet; iSet++)
	{
		for(int iDayType = 0; iDayType<nDayType; iDayType++)
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


void cRooms::ChargeRooms( boolean bneedChargeRooms, boolean BoilerCharges )
{
	_dSpTempHeatingLead = getSpHeating();
	_dIsTempHeatingLead = IsTempHeatingLead.get();
	_dSpTempHeatingReturn = _dIsTempHeatingLead-DiffTempHeatingLeadReturn;
	_dIsTempHeatingReturn = IsTempHeatingReturn.get();
	
	if (bneedChargeRooms)
	{
		// Run Mixer and PID
		Mixer.run(PIDMixer.run(_dSpTempHeatingLead, _dIsTempHeatingLead));
		
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
			Pump.setPower(PIDPump.run(_dSpTempHeatingReturn, _dIsTempHeatingReturn));
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
	double dneedChargeRooms = 0.0;
	dMaxSp = 0.0;
	dMaxDiff = 0.0;
	
	DayTypes DayType = getDayType( TimeNow.dayOfWeek() );
	
	TimeSpan rel;
	rel.set(0, TimeNow.hour(), TimeNow.minute(), 0);
	
	// Apply heating set point temperature schedule
	for(int i = 0; i<nRooms; i++)
	{
		
		// Check if manual override is valid
		double MasterSPTemp = 0.0;
		double SPTemp = 0.0;
		if((Room[i].SpTempOverride.get()<TempRoomHigh)&&(Room[i].SpTempOverride.get()>TempRoomLow))
		{
			MasterSPTemp = Room[i].SpTempOverride.get();
		}
		else
		{
			MasterSPTemp = RoomTemps[(Room[i].RoomType)];
		}
		
		for(int iSwitch=0; iSwitch<nSwitch; iSwitch++)
		{
			if (TempOffsetSchedule[SetType][Room[i].RoomType][DayType][iSwitch].time.totalseconds() < rel.totalseconds())
			{
				SPTemp = MasterSPTemp + TempOffsetSchedule[SetType][Room[i].RoomType][DayType][iSwitch].temp;
				// Apply offset according to schedule
				
				if((SPTemp<TempRoomHigh)&&(SPTemp>TempRoomLow))	Room[i].setSpTemp(SPTemp);
				else if (SPTemp<=TempRoomLow) Room[i].setSpTemp(TempRoomLow);
				else if (SPTemp>=TempRoomHigh)  Room[i].setSpTemp(TempRoomHigh);
			}

		}
	}
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
		
		// Store maximum set point
		if (Room[i].getSpTemp()>dMaxSp)
		{
			dMaxSp = Room[i].getSpTemp();
		}
		// Store maximum sp-is difference
		if ((Room[i].getSpTemp() - Room[i].getIsTemp()) > dMaxDiff)
		{
			dMaxDiff = Room[i].getSpTemp() - Room[i].getIsTemp();
		}
		// Check for rooms needing heat
		if(Room[i].getNeed()>dneedChargeRooms)
		{
			dneedChargeRooms = Room[i].getNeed();
		}
	}
	
	return (dneedChargeRooms>0);
}

double cRooms::getSpHeating(void)
{
	double t1;
	double t2;
	double swhk;
	double swhkLimit;
	
	need();
	
	t1 = (TempOutside.get()/(320-4*TempOutside.get()));
	t2 = pow(dMaxSp,t1);
	swhk = 0.55*dsteil*t2*(-TempOutside.get()+20)*2+dMaxSp+dkh+dMaxDiff*dverst;
	swhkLimit = min(max(swhk,dminvl),dmaxvl);
	return swhkLimit;
}

DayTypes cRooms::getDayType( uint8_t day  )
{
	if ((day == 0)||(day == 6) )
	{
		DayType = Weekend;
	}
	else
	{
		DayType = Workday;
	}
	return DayType;
}

void cRooms::getOffsetTime( JsonObject& root )
{
	JsonArray&  times  = root.createNestedArray("times");
	// Iterate over all sets (At home, away)
	for(int iSet = 0; iSet<nSet; iSet++)
	{
		for(int iRoomType = 0; iRoomType<nRoomType; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
		{
			for(int iDayType = 0; iDayType<nDayType; iDayType++)
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
			
			if (times.size()==(nSet*nRoomType*nDayType*nSwitch))
			{
				for(int iSet = 0; iSet<nSet; iSet++)
				{
					for(int iRoomType = 0; iRoomType<nRoomType; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
					{
						for(int iDayType = 0; iDayType<nDayType; iDayType++)
						{
							for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
							{
								// Last iteration
								//int idx = (nSwitch-1)+(nDayType-1)*(nSwitch)+(nRoomType-1)*(nDayType)*(nSwitch)+(nSet-1)*(nRoomType)*(nDayType)*(nSwitch);
								int idx = iSwitch+iDayType*(nSwitch)+iRoomType*(nDayType)*(nSwitch)+iSet*(nRoomType)*(nDayType)*(nSwitch);
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
	for(int iSet = 0; iSet<nSet; iSet++)
	{
		for(int iRoomType = 0; iRoomType<nRoomType; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
		{
			for(int iDayType = 0; iDayType<nDayType; iDayType++)
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
			
			if (temps.size()==(nSet*nRoomType*nDayType*nSwitch))
			{
				for(int iSet = 0; iSet<nSet; iSet++)
				{
					for(int iRoomType = 0; iRoomType<nRoomType; iRoomType++) // Iterate over all Roomtypes (Living, sleeping, hallway, side)
					{
						for(int iDayType = 0; iDayType<nDayType; iDayType++)
						{
							for(int iSwitch = 0; iSwitch<nSwitch; iSwitch++)
							{
								// Last iteration
								//int idx = (nSwitch-1)+(nDayType-1)*(nSwitch)+(nRoomType-1)*(nDayType)*(nSwitch)+(nSet-1)*(nRoomType)*(nDayType)*(nSwitch);
								int idx = iSwitch+iDayType*(nSwitch)+iRoomType*(nDayType)*(nSwitch)+iSet*(nRoomType)*(nDayType)*(nSwitch);
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
	
	for (int i = 0; i<nRoomType;i++){
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
			if (RoomsTemps.size()==nRoomType) {
				for (int i = 0; i<nRoomType;i++){
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
		RoomsIsTemps.add(Room[i].getIsTemp());
		RoomsSPTemps.add(Room[i].getSpTemp());
	}
	
	root["TempOutside"] = TempOutside.get();
	root["RoomNeed"] = need();
	root["SpHeating"] = getSpHeating();
	
	root["IsTempHeatingLead"] = IsTempHeatingLead.get();
	root["IsTempHeatingReturn"] = IsTempHeatingReturn.get();
	root["RoomsPump"] = Pump.getPower();
}