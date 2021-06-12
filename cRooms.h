#ifndef cRooms_h
#define cRooms_h
//RTC


#include "Arduino.h"
#include "cTemp.h"
#include "cRoomValve.h"
#include <PID_v1.h>
#include "cPump.h"
#include "cMixer.h"
#include "cRoom.h"
#include "cLFPWM.h"
#include <ArduinoJson.h>
#include "cWarmWater.h"

// RTC for DateTime class
#include <RTClib.h>

#define nSwitch     4 //Number of switching times each day
#define nRooms     16 //Number of rooms

#define DiffTempHeatingLeadReturn 5 //

// Set default parameters for SpHeating curve
#define dsteil 0.6 // Steigung
#define dkh    2.5  // Verschiebung vertikal
#define dverst 1.8 // Raumeinfluss
#define dminvl 30.0
#define dmaxvl 43.0


class cRooms
{
	public:
	boolean bAct;
	cRooms(cWarmWater* WarmWater_);

	cTempSensor IsTempHeatingLead;
	cTempSensor IsTempHeatingReturn;
	cTempSensor TempOutside;
	cRoom Room[nRooms] = {{0, this},{1, this},
	{2, this},{3, this},{4, this},{5, this},
	{6, this},{7, this},{8, this},{9, this},
	{10, this},{11, this},{12, this},{13, this},
	{14, this},{15, this}};
	cPump Pump;
	cMixer Mixer;
	
    double getNeed(void);
	boolean active(void);
	double SpTemp(void);
	SetTypes SetType;
	
	void charge(boolean bneedChargeRooms, boolean bcloseMixer=false);
	
	void initDefaultSetpoint();
	double MasterSpTemps[nRoomTypes];
	sTempSchedule TempOffsetSchedule[nSetTypes][nRoomTypes][nDayTypes][nSwitch];
	
	void getOffsetTime(JsonObject& root);
	int setOffsetTime(JsonObject& root);
	void getOffsetTemp(JsonObject& root);
	int setOffsetTemp(JsonObject& root);
	void getRooms(JsonObject& root);
	int setRooms(JsonObject& root);
	
	void getData(JsonObject& root);
        
        cLFPWM PWM;
        cWarmWater* WarmWater;
        
	private:
	double dMaxSp;
	double MaxNeed;	
};


#endif
