#include "cRoom.h"
#include "cRooms.h"

/// Creates a room.
/** As the rooms get created in an array, only the default constructor can be used cRoom(void) */
cRoom::cRoom(int RoomNumber , cRooms* Rooms_ , double p , double i , double d):
PID(IsTemp.getPtr(), &need, &spT, p, i, d, DIRECT),
IsTemp(&MultiplexNumberRooms[RoomNumber], &MultiplexChannelRoomsIs[RoomNumber], &RoomIsOffset[RoomNumber], TRoomInit, AlphaTRoom),
SpTempOverride(&MultiplexNumberRooms[RoomNumber], &MultiplexChannelRoomsSp[RoomNumber], &RoomSpOffset[RoomNumber], TRoomInit, AlphaTRoom),
Valve(&RoomValvePin[RoomNumber])
{
	RoomType = static_cast<RoomTypes>(pgm_read_word(&DefaultRoomType[RoomNumber]));
	Rooms = Rooms_;
        
        spT=0.0;
        need=0.0;
        IsTemp.get();
        
        SetSampleTime(32767); // 15(min)*60(s)*1000(ms/s)
        SetMode(AUTOMATIC);
        SetOutputLimits(0.0, 1.0);
}

double cRoom::getSpTemp(void)
{
	DayTypes DayType = getDayType( TimeNow.dayOfTheWeek() );
	
	TimeSpan Now(0, TimeNow.hour(), TimeNow.minute(), 0);
	
	double MasterSPTemp = 0.0;
	// Check if manual override is valid
	if((SpTempOverride.get()<TempRoomHigh)&&(SpTempOverride.get()>TempRoomLow))
		MasterSPTemp = SpTempOverride.get();
	else
		MasterSPTemp = Rooms->MasterSpTemps[(RoomType)];
	
	double SpTemp = MasterSPTemp + Rooms->TempOffsetSchedule[Rooms->SetType][RoomType][DayType][nSwitch-1].temp;
	
	for(int iSwitch=0; iSwitch<nSwitch; iSwitch++)
	{	// Apply offset according to schedule if time of schedule is later than Lastrent and sooner than 
		if (Rooms->TempOffsetSchedule[Rooms->SetType][RoomType][DayType][iSwitch].time.totalseconds() < Now.totalseconds())
			SpTemp = MasterSPTemp + Rooms->TempOffsetSchedule[Rooms->SetType][RoomType][DayType][iSwitch].temp;
	}
	
    // Reduce setpoint to minimum room temperature if
    // no warm water is used and therefore no one is at home
	if (!Rooms->WarmWater->Period.get()) {SpTemp = 15.0;}
	//Check Limits
	SpTemp = min(max(SpTemp,TempRoomLow),TempRoomHigh);
	return SpTemp;
}


double cRoom::getNeed(void)
{
	spT = getSpTemp();
        IsTemp.get();
        Compute(); // Execute PID
	// Open Valve if heat is needed
		Valve.set(Rooms->PWM.get(need) && Rooms->bAct);
        
        return need;
}

const float cRoom::RoomIsOffset[] = {-2.69, -1.89, -1.21, 0.12, -0.8, 1.26, -2.64, -2.82, -1.11, -0.93, -2.3, -0.7, 0.56, -1.93, -1.13, -0.41}; //InputBoard1
const float cRoom::RoomSpOffset[] = {0.16, -1.37, -0.08, -0.34, -1.75, -0.31, 2.04, 0.02, -1.47, -0.36, 0.24, 0.21, 0.83, -0.43, 0.13, -0.61}; //InputBoard1

//const float cRoom::RoomIsOffset[] = {-2.49, -1.29, -0.61, 0.52, -0.6, 1.06, -2.14, -2.02, -1.21, -0.03, -0.8, -1.0, -0.14, -1.73, -1.13, -0.31}; //InputBoard1_old
//const float cRoom::RoomSpOffset[] = {0.56, -1.47, -0.58, -1.04, -1.65, -0.81, 2.24, -0.58, -1.97, -0.86, -0.66, -0.09, 0.23, -0.13, 1.23, 0.09}; //InputBoard1_old

//const int cRoom::RoomValvePin[] = {31, 30, 28, 33, 29, 32, 26, 35, 27, 34, 41, 40, 38, 43, 39, 42}; //Board_V8.7
const int cRoom::RoomValvePin[] = {31, 30, 28, 33, 29, 32, 26, 35, 27, 34, 39, 38, 36, 41, 37, 40}; //Board_V8.6
//const int cRoom::RoomValvePin[] = {29, 28, 26, 31, 27, 30, 24, 33, 25, 32, 37, 36, 34, 39, 35, 38}; //Board_V8.4
//const int cRoom::RoomValvePin[] = {26, 27, 25, 28, 24, 29, 23, 30, 22, 31, 34, 35, 33, 36, 32, 37}; //Board_V8

// Default room types mapping
const RoomTypes cRoom::DefaultRoomType[] = {Side,Side,Side,Side,Living,Living,Living,Hallway,Hallway,Work,Side,Sleeping,Bath,Side,Trim,Hallway};

const int cRoom::MultiplexChannelRoomsIs[] = {14,12,10,8,6,4,2,0,14,12,10,8,6,4,2,0};
const int cRoom::MultiplexChannelRoomsSp[] = {15,13,11,9,7,5,3,1,15,13,11,9,7,5,3,1};
const int cRoom::MultiplexNumberRooms[] =    {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
