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

const float cRoom::RoomIsOffset[] = {-2.5, -1.47, -1.37, 0.6, -2.0, 1.84, -1.44, -1.65, -0.4, -1.35, -0.6, 0.6, 0.6, -2.2, -1.04, -0.07};
const float cRoom::RoomSpOffset[] = {0.77, 2.0436198179979925, 1.6490293225480279, 1.4206471183013285, -0.95, 1.0153993933264864, 2.1353791708794319, 2.4985338725986033, -0.43, 2.1375530839231054, 2.2762386248736455, 1.225520728008032, 0.36619817997989657, 1.0406370070778195, 1.5027704752275994, 1.5950556117289487};

// room valve pinout mapping
const int cRoom::RoomValvePin[] = {23, 24, 22, 25, 15, 26, 14, 27, 5, 28, 34, 29, 33, 30, 32, 31};

// Default room types mapping
const RoomTypes cRoom::DefaultRoomType[] = {Side,Side,Side,Side,Living,Living,Living,Hallway,Hallway,Hallway,Sleeping,Sleeping,Bath,Side,Side,Hallway};

const int cRoom::MultiplexChannelRoomsIs[] = {14,12,10,8,6,4,2,0,14,12,10,8,6,4,2,0};
const int cRoom::MultiplexChannelRoomsSp[] = {15,13,11,9,7,5,3,1,15,13,11,9,7,5,3,1};
const int cRoom::MultiplexNumberRooms[] =    { 0, 0, 0,0,0,0,0,0, 1, 1, 1,1,1,1,1,1};