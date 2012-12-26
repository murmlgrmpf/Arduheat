#include "Arduino.h"
#include "PinDefinitions.h"
//#include "cTemp.h"
#include "cPump.h"
#include "cRoom.h"
#include "cHeating.h"
#include <PID_v1.h>

#define LiterPerImpuls 0.009263548

#define TimePeriod 1000
#define CPumpWarmWater 60
//#define NWarmWaterHeatExchanger 4.3
#define NWarmWaterHeatExchanger 

//! Counter for warm water
int iLastCounter = 0;
unsigned long LastTimeCounter = 0;
float fmassFlux = 0.0;
float fMassFlux = 0.0;
float fMassFlowPumpWarmWater = 0.0;
float fTempRefSchedule[16]={20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                    20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0};



unsigned long LastTime = 0;
boolean trigger = false;

float PumpState =0;

cHeating Heating;

double Input, Output;

// Simple Vorlauftemperatur reference
double SetpointTempWarmWater = 30.0;

PID myPID(&Input, &Output, &SetpointTempWarmWater, 0.01, 0.001, 0.03, DIRECT);

/**
 * @fn void setup(void)
 * @brief Initializes the serial connection and defines the in- and outputs.
 */
void  setup()
{
  Serial.begin(57600);
  //Serial.println("Steuerung");
  attachInterrupt(4,incCounter,RISING);
  PinInitialization();
  delay(2000);// Give reader a chance to see the output.
  analogReference(DEFAULT);
  
  SerialDebug();
  DataAcquisition(true);
  Heating.SetpointTempWarmWater = SetpointTempWarmWater;
  myPID.SetOutputLimits(0.0, 1.0);
  Output = 0.0;

}



/**
 * @fn void loop(void)
 * @brief Runs the main program which controls the system.
 */
void loop()
{
//  float fmaxHeatflow =0;
  
  
//  for(int i=0; i<16;i++)
//  {
//    Heating.Rooms[i].setTempRefSchedule(fTempRefSchedule[i]);
//    // Open valves of rooms that need heat
//    if (Heating.Rooms[i].getHeatflow()>0)
//    {
////      digitalWrite(Heating.Rooms[i].getPinValve(), LOW);
//    }
//    else // Close valves for rooms that do not need heat
//    {
////      digitalWrite(Rooms[i].getPinValve(), HIGH);
//    }
//    
//    // Identify maximum required Heatflow
//    if (Heating.Rooms[i].getHeatflow()<fmaxHeatflow)
//    {
//      fmaxHeatflow = Heating.Rooms[i].getHeatflow();
//    }
//    
////    PumpState = Heating.Rooms[i].getTempActual();
////    PumpState = Heating.Rooms[i].getTempRef();
////    PumpState = Heating.Rooms[i].getHeatflow();
//    
//  }
  //Heating.Rooms[1].getHeatflow();
  //Heating.ValveWarmWater.set(false);
  //Heating.ValveWarmWater.set(true);

  //Heating.PumpSolar.set(0.5);
  //Heating.PumpHeating.set(0.5);
  //Heating.PumpBoiler.set(0.50);
  //Heating.PumpWarmWater.set(0.5);
  
  // ***************Brauchwasser control
  
  //fMassFlowPumpWarmWater = Heating.HxWarmWater.calcMassflow(50.0, 8.0, Heating.TempBoilerTop(), Heating.FlowMeter.get() );
  //fMassFlowPumpWarmWater = Heating.HxWarmWater.calcMassflow(50.0, 8.0, 57.0, Heating.FlowMeter.get() );
  
  if(Heating.FlowMeter.get()>0.0)
  {
   
    Input = Heating.TempWarmWater();
    
   // timeoptimal control until 5 degrees below Setpoint
   if(Heating.TempWarmWater()<=Heating.SetpointTempWarmWater - 7)
   {
     //turn the PID on
     myPID.SetMode(MANUAL);
     // open Valve
     Heating.ValveWarmWater.set(true);
     // Set Pump to maximum mass flow rate
     Heating.PumpWarmWater.setMassFlowRate(Heating.PumpWarmWater.getMaxMassFlowRate());
     Output = 0;
   }
   else if (Heating.TempWarmWater()>=Heating.SetpointTempWarmWater+2)
   {
    myPID.SetMode(MANUAL);
    // stop pump
    Heating.PumpWarmWater.setMassFlowRate(0.0);
    // close valve
    Heating.ValveWarmWater.set(false);
    
    Output = 0;
   }
   else
   {
     // open Valve
     Heating.ValveWarmWater.set(true);
     
     // open loop control
     fMassFlowPumpWarmWater = Heating.HxWarmWater.calcMassflow(Heating.SetpointTempWarmWater, 8.1560, Heating.TempBoilerTop(), Heating.FlowMeter.get() );
//      Heating.PumpWarmWater.setMassFlowRate(fMassFlowPumpWarmWater);
     
     // closed loop control
     //turn the PID on
     myPID.SetMode(AUTOMATIC);
     myPID.Compute();
//      float theta = (Heating.TempBoilerTop()-Heating.SetpointTempWarmWater)/(Heating.TempBoilerTop()-8.1560);
//      float beta = 1;// - 1.5*theta;
      Heating.PumpWarmWater.setMassFlowRate(Output*Heating.PumpWarmWater.getMaxMassFlowRate());
     
//      Heating.PumpWarmWater.setMassFlowRate(Heating.FlowMeter.get());
     
   }

  }
  else
  {
    // stop pump
    Heating.PumpWarmWater.setMassFlowRate(0.0);
    // close valve
    Heating.ValveWarmWater.set(false);
  }
  //***************************
  
  
  //SerialDebug();
  DataAcquisition(false);
  CheckTime();
}


