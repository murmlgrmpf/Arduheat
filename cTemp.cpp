#include "cTemp.h"

cTemp::cTemp(void)
{
  // Initialize temperatures
  for (int j=0; j<16; j++){
    for (int i = 0; i<3; i++) {
       _TempFilt[j][i] = 0;
    }
  }
  
  _LastTime = millis();
  _TimePeriod = 100;
  // exponential filtering coefficient [1/#Measurements]
  _alphaT = AlphaT*_TimePeriod/1000;
}

void cTemp::run(void)
{
  unsigned long now = millis();
  unsigned long _NextTime = _LastTime+_TimePeriod;
  if(_NextTime < now) {
    
  for (int j=0; j<16; j++){
    for (int i = 0; i<3; i++)
    {
      _TempFilt[j][i] = (_alphaT/(_alphaT+1))*readTemperature(i+1,j)  + 1/(_alphaT+1)*_TempFilt[j][i];
    }
  }
  
  _LastTime = now;
  }
}


float cTemp::getTemp(int iMultiplexNumber,int iMultiplexConnector)
{
  cTemp::run();
  
  return(_TempFilt[iMultiplexConnector][iMultiplexNumber-1]);
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
void setMultiplexer(int i)
{
  //delay(2000);
  //Serial.println("DebugMultiplexer");
  //
  digitalWrite(MultiplexControl1,HIGH && (i & B00000001));
  digitalWrite(MultiplexControl2,HIGH && (i & B00000010));
  digitalWrite(MultiplexControl3,HIGH && (i & B00000100));
  digitalWrite(MultiplexControl4,HIGH && (i & B00001000));
}
