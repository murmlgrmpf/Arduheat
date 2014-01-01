#ifndef cRoom_h
#define cRoom_h

#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"
#include <PID_v1.h>
//#include "cHeating.h"
// Highest and lowest value for manual setpoint override
#define TempLow 10
#define TempHigh 40

class cRoom
{
	private:
	int _iRoomNumber;
	double _dIsTemp;
	double _dSpTemp;
	double _need;
	double _dSpTempSchedule;
	
	public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used */
	cRoom(void);
	cRoom(int iRoomNumber);
	//~cRoom();
	/// Each room has a valve
	void init(int iRoomNumber);
	cRoomValve Valve;
	PID pid;
	cTempSingle IsTemp;
	cTempSingle SpTemp;
	void setRoomNumber(int iRoomNumber);
	void setSpTempSchedule(double fSpTempSchedule);
	double getIsTemp(void);
	double getSpTemp(void);
	double getNeed(void);
	double getSpTempSchedule(void);
};

#endif
