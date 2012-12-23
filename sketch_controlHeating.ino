#include "Arduino.h"
#include "PinDefinitions.h"
#include "Temperatures.h"
#include "cPump.h"
#include "cRoom.h"
#include "cHeating.h"

// Simple Vorlauftemperatur reference
float TempHeatingLeadRef = 42.0;


#define LiterPerImpuls 0.009263548

#define TimePeriod 5000
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


/**
 * @fn void setup(void)
 * @brief Initializes the serial connection and defines the in- and outputs.
 */
void  setup()
{
  Serial.begin(57600);
  //Serial.println("Steuerung");
  attachInterrupt(4,incCounter,RISING);
  delay(2000);// Give reader a chance to see the output.
  analogReference(DEFAULT);
  PinInitialization();
  DataAcquisition(true);
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
//    
//    // open Valve
//    Heating.ValveWarmWater.set(true);
//    // Test Massflow rate calculation
//    fMassFlowPumpWarmWater = Heating.HxWarmWater.calcMassflow(25.0, 8.0, 57.0, Heating.FlowMeter.get() );
//    Heating.PumpWarmWater.setMassFlowRate(fMassFlowPumpWarmWater);
//    // timeoptimal control until 5 degrees below Setpoint
//    if(Heating.TempWarmWater()<=Heating.SetpointTempWarmWater - 5)   
//    {
//      // Set Pump to maximum mass flow rate
//    //  Heating.PumpWarmWater.setMassFlowRate(Heating.PumpWarmWater.getMaxMassFlowRate());
//    }
//    // open loop control
//    else
//    {
//      //Heating.PumpWarmWater.setMassFlowRate(Heating.HxWarmWater.calcMassflow(50.0, 8.0, Heating.TempBoilerTop(), Heating.FlowMeter.get() ));
//      
//    }
    // open Valve
    Heating.ValveWarmWater.set(true);
    float MaxMassFlowRate = Heating.PumpWarmWater.getMaxMassFlowRate();
    float PowerPercent= 0.1;
    Heating.PumpWarmWater.setMassFlowRate(PowerPercent*MaxMassFlowRate);

  }
  else
  {
    Heating.PumpWarmWater.setMassFlowRate(0.0);
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
    Serial.println("Time ; _Power ; PumpMassFlow ; FlowMeter ; TempWarmWater ; TempBoilerTop ; TempBoilerReturn ;");
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
  if(trigger)
  {
//  Serial.println("************");
//  Serial.println(PumpState, DEC);
//***********    Write Counter of Warmwater flow rate ***********
     Serial.println("Maximum Massflow Pump WarmWater");
     Serial.println(Heating.PumpWarmWater.getMaxMassFlowRate(), DEC);
     Serial.println("Soll MassFlow Pump WarmWater");
     Serial.println(fMassFlowPumpWarmWater, DEC);
     Serial.println("Ist MassFlow Pump WarmWater");
     Serial.println(Heating.PumpWarmWater.getMassFlowRate(), DEC);
     Serial.println("Ist MassFlow FlowMeter");
     Serial.println(Heating.FlowMeter.get(), DEC);
     Serial.println("Counter");
     Serial.println(Heating.FlowMeter._iCounter, DEC);
    // Serial.println("Testing Heat Exchanger function. Values should be equal to the ones in brackets");
    // Serial.println("Soll Cr Pump WarmWater 100C (1.9786)");
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 100.0, 1.0), DEC);
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 90.0, 1.0), DEC);
    // Serial.println("Soll Cr Pump WarmWater 80C (1.5109)");
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 80.0, 1.0), DEC);
    // Serial.println("Soll Cr Pump WarmWater 70C (1.2628)");
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 70.0, 1.0), DEC);
    // Serial.println("Soll Cr Pump WarmWater 60C (0.9790)");
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 60.0, 1.0), DEC);
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 55.0, 1.0), DEC);
    // Serial.println("Soll Cr Pump WarmWater 52C (0.6681)");
    // Serial.println(Heating.HxWarmWater.calcMassflow(50.0, 8.0, 52.0, 1.0), DEC);
//***********    Write Systemtemperatures for calibration ***********
      Serial.println("TempWarmWater");
      Serial.println(Heating.TempWarmWater(), DEC);
      Serial.println("TempBoilerTop");
      Serial.println(Heating.TempBoilerTop(), DEC);
//      Serial.println("Print All System Temp. for Debug");
//      Serial.println("TempHeatingLead");
//      Serial.println(Heating.TempHeatingLead(), DEC);
//      Serial.println("TempHeatingReturn");
//      Serial.println(Heating.TempHeatingReturn(), DEC);
//      Serial.println("TempHeatSource1Lead");
//      Serial.println(Heating.TempHeatSource1Lead(), DEC);
//      Serial.println("TempHeatSource1Return");
//      Serial.println(Heating.TempHeatSource1Return(), DEC);
//      Serial.println("TempHeatSource1Operation");
//      Serial.println(Heating.TempHeatSource1Operation(), DEC);
//      Serial.println("TempSolarReturn");
//      Serial.println(Heating.TempSolarReturn(), DEC);
//      Serial.println("TempSolarLead");
//      Serial.println(Heating.TempSolarLead(), DEC);
//      Serial.println("TempBoilerCharge");
//      Serial.println(Heating.TempBoilerCharge(), DEC);
//      Serial.println("TempBoilerReserve1");
//      Serial.println(Heating.TempBoilerReserve1(), DEC);
//      Serial.println("TempBoilerReserve2");
//      Serial.println(Heating.TempBoilerReserve2(), DEC);
//      Serial.println("TempBoilerHead");
//      Serial.println(Heating.TempBoilerHead(), DEC);
//      Serial.println("TempBoilerTop");
//      Serial.println(Heating.TempBoilerTop(), DEC);
//      Serial.println("TempWarmWater");
//      Serial.println(Heating.TempWarmWater(), DEC);
//      Serial.println("TempCirculationReturn");
//      Serial.println(Heating.TempCirculationReturn(), DEC);
//      Serial.println("IntensitySolar");
//      Serial.println(Heating.IntensitySolar(), DEC);
//      Serial.println("TempOutside");
//      Serial.println(Heating.TempOutside(), DEC);


//       Serial.println("TempRoom12 actual temperature");
//       Serial.println(Heating.Rooms[12].getTempActual(), DEC);
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


