#ifndef cRooms_h
#define cRooms_h

#include <ArduinoJson.h>
#include <TimeLib.h>

#include "Arduino.h"
#include "cLFPWM.h"
#include "cMixer.h"
#include "cPump.h"
#include "cRoom.h"
#include "cTemp.h"
#include "cWarmWater.h"

#define nSwitch 4  // Number of switching times each day
#define nRooms 16  // Number of rooms

#define DiffTempHeatingLeadReturn 5

// Set default parameters for SpHeating curve
#define dsteil 0.55  // Steigung (war 0.6)
#define dkh 1.8      // Verschiebung vertikal (war 2.1)
#define dverst 2.5   // Raumeinfluss (war 1.8, 2.2)
#define dminvl 28.0  // (war 30.0)
#define dmaxvl 42.0

typedef struct{
	time_t time;
	float temp;
} sTempSchedule;

class cRooms {
   public:
	boolean bAct;
	cRooms(cWarmWater* WarmWater_);

	cTempSensor IsTempHeatingLead;
	cTempSensor IsTempHeatingReturn;
	cTempSensor TempOutside;
	cRoom Room[nRooms] = {{0, this},  {1, this},  {2, this},  {3, this}, {4, this},  {5, this},
						  {6, this},  {7, this},  {8, this},  {9, this}, {10, this}, {11, this},
						  {12, this}, {13, this}, {14, this}, {15, this}};
	cPump Pump;
	cMixer Mixer;

	double getNeed(void);
	boolean active(void);
	double SpTemp(void);
	SetTypes SetType;

	void charge(boolean bneedChargeRooms, boolean bcloseMixer = false);

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
