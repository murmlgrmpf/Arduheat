#ifndef cRoom_h
#define cRoom_h
//RTC


#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"
#include "cPID.h"
#include "cPump.h"
#include "cSpHeating.h"
#include "cMixer.h"
#include <ArduinoJson.h>

// RTC
//#include <Wire.h>
#include <RTClib.h>

// Highest and lowest value for Room temperature setpoints
#define TempRoomLow 10
#define TempRoomHigh 30

#define nSet 2 // Number of sets of Schedules (At home, away)
#define nRoomType 5 // Number of different room programs
#define nDayType 2 // Days of the Week vs. Weekend
#define nSwitch 4  // Number of switching times each day
#define DiffTempHeatingLeadReturn 5 //

enum RoomTypes {Living=0, Sleeping=1, Hallway=2, Bath=3, Side=4};
enum SetTypes {Normal=0, Holiday=1};
enum DayTypes {Weekend=0, Workday=1};

extern DateTime TimeNow;

typedef struct{
	TimeSpan time;
	float temp;
} sTempSchedule;

class cRoom
{
	public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used */
	cRoom(void);
	cRoom(int iRoomNumber);
	//~cRoom();
	/// To initialize the rooms
	void init(int iRoomNumber);
	
	void   setSpTemp(double spTemp){_dSpTemp = spTemp;};
	double getIsTemp(void){return IsTemp.get();};
	double getSpTemp(void){return _dSpTemp;};
	double getNeed(void);
	RoomTypes RoomType;
	cTempSensor SpTempOverride;
	
	private:
	int _iRoomNumber;
	double _dSpTemp;
	
	//cPID pid;
	/// Each room has a valve
	cRoomValve Valve;
	cTempSensor IsTemp;

	void setRoomNumber(int iRoomNumber);

};

class cRooms
{
	public:
	cRooms(void);
	
	cTempSensor IsTempHeatingLead;
	cTempSensor IsTempHeatingReturn;
	
	cRoom Room[16];
	cPump PumpHeating;
	cPID PIDPumpHeating;
	cPID PIDMixer;
	cMixer Mixer;
	cSpHeating SpHeating;

	DateTime relTime;
	
	boolean need(void);
	double getSpHeating(void);
	SetTypes SetType;
	DayTypes DayType;
	DayTypes getDayType(uint8_t day);
	
	void ChargeRooms(boolean bneedChargeRooms, boolean BoilerCharges = false);
	
	void initDefaultRoomtypes();
	void initDefaultSetpoint();
	double RoomTemps[nRoomType];
	sTempSchedule TempOffsetSchedule[nSet][nRoomType][nDayType][nSwitch];
	
	void getOffsetTime(JsonObject& root);
	int setOffsetTime(JsonObject& root);
	void getOffsetTemp(JsonObject& root);
	int setOffsetTemp(JsonObject& root);
	void getRooms(JsonObject& root);
	int setRooms(JsonObject& root);
	
	void getData(JsonObject& root);
	
	private:
	double _dIsTempHeatingLead;
	double _dSpTempHeatingLead;
	double _dIsTempHeatingReturn;
	double _dSpTempHeatingReturn;
	
	double dMaxDiff;
	double dMaxSp;
	
};



#endif
