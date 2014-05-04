#ifndef cRoom_h
#define cRoom_h

#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"
#include "cPID.h"
#include "cPump.h"
#include "cSpHeating.h"
#include "cMixer.h"
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
	
	cPID pid;
	/// Each room has a valve
	cRoomValve Valve;
	cTempSensor IsTemp;
	cTempSensor SpTemp;

	void setRoomNumber(int iRoomNumber);
	
	public:
	/// Creates a room.
	/** As the rooms get created in an array, only the default constructor can be used */
	cRoom(void);
	cRoom(int iRoomNumber);
	//~cRoom();
	/// To initialize the rooms
	void init(int iRoomNumber);
	
	void setSpTempSchedule(double fSpTempSchedule);
	double getIsTemp(void);
	double getSpTemp(void);
	double getNeed(void);
	double getSpTempSchedule(void);
};

class cRooms
{
	private:
	double _dIsTempHeatingLead;
	double _dSpTempHeatingLead;
	double _dIsTempHeatingReturn;
	double _dSpTempHeatingReturn;
	
	double dMaxDiff;
	double dneedChargeRooms;
	double dMaxSp;
	
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
	
	boolean need();
	double getSpHeating();
	
	void ChargeRooms(boolean bneedChargeRooms, boolean BoilerCharges = false);
	
};

#endif
