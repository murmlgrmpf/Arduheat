#include "cRoom.h"
#include "cRooms.h"

/// Creates a room.
/** As the rooms get created in an array, only the default constructor can be used cRoom(void) */
cRoom::cRoom(int RoomNumber , cRooms* Rooms_ ):
IsTemp(&MultiplexNumberRooms[RoomNumber], &MultiplexChannelRoomsIs[RoomNumber], &RoomIsOffset[RoomNumber]),
SpTempOverride(&MultiplexNumberRooms[RoomNumber], &MultiplexChannelRoomsSp[RoomNumber], &RoomSpOffset[RoomNumber]),
Valve(&RoomValvePin[RoomNumber])
{
	RoomType = static_cast<RoomTypes>(pgm_read_word(&DefaultRoomType[RoomNumber]));
	Rooms = Rooms_;
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

const float cRoom::RoomIsOffset[] = {-1.1, -1.6, -1.2, 0.6, -1.5, 0.6, -2.2, -1.4, -0.5, -0.5, -0.1, 0.2, 0.7, -1.6, -1, 0.2};
const float cRoom::RoomSpOffset[] = {1.82, 0.53, 1.42, 0.8, -0.5, 0.71, 2.44, 0.79, 0.11, 0.62, 1.24, 1.69, 1.23, 0.34, 1.64, 0.98};

// room valve pinout mapping
const int cRoom::RoomValvePin[] = {26, 27, 25, 28, 24, 29, 23, 30, 22, 31, 34, 35, 33, 36, 32, 37};

// Default room types mapping
const RoomTypes cRoom::DefaultRoomType[] = {Side,Side,Side,Side,Living,Living,Living,Hallway,Hallway,Hallway,Sleeping,Sleeping,Bath,Side,Side,Hallway};

const int cRoom::MultiplexChannelRoomsIs[] = {14,12,10,8,6,4,2,0,14,12,10,8,6,4,2,0};
const int cRoom::MultiplexChannelRoomsSp[] = {15,13,11,9,7,5,3,1,15,13,11,9,7,5,3,1};
const int cRoom::MultiplexNumberRooms[] =    { 0, 0, 0,0,0,0,0,0, 1, 1, 1,1,1,1,1,1};