void incCounter()
{
  Heating.FlowMeter.incCounter();
}



void DataAcquisition(boolean bfirstRun)
{
  // Header
  if (bfirstRun) {
    Serial.println("Time ; _Power% ; PumpMassFlow ; FlowMeter ; TempWarmWater ; TempBoilerTop ; TempBoilerReturn ; PID% ;");
  }
  
  unsigned long time = millis();
  if(trigger)
  {
    printValue(time);
    printValue(Heating.PumpWarmWater._Power);
    printValue(Heating.PumpWarmWater.getMassFlowRate());
    printValue(Heating.FlowMeter.get());
    printValue(Heating.TempWarmWater());
    printValue(Heating.TempBoilerTop());
    printValue(Heating.TempCirculationReturn());
    printValue(Output);
    //printValue();
    
    Serial.println(); //carriage return and new line
  }
}

void printValue(float fValue)
{
  Serial.print(fValue,DEC);
  Serial.print(" ; "); // separator
}

void SerialDebug()
{
  unsigned long time = millis();
//   if(trigger)
  {
//  Serial.println("************");
//  Serial.println(PumpState, DEC);
//***********    Write Counter of Warmwater flow rate ***********
    Serial.println("Testing Heat Exchanger function. Values should be equal to the ones in brackets");
    Serial.println("Soll Cr Pump WarmWater 100C (1.9786)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 100.0, 1.0), DEC);
    Serial.println("Soll Cr Pump WarmWater 90C (1.9786)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 90.0, 1.0), DEC);
    Serial.println("Soll Cr Pump WarmWater 80C (1.5109)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 80.0, 1.0), DEC);
    Serial.println("Soll Cr Pump WarmWater 70C (1.2628)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 70.0, 1.0), DEC);
    Serial.println("Soll Cr Pump WarmWater 60C (0.9790)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 60.0, 1.0), DEC);
    Serial.println("Soll Cr Pump WarmWater 55C (0.6681)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 55.0, 1.0), DEC);
    Serial.println("Soll Cr Pump WarmWater 52C (0.6681)");
    Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 52.0, 1.0), DEC);
//***********    Write Systemtemperatures for calibration ***********
  }
}

void CheckTime()
{
  unsigned long time = millis();
  if(time>LastTime+TimePeriod)
  {
    trigger = true;
    LastTime = time;
  }
  else {
    trigger = false;
  }
}


