#ifndef cTemp_h
#define cTemp_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTrigger.h"

// RTC for DateTime class
#include <RTClib.h>

//! Pullup Resistor Value in Ohm
#define R 2000.0
//! OP Resistor Value [Ohm]
#define R1 12000.0
//! OP Resistor Value [Ohm]
#define R2 11000.0
//! OP Resistor Value [Ohm]
#define R3 10000.0
//! Voltage of power supply [V]
#define Vcc 5.04
//! Factors for Temperature Sensors
#define Alpha 0.00788
//! Factors for Temperature Sensors
#define Beta 0.00001937
//! Factors for Temperature Sensors
#define R25 2000.0

#define AlphaT 10 // smoothing filter coefficient
#define FilterTimePeriod 100 // filter sampling interval

// extern int* MultiplexInput;


typedef struct{
	TimeSpan time;
	float temp;
} sTempSchedule;


class cTempSensor
{
	public:
	/**
	* \brief Constructor for temperature sensors.
	* Temperature sensors use an exponential filter to smooth the sensor measurements.
	*
	* \param iMultiplexNumber
	* \param iMultiplexConnector
	* \param dOffset
	*
	*/
	cTempSensor(int iMultiplexNumber = 0.0,int iMultiplexConnector = 0.0, float dOffset = 0.0);
	
	float get(void);
	void set(int MultiplexNumber_,int MultiplexChannel_, float Offset_);
	
	private:
	float TempFilt;
	float Offset;
	
	float alphaFilt;
	int pinMultiplexInput;
	int MultiplexChannel;
	
	cTrigger Trigger;
};

float readTemperature(int pinMultiplexInput);
void setMultiplexer(int MultiplexChannel);

#endif
