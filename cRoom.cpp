#include "cRoom.h"
#include "cRooms.h"

/// Creates a room.
/** As the rooms get created in an array, only the default constructor can be used cRoom(void) */
cRoom::cRoom(int RoomNumber , cRooms* Rooms_ , double p , double i , double d):
IsTemp(&MultiplexNumberRooms[RoomNumber], &MultiplexChannelRoomsIs[RoomNumber], &RoomIsOffset[RoomNumber], TRoomInit, AlphaTRoom),
SpTempOverride(&MultiplexNumberRooms[RoomNumber], &MultiplexChannelRoomsSp[RoomNumber], &RoomSpOffset[RoomNumber], TRoomInit, AlphaTRoom),
Valve(&RoomValvePin[RoomNumber]),
PID(IsTemp.getPtr(), &need, &spT, p, i, d, DIRECT)
{
	RoomType = static_cast<RoomTypes>(pgm_read_word(&DefaultRoomType[RoomNumber]));
	Rooms = Rooms_;
        
        spT=0.0;
        need=0.0;
        IsTemp.get();
        
        SetSampleTime(60000); // 60(s)*1000(ms/s)
        SetMode(AUTOMATIC);
        SetOutputLimits(0.0, 1.0);
}

double cRoom::getSpTemp(void)
{
	DayTypes DayType = getDayType( TimeNow.dayOfWeek() );
	
	TimeSpan Now;
	Now.set(0, TimeNow.hour(), TimeNow.minute(), 0);
	
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
	
       // Hysteresis for charging the rooms
       Valve.set(Rooms->PWM.get(need));
        
        return need;
}

const float cRoom::RoomIsOffset[] = {-1.69, -0.48, 0.19, 1.32, -1.6, 1.86, -1.34, -1.22, -0.41, -0.13, -0.55, 0.2, 0.66, -0.93, -0.33, 0.49};
const float cRoom::RoomSpOffset[] = {1.36, -0.67, 0.22, -0.24, 0.05, -0.01, 3.04, 0.22, -1.17, -0.06, 0.14, 0.71, 1.03, 0.67, 2.03, 0.89};

// room valve pinout mapping
const int cRoom::RoomValvePin[] = {26, 27, 25, 28, 24, 29, 23, 30, 22, 31, 34, 35, 33, 36, 32, 37};

// Default room types mapping
const RoomTypes cRoom::DefaultRoomType[] = {Side,Side,Side,Side,Living,Living,Living,Hallway,Hallway,Hallway,Sleeping,Sleeping,Bath,Side,Side,Hallway};

const int cRoom::MultiplexChannelRoomsIs[] = {14,12,10,8,6,4,2,0,14,12,10,8,6,4,2,0};
const int cRoom::MultiplexChannelRoomsSp[] = {15,13,11,9,7,5,3,1,15,13,11,9,7,5,3,1};
const int cRoom::MultiplexNumberRooms[] =    { 0, 0, 0,0,0,0,0,0, 1, 1, 1,1,1,1,1,1};
