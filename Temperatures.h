#ifndef Temperatures_h
#define Temperatures_h

#include "Arduino.h"
#include "PinDefinitions.h"


//! Resistor Values in Ohm
#define R 2000.0
//! Resistor Values in Ohm
#define R1 12000.0
//! Resistor Values in Ohm
#define R2 11000.0
//! Resistor Values in Ohm
#define R3 10000.0
//! Voltage of power supply
#define Vcc 5.05
//! Factors for Temperature Sensors
#define Alpha 0.00788
//! Factors for Temperature Sensors
#define Beta 0.00001937
//! Factors for Temperature Sensors
//1980-2020
#define R25 2000.0
//!Resistance of Multiplexer 1
#define RM1 57.3248//331.606//358.21//265.45
//!Resistance of Multiplexer 2
#define RM2 55.3797//37.647//312.258
//!Resistance of Multiplexer 3
#define RM3 50.309//330.853

//extern float TemperaturesArray[16][3] ={0};

/**
 * @fn float getTemperatures(void)
 * @brief Reads in all Temperatures and stores them in an array.
 *
 * Switches the channels of the multiplexers (see setMultiplexer()) and
 * then reads in the temperatures via evaluateTemperatures()
 * @return T1raw
 */

void getTemperatures(float TemperatureArray[][3]);
void setMultiplexer(int i);
float readTemperature(int iMultiplexNumber,int iMultiplexConnector);

#endif
