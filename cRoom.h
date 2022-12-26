#ifndef cRoom_h
#define cRoom_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cRoomValve.h"
#include "cTemp.h"

// RTC for DateTime class
#include <PID_v1.h>
#include <RTClib.h>
#include <avr/pgmspace.h>

#define TempRoomLow 13.5
#define TempRoomHigh 30.0
#define RoomMargin 0.4
#define AlphaTRoom 0.1
#define TRoomInit 25.0

enum RoomTypes { Living, Sleeping, Hallway, Bath, Side, Trim, Work, nRoomTypes };
// enum SetTypes  {Normal, Holiday, nSetTypes};
enum SetTypes { Normal, nSetTypes };
enum DayTypes { Weekend, Workday, nDayTypes };

extern DateTime TimeNow;

// Forward declaration of cRooms class
class cRooms;

class cRoom : public PID {
   public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used cRoom(void)
	 */
	cRoom(int RoomNumber, cRooms* Rooms_, double p = 2.1, double i = 0.0, double d = 0.0);

	/// Compute need of room.
	double getNeed(void);

	boolean getValve(void) { return Valve.get(); }

	static DayTypes getDayType(uint8_t day) {
		DayTypes DayType;
		if ((day == 0) || (day == 6))
			DayType = Weekend;
		else
			DayType = Workday;

		return DayType;
	}

	double getSpTemp(void);

	cRooms* Rooms;
	RoomTypes RoomType;
	cTempSensor IsTemp;
	cTempSensor SpTempOverride;

	double spT;
	double need;

   private:
	cRoomValve Valve;  // Calibration by measurements

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
