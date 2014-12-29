#include "cTemp.h"

cTempSensor::cTempSensor(int iMultiplexNumber,int MultiplexChannel, float Offset):
Trigger(FilterTimePeriod)
{
	// Multiplexer input line initializations
	pinMode(MultiplexInput[0], INPUT);
	pinMode(MultiplexInput[1], INPUT);
	pinMode(MultiplexInput[2], INPUT);
	// Multiplexer control line initializations
	pinMode(MultiplexControl[0], OUTPUT);
	pinMode(MultiplexControl[1], OUTPUT);
	pinMode(MultiplexControl[2], OUTPUT);
	pinMode(MultiplexControl[3], OUTPUT);
	
	set(iMultiplexNumber,MultiplexChannel,Offset);
}

void cTempSensor::set(int MultiplexNumber_,int MultiplexChannel_, float Offset_)
{
	pinMultiplexInput =  MultiplexInput[MultiplexNumber_-1];
	MultiplexChannel = MultiplexChannel_;
	Offset = Offset_;
	
	// Initialize temperature using high value to avoid switching on the burner
	// during initialization
	TempFilt = 60;
	
	// exponential filtering coefficient [1/#Measurements]
	alphaFilt = AlphaT*FilterTimePeriod/1000;
}

float cTempSensor::get( void )
{
	if(Trigger.get()) {
		setMultiplexer(MultiplexChannel);
		
		float Ua = analogRead(pinMultiplexInput)/1023.0*Vcc;
		float Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
		float Kt = R*Ue/(Vcc-Ue) /R25;
		float Temp = 25+(sqrt(pow(Alpha, 2)-4*Beta+4*Beta*Kt)-Alpha)/(2*Beta);
		
		TempFilt = (alphaFilt/(alphaFilt+1))*Temp  + 1/(alphaFilt+1)*TempFilt;
	}
	
	return(TempFilt+Offset);
}


/**
 * @fn float readTemperature(int pinMultiplexInput,int MultiplexChannel)
 * @brief Reads in and evaluates the Temperature for a given multiplexer and connector index at the multiplexer.
 *
 * Reads in the previously switched channels of the multiplexers (see setMultiplexer()).
 * Then converts the values to temperatures and gives back the result to getTemperatures().
 * @return fTemp
 */
float readTemperature(int pinMultiplexInput)
{
	// Calculate raw Temperatures (Before Calibration)
	float Ua = analogRead(pinMultiplexInput)/1023.0*Vcc;
	float Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
	float Kt = R*Ue/(Vcc-Ue) /R25;
	float Temp = 25+(sqrt(pow(Alpha, 2)-4*Beta+4*Beta*Kt)-Alpha)/(2*Beta);
	
	return Temp;
}

/**
 * @fn void setMultiplexer(int)
 * @brief The function sets the steering pins A0..A3 of the multiplexers to the correct values.
 * @param i Number of the sensor input channel to be read from the multiplexers
 */
void setMultiplexer(int MultiplexChannel)
{
	digitalWrite(MultiplexControl[0],HIGH && (MultiplexChannel & B00000001));
	digitalWrite(MultiplexControl[1],HIGH && (MultiplexChannel & B00000010));
	digitalWrite(MultiplexControl[2],HIGH && (MultiplexChannel & B00000100));
	digitalWrite(MultiplexControl[3],HIGH && (MultiplexChannel & B00001000));
}