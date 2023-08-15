#ifndef cTemp_h
#define cTemp_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTrigger.h"

//! Pullup Resistor Value in Ohm
#define R 2000.0   //! OP Resistor Value [Ohm]
#define R1 12000.0 //! OP Resistor Value [Ohm]
#define R2 11000.0 //! OP Resistor Value [Ohm]
#define R3 10000.0 //! OP Resistor Value [Ohm]
//! Factors for Temperature Sensors
#define Alpha 0.00788 //! Factors for Temperature Sensors
#define Beta 0.00001937 //! Factors for Temperature Sensors
#define R25 2000.0 
//! Smoothing filter coefficient
#define AlphaT 8.0 //Was 10 - ! Error decay rate
#define TimePeriod 100
#define nMultiSample 3


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
	cTempSensor(const int * MultiplexNumber_ , const int* MultiplexChannel_, const float* Offset_, float TempFilt_=60.0, float AlphaT_=AlphaT, unsigned long TimePeriod_ =TimePeriod):
        Trigger(TimePeriod_)
	{
		pinMultiplexInput = &MPInput[pgm_read_word(MultiplexNumber_)];
		MultiplexChannel = MultiplexChannel_;
		//Offset = Offset_;
		Offset = Offset_;
		// Initialize temperature using high value to avoid switching on the burner
		// during initialization
		TempFilt = TempFilt_;
		alphaFilt = AlphaT_*TimePeriod/1000;
		initMultiplexer();
	}
	
	float getRaw( void )
        {
            // Set the right multiplexer channel
            digitalWrite(pgm_read_word(&MPControl[0]),HIGH && (pgm_read_word(MultiplexChannel) & B00000001));
            digitalWrite(pgm_read_word(&MPControl[1]),HIGH && (pgm_read_word(MultiplexChannel) & B00000010));
            digitalWrite(pgm_read_word(&MPControl[2]),HIGH && (pgm_read_word(MultiplexChannel) & B00000100));
            digitalWrite(pgm_read_word(&MPControl[3]),HIGH && (pgm_read_word(MultiplexChannel) & B00001000));
            // Multisampling
            float TempMult = pgm_read_float(Offset); //Initialize with offset
            for (int i = 1;i<=nMultiSample;i++)
            {
                // Determine raw temperature reading
                float Ua = analogRead(pgm_read_word(pinMultiplexInput))/1023.0*Vcc;
                float Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
                float Kt = R*Ue/(Vcc-Ue) /R25;
                float Temp = 25+(sqrt(pow(Alpha, 2)-4*Beta+4*Beta*Kt)-Alpha)/(2*Beta);
                TempMult = TempMult+Temp/nMultiSample; // Add up offset and the average temperature measurement from the sensor
            }
            return(TempMult);
        }
	
	float get( void )
	{
		if (Trigger.get()) {
                        // Apply filtering: exponential filtering coefficient [1/#Measurements]
						TempFilt = (alphaFilt/(alphaFilt+1))*getRaw()  + (1/(alphaFilt+1))*TempFilt;
		}
		// Return filtered temperature
		return(TempFilt);
	}
	
	double* getPtr(void) {return &TempFilt;}
	
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
        
        cTrigger Trigger;
        
        float alphaFilt;
	double TempFilt;
	const float* Offset;
	
	const int* pinMultiplexInput;
	const int* MultiplexChannel;
};

#endif
