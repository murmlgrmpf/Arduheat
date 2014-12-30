#ifndef cRoom_h
#define cRoom_h

#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"

// RTC for DateTime class
#include <RTClib.h>

#define TempRoomLow 10
#define TempRoomHigh 30
#define RoomMargin 0.5

enum RoomTypes {Living, Sleeping, Hallway, Bath, Side, nRoomTypes};
enum SetTypes  {Normal, Holiday, nSetTypes};
enum DayTypes  {Weekend, Workday, nDayTypes};

// Calibration by measurements
const double RoomIsOffset[] = {-2.2, -1.6, -2.1, 0.6, -1.5, 0.75, -1.7, 0.09, -2.21, -1, -1, 0.55, 0.3, -2, -2.2, -0.61};
const double RoomSpOffset[] = {2.6722851365013511, 2.0436198179979925, 1.6490293225480279, 1.4206471183013285, 1.0462285136502452, 1.0153993933264864, 2.1353791708794319, 2.4985338725986033, 5.2720930232559837, 2.1375530839231054, 2.2762386248736455, 1.225520728008032, 0.36619817997989657, 1.0406370070778195, 1.5027704752275994, 1.5950556117289487};

// room valve pinout mapping
const int RoomValvePin[] = {23, 24, 22, 25, 15, 26, 14, 27, 5, 28, 34, 29, 33, 30, 32, 31};

// Default room types mapping
const RoomTypes DefaultRoomType[] = {Side,Side,Side,Side,Living,Living,Living,Hallway,Hallway,Hallway,Sleeping,Sleeping,Bath,Side,Side,Hallway};

extern DateTime TimeNow;

// Forward declaration of cRooms class
class cRooms;

class cRoom
{
	public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used cRoom(void) */
	cRoom(int RoomNumber , cRooms* Rooms_ ):
	IsTemp(MultiplexNumberRooms[RoomNumber],MultiplexChannelRoomsIs[RoomNumber],RoomIsOffset[RoomNumber]),
	SpTempOverride(MultiplexNumberRooms[RoomNumber],MultiplexChannelRoomsSp[RoomNumber],RoomSpOffset[RoomNumber]),
	Valve(RoomValvePin[RoomNumber])
	{
		RoomType = DefaultRoomType[RoomNumber];
		Rooms = Rooms_;
	}
	
	/// Compute need of room.
	double getNeed(void)
	{
		double need = getSpTemp()-IsTemp.get();
		// Open Valve if heat is needed
		
		Valve.set((need+RoomMargin > 0));
		
		return need;
	}
	
	static DayTypes getDayType( uint8_t day  )
	{
		DayTypes DayType;
		if ((day == 0)||(day == 6))
			DayType = Weekend;
		else
			DayType = Workday;
		
		return DayType;
	}
	
	double getSpTemp(void);
	
	cRooms* Rooms;
	RoomTypes RoomType;
	cTempSensor SpTempOverride;
	cTempSensor IsTemp;
	
	private:
	
	cRoomValve Valve;
};




#endif
