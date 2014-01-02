#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cPump.h"
#include "cRoom.h"
#include "cHeating.h"
#include "cWarmWater.h"
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

cWarmWater WarmWater;
cHeating Heating;


/**
 * @fn void setup(void)
 * @brief Initializes the serial connection and defines the in- and outputs.
 */
void  setup()
{
  Serial.begin(115200);
  Serial.println("Steuerung");
  attachInterrupt(4,incCounter,RISING);
  PinInitialization();
  delay(2000);// Give reader a chance to see the output.
  analogReference(DEFAULT);
  
  DataAcquisition(true);
  // Overwrite Setpoint Temperature for WarmWater
  //Heating.SpTempWarmWater = 45.0;

}




/**
 * @fn void loop(void)
 * @brief Runs the main program which controls the system.
 */
void loop()
{
   Heating.Control();
   WarmWater.Control();
  
  DataAcquisition(false);
  CheckTime();
}


void incCounter()
{
   WarmWater.FlowMeter.incCounter();
}



void DataAcquisition(boolean bfirstRun)
{
  // Header
  if (bfirstRun) {
    Serial.println("Time; Endtime; _bFLame; TempBoilerTop; SpTempChargeWarmWater; TempBoilerHead; TempBoilerCharge; PID%; haveWarmWater; needWarmWater; SpTempChargeHeating");
        printValue(Heating.Burner._bFlame);
  }
  unsigned long endtime = 0;
  unsigned long time = millis();
  if(trigger)
  {
    printValue(time);
    endtime = Heating.Burner._StartTime+Heating.Burner._MinBurnTime;
    printValue(endtime);
    printValue(Heating.Burner._bFlame);
    printValue(Heating.Boiler.TempBoilerTop.get());
    printValue(Heating.Boiler.SpTempChargeWarmWater());
    printValue(Heating.Boiler.TempBoilerHead.get());
    printValue(Heating.Boiler.TempBoilerCharge.get());
    printValue(Heating.Boiler.PumpBoilerCharge.Power);
    printValue(Heating.Boiler.haveWarmWater());
    printValue(Heating.Boiler.needWarmWater());
    printValue(Heating.Boiler.needHeating());
    printValue(Heating.Boiler.SpTempChargeHeating());

    
    //printValue();
    
    Serial.println(); //carriage return and new line
  }
}

void printValue(float fValue)
{
  Serial.print(fValue,DEC);
  Serial.print(" ; "); // separator
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


