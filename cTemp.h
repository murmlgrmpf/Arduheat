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
#define TimePeriod 100

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
	cTempSensor(const int * MultiplexNumber_ , const int* MultiplexChannel_, const float* Offset_)//:
	{
		StartTime = millis();
		
		pinMultiplexInput = &MPInput[pgm_read_word(MultiplexNumber_)];
		MultiplexChannel = MultiplexChannel_;
		//Offset = Offset_;
		Offset = Offset_;
		// Initialize temperature using high value to avoid switching on the burner
		// during initialization
		TempFilt = 60;
		
		initMultiplexer();
	}
	
	
	float get( void )
	{
		if (millis()-StartTime > TimePeriod) {
			StartTime = millis();
			
			// Set the right multiplexer channel
			digitalWrite(pgm_read_word(&MPControl[0]),HIGH && (pgm_read_word(MultiplexChannel) & B00000001));
			digitalWrite(pgm_read_word(&MPControl[1]),HIGH && (pgm_read_word(MultiplexChannel) & B00000010));
			digitalWrite(pgm_read_word(&MPControl[2]),HIGH && (pgm_read_word(MultiplexChannel) & B00000100));
			digitalWrite(pgm_read_word(&MPControl[3]),HIGH && (pgm_read_word(MultiplexChannel) & B00001000));
			
			// Determine raw temperature reading
			float Ua = analogRead(pgm_read_word(pinMultiplexInput))/1023.0*Vcc;
			float Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
			float Kt = R*Ue/(Vcc-Ue) /R25;
			float Temp = 25+(sqrt(pow(Alpha, 2)-4*Beta+4*Beta*Kt)-Alpha)/(2*Beta);
			
			// Apply filtering: exponential filtering coefficient [1/#Measurements]
			float alphaFilt = AlphaT*TimePeriod/1000;
			TempFilt = (alphaFilt/(alphaFilt+1))*Temp  + 1/(alphaFilt+1)*TempFilt;
		}
		// Return filtered temperature plus offset
		return(TempFilt+pgm_read_float(Offset));
	}
	

	
	private:
	static void initMultiplexer(void)
	{
		// Multiplexer input line initializations
		pinMode(pgm_read_word(&MPInput[0]), INPUT);
		pinMode(pgm_read_word(&MPInput[1]), INPUT);
		pinMode(pgm_read_word(&MPInput[2]), INPUT);
		// Multiplexer control line initializations
		pinMode(pgm_read_word(&MPControl[0]), OUTPUT);
		pinMode(pgm_read_word(&MPControl[1]), OUTPUT);
		pinMode(pgm_read_word(&MPControl[2]), OUTPUT);
		pinMode(pgm_read_word(&MPControl[3]), OUTPUT);
	}  
	
	unsigned long StartTime;
	  
	double TempFilt;
	const float* Offset;
	
	const int* pinMultiplexInput;
	const int* MultiplexChannel;
	
};

#endif
