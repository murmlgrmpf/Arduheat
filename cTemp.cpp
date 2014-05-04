#include "cTemp.h"

cTempSensor::cTempSensor(void):
Trigger(FilterTimePeriod)
{
	set(0,0,0);
}

cTempSensor::cTempSensor(int iMultiplexNumber,int iMultiplexConnector, float Offset):
Trigger(FilterTimePeriod)
{
	set(iMultiplexNumber,iMultiplexConnector,Offset);
}

void cTempSensor::set(int iMultiplexNumber,int iMultiplexConnector, float Offset)
{
	_iMultiplexNumber = iMultiplexNumber;
	_iMultiplexConnector = iMultiplexConnector;
	_dOffset = Offset;
	
	// Initialize temperature using high value to avoid switching on the burner
	// during initialization
	_TempFilt = 60;
	
	// exponential filtering coefficient [1/#Measurements]
	_alphaT = AlphaT*FilterTimePeriod/1000;
}

float cTempSensor::get( void )
{
	if(Trigger.get())
	{
		_TempFilt = (_alphaT/(_alphaT+1))*readTemperature(_iMultiplexNumber,_iMultiplexConnector)  + 1/(_alphaT+1)*_TempFilt;
	}
	
	return(_TempFilt+_dOffset);
}


/**
 * @fn float readTemperature(int iMultiplexNumber,int iMultiplexConnector)
 * @brief Reads in and evaluates the Temperature for a given multiplexer and connector index at the multiplexer.
 *
 * Reads in the previously switched channels of the multiplexers (see setMultiplexer()).
 * Then converts the values to temperatures and gives back the result to getTemperatures().
 * @return fTemp
 */
float readTemperature(int iMultiplexNumber,int iMultiplexConnector)
{
  setMultiplexer(iMultiplexConnector);
  delay(1); // Timing issue: Temperature drops when delay is removed
  
  float Ua = 0;
  float Ue = 0;
  float Kt = 0;
  float fTemp =0;
  
    // Calculate raw Temperatures (Before Calibration)
  if (iMultiplexNumber==1){ // first Multiplexer
    Ua = analogRead(MultiplexInput1)/1023.0*Vcc;
    Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
    Kt = (Ue*(R+RM1)-Vcc*RM1)/((Vcc-Ue)*R25);
  }
  else if (iMultiplexNumber==2){ //second Multiplexer
    Ua = analogRead(MultiplexInput2)/1023.0*Vcc;
    Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
    Kt = (Ue*(R+RM2)-Vcc*RM2)/((Vcc-Ue)*R25);
  }
  else{ // third Multiplexer
    Ua = analogRead(MultiplexInput3)/1023.0*Vcc;
    Ue = (Ua + Vcc*(R1/R3))/(1+R1/R2+R1/R3);
    Kt = (Ue*(R+RM3)-Vcc*RM3)/((Vcc-Ue)*R25);
  }
  
  fTemp = 25+(sqrt(pow(Alpha, 2)-4*Beta+4*Beta*Kt)-Alpha)/(2*Beta);
  
  return fTemp;
}

/**
 * @fn void setMultiplexer(int)
 * @brief The function sets the steering pins A0..A3 of the multiplexers to the correct values.
 * @param i Number of the sensor input channel to be read from the multiplexers
 */
void setMultiplexer(int iMultiplexConnector)
{
  digitalWrite(MultiplexControl1,HIGH && (iMultiplexConnector & B00000001));
  digitalWrite(MultiplexControl2,HIGH && (iMultiplexConnector & B00000010));
  digitalWrite(MultiplexControl3,HIGH && (iMultiplexConnector & B00000100));
  digitalWrite(MultiplexControl4,HIGH && (iMultiplexConnector & B00001000));
}