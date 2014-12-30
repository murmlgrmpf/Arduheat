#ifndef cTemp_h
#define cTemp_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTrigger.h"

// RTC for DateTime class
#include <RTClib.h>

//! Pullup Resistor Value in Ohm
#define R 2000.0   //! OP Resistor Value [Ohm]
#define R1 12000.0 //! OP Resistor Value [Ohm]
#define R2 11000.0 //! OP Resistor Value [Ohm]
#define R3 10000.0 //! Voltage of power supply [V]
#define Vcc 5.04   
//! Factors for Temperature Sensors
#define Alpha 0.00788 //! Factors for Temperature Sensors
#define Beta 0.00001937 //! Factors for Temperature Sensors
#define R25 2000.0 
//! Smoothing filter coefficient
#define AlphaT 10 //! Filter sampling interval
#define FilterTimePeriod 100 

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
	* \param iMultiplexChannel
	* \param dOffset
	*
	*/
	cTempSensor(int MultiplexNumber_ = 0,int MultiplexChannel_ = 0, float Offset_ = 0.0):
	Trigger(FilterTimePeriod)
	{
		pinMultiplexInput =  MultiplexInput[MultiplexNumber_];
		MultiplexChannel = MultiplexChannel_;
		Offset = Offset_;
		
		// Initialize temperature using high value to avoid switching on the burner
		// during initialization
		TempFilt = 60;
		
		// exponential filtering coefficient [1/#Measurements]
		alphaFilt = AlphaT*FilterTimePeriod/1000;
		
		// Multiplexer input line initializations
		pinMode(MultiplexInput[0], INPUT);
		pinMode(MultiplexInput[1], INPUT);
		pinMode(MultiplexInput[2], INPUT);
		// Multiplexer control line initializations
		pinMode(MultiplexControl[0], OUTPUT);
		pinMode(MultiplexControl[1], OUTPUT);
		pinMode(MultiplexControl[2], OUTPUT);
		pinMode(MultiplexControl[3], OUTPUT);
	}
	
	
	float get( void )
	{
		if(Trigger.get()) {
			// Set the right multiplexer channel
			digitalWrite(MultiplexControl[0],HIGH && (MultiplexChannel & B00000001));
			digitalWrite(MultiplexControl[1],HIGH && (MultiplexChannel & B00000010));
			digitalWrite(MultiplexControl[2],HIGH && (MultiplexChannel & B00000100));
			digitalWrite(MultiplexControl[3],HIGH && (MultiplexChannel & B00001000));
			
			// Determine raw temperature reading
			float Ua = analogRead(pinMultiplexInput)/1023.0*Vcc;
			float Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
			float Kt = R*Ue/(Vcc-Ue) /R25;
			float Temp = 25+(sqrt(pow(Alpha, 2)-4*Beta+4*Beta*Kt)-Alpha)/(2*Beta);
			
			// Apply filtering
			TempFilt = (alphaFilt/(alphaFilt+1))*Temp  + 1/(alphaFilt+1)*TempFilt;
		}
		// Return filtered temperature plus offset
		return(TempFilt+Offset);
	}
	
	private:
	float TempFilt;
	float Offset;
	
	float alphaFilt;
	int pinMultiplexInput;
	int MultiplexChannel;
	
	cTrigger Trigger;
};

#endif
