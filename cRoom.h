#ifndef cRoom_h
#define cRoom_h
//RTC


#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"
#include "cPID.h"
#include "cPump.h"
#include "cMixer.h"
#include <ArduinoJson.h>

// RTC for DateTime class
#include <RTClib.h>

// Highest and lowest value for Room temperature setpoints
#define TempRoomLow 10
#define TempRoomHigh 30

#define nSet        2 // Number of sets of Schedules (At home, away)
#define nRoomType   5 // Number of different room programs
#define nDayType    2 // Days of the Week vs. Weekend
#define nSwitch     4  // Number of switching times each day
#define nRooms     16

// Calibration needed!
const double RoomIsOffset[] = {-2.2, -1.6, -2.1, 0.6, -1.5, 0.75, -1.7, 0.09, -2.21, -1, -1, 0.55, 0.3, -2, -2.2, -0.61};
// Calibration by measurements
const double RoomSpOffset[] = {2.6722851365013511, 2.0436198179979925, 1.6490293225480279, 1.4206471183013285, 1.0462285136502452, 1.0153993933264864, 2.1353791708794319, 2.4985338725986033, 5.2720930232559837, 2.1375530839231054, 2.2762386248736455, 1.225520728008032, 0.36619817997989657, 1.0406370070778195, 1.5027704752275994, 1.5950556117289487};

// Pinout definition
const int RoomValvePin[] = {23, 24, 22, 25, 15, 26, 14, 27, 5, 28, 34, 29, 33, 30, 32, 31};

#define DiffTempHeatingLeadReturn 5 //

enum RoomTypes {Living=0, Sleeping=1, Hallway=2, Bath=3, Side=4};
enum SetTypes {Normal=0, Holiday=1};
enum DayTypes {Weekend=0, Workday=1};

const RoomTypes DefaultRoomType[] = {Side,Side,Side,Side,Living,Living,Living,Hallway,Hallway,Hallway,Sleeping,Sleeping,Bath,Side,Side,Hallway};

extern DateTime TimeNow;

class cRoom
{
	public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used cRoom(void) */
	cRoom(int RoomNumber_ = 1 )
	//:pid(0.1, 0, 0, DIRECT)
	{
		init(RoomNumber_);
		RoomType = DefaultRoomType[RoomNumber];
		SpTemp = 20.0;
	}
	
	/// To initialize the rooms
	void init(int RoomNumber_)
	{
		RoomNumber = RoomNumber_;
		
		IsTemp.set(MultiplexNumberRooms[RoomNumber],MultiplexChannelRoomsIs[RoomNumber],RoomIsOffset[RoomNumber]);
		SpTempOverride.set(MultiplexNumberRooms[RoomNumber],MultiplexChannelRoomsSp[RoomNumber],RoomSpOffset[RoomNumber]);
		
		//Set the pin of the valve according to pinout scheme.
		Valve.setPinOpen(RoomValvePin[RoomNumber_]);
		
		//pid.SetOutputLimits(0, 1);
	}
	
	double getNeed(void)
	{
		// Compute need
		//double _need = pid.run(getSpTemp(),IsTemp.get());
		double _need = getSpTemp()-IsTemp.get();
		// Open Valve if heat is needed
		Valve.set((_need>0));
		
		return _need;
	}
	
	void   setSpTemp(double spTemp_){SpTemp = spTemp_;};
	double getIsTemp(void){return IsTemp.get();};
	double getSpTemp(void){return SpTemp;};
	
	RoomTypes RoomType;
	cTempSensor SpTempOverride;
	cTempSensor IsTemp;
	
	private:
	int RoomNumber;
	double SpTemp;
	
	//cPID pid;
	/// Each room has a valve
	cRoomValve Valve;
	
	void setRoomNumber(int iRoomNumber);
};

// Set default parameters for SpHeating curve
#define dsteil 0.6
#define dkh    4
#define dverst 2
#define dminvl 30
#define dmaxvl 45

class cRooms
{
	public:
	cRooms(void);
	
	cTempSensor IsTempHeatingLead;
	cTempSensor IsTempHeatingReturn;
	cTempSensor TempOutside;
	
	cRoom Room[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	cPump Pump;
	cPID PIDPump;
	cMixer Mixer;
	cPID PIDMixer;
	
	DateTime relTime;
	
	boolean need(void);
	double getSpHeating(void);
	SetTypes SetType;
	DayTypes DayType;
	DayTypes getDayType(uint8_t day);
	
	void ChargeRooms(boolean bneedChargeRooms, boolean BoilerCharges = false);
	
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
