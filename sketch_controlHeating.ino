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
#define NWarmWaterHeatExchanger 4.3

//! Counter for warm water
int iCounter = 0;
int iLastCounter = 0;
unsigned long LastTimeCounter = 0;
float fmassFlux = 0.0;
float fMassFlux = 0.0;
float fTempRefSchedule[16]={20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                    20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0};

unsigned long LastTime = 0;

float PumpState =0;

cHeating Heating;


/**
 * @fn void setup(void)
 * @brief Initializes the serial connection and defines the in- and outputs.
 */
void  setup()
{
  Serial.begin(57600);
  Serial.println("Steuerung");
  attachInterrupt(4,incCounter,RISING);
  //attachInterrupt(19,incCounter,RISING);
  delay(2000);// Give reader a chance to see the output.
  analogReference(DEFAULT);
  PinInitialization();
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
  if(Heating.FlowMeter.get()>0.0)
  {
    // open Valve
    Heating.ValveWarmWater.set(true);
    // Heating.PumpWarmWater.set(0.2);

    // timeoptimal control until 5 degrees below Setpoint
    if(Heating.TempWarmWater()<=Heating.SetpointTempWarmWater - 5)   
    {
      // Set Pump to maximum mass flow rate
      Heating.PumpWarmWater.setMassFlowRate(Heating.PumpWarmWater.getMaxMassFlowRate());
    }
    // open loop control
    else
    {
      
      // Newton procedure to compute the necessary Massflow
      //Heating.PumpWarmWater.set();
    }
  }
  else
  {
    Heating.PumpWarmWater.setMassFlowRate(0.0);
    Heating.ValveWarmWater.set(false);
  }
  //***************************
  
  SerialDebug();
  CheckTime();
}


void incCounter()
{
  Heating.FlowMeter.incCounter();
  //iCounter = iCounter +1;
}



void SerialDebug()
{
  unsigned long time = millis();
  if(time>LastTime+TimePeriod)
  {
//  Serial.println("************");
//  Serial.println(PumpState, DEC);
  //***********    Write Counter of Warmwater flow rate ***********
        Serial.println("PumpWarmWater");
      Serial.println(Heating.PumpWarmWater.getMassFlowRate(), DEC);
  Serial.println("fmassflux");
    Serial.println(fmassFlux, DEC);
    //Serial.println("Counter");
    //Serial.println(flowmeter._iCounter, DEC);
//***********    Write Systemtemperatures for calibration ***********
      Serial.println("TempWarmWater");
      Serial.println(Heating.TempWarmWater(), DEC);
      Serial.println("TempBoilerTop");
      Serial.println(Heating.TempBoilerTop(), DEC);


      //Serial.println("TempRoom12");
      //Serial.println(Heating.Rooms[12].getTempActual(), DEC);
  }
}

void CheckTime()
{
  unsigned long time = millis();
  if(time>LastTime+TimePeriod)
  {
    Serial.println("********");
    LastTime = time;
  }
}


