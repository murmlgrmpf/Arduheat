#ifndef cRoom_h
#define cRoom_h

#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"

// RTC for DateTime class
#include <RTClib.h>

#include <avr/pgmspace.h>

#define TempRoomLow 13.0
#define TempRoomHigh 30.0
#define RoomMargin 0.4
#define AlphaTRoom 0.2
#define TRoomInit 25.0

enum RoomTypes {Living, Sleeping, Hallway, Bath, Side, nRoomTypes};
// enum SetTypes  {Normal, Holiday, nSetTypes};
enum SetTypes  {Normal, nSetTypes};
enum DayTypes  {Weekend, Workday, nDayTypes};

extern DateTime TimeNow;

// Forward declaration of cRooms class
class cRooms;

class cRoom
{
	public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used cRoom(void) */
	cRoom(int RoomNumber , cRooms* Rooms_ );
	
	/// Compute need of room.
	double getDeltaT(void)
	{
		double need = getSpTemp()-IsTemp.get();
		// Open Valve if heat is needed
		
                // Hysteresis for charging the rooms
                if (getSpTemp()-RoomMargin/2 > IsTemp.get()) Valve.set(true);
                if (getSpTemp()+RoomMargin/2 < IsTemp.get()) Valve.set(false);
		
		return need;
	}
	
	boolean getNeed(void) {return Valve.get();}
	
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
	// Calibration by measurements
	
	static const PROGMEM int MultiplexChannelRoomsIs[];
	static const PROGMEM int MultiplexChannelRoomsSp[];
	static const PROGMEM int MultiplexNumberRooms[];

	static const PROGMEM float RoomIsOffset[];
	static const PROGMEM float RoomSpOffset[];

	// room valve pinout mapping
	static const PROGMEM int RoomValvePin[];

	// Default room types mapping
	static const PROGMEM RoomTypes DefaultRoomType[];
};




#endif
