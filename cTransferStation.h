#ifndef cTransferStation_h
#define cTransferStation_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"
#include <cPID.h>
#include <ArduinoJson.h>

class cTransferStation : public PID
{
	public:
	
	cTransferStation():
	PID(&Is, &Power, &Setpoint, 0.003, 0.0001, 0.001, DIRECT),
	TempFromTeleheating(&MPNumSys[0], &MPChanSys[idxTempTransferStationFromTeleheating], &SysTempOffset[idxTempTransferStationFromTeleheating]),
	TempOperation(&MPNumSys[0], &MPChanSys[idxTempTransferStationOperation], &SysTempOffset[idxTempTransferStationOperation]),
	Valve(PinValveTransferStation)
	{
		sufficientHeat = false;
		pinMode(PinFlowRegulator, OUTPUT);
		
		Power = 0.0;
		Setpoint = 0.0;
		SetOutputLimits(0.0, 0.95);
		Compute();
	}
	
	boolean tap(double SpTempSource, boolean enable, boolean disable = false)
	{
	double IsTempkorr = (TempOperation.getRaw()-(2.2*min(1.7,(SpTempSource-TempOperation.getRaw()))));
	Is = IsTempkorr;

	Setpoint = SpTempSource;
		
		if (enable)
		
		{
		if (GetMode()==MANUAL)
			{
				Power = max(0.5,(min(95,((95-10)/(20-90)*(TempFromTeleheating.getRaw()-90)+10))/100)); //Starting Power dependent upon incoming Temperature
			}

		SetMode(AUTOMATIC);
		}
		else
		{
			Power = 0.0;
			SetMode(MANUAL);
		}
		Compute();
		analogWrite(PinFlowRegulator, int(round(255*Power)));
		
		// Check for residual/sufficient heat with hysteresis
		if ((sufficientHeat == true) && (TempOperation.getRaw() < (SpTempSource-1)))
		sufficientHeat = false;
		if ((sufficientHeat == false) && ((TempOperation.getRaw() > SpTempSource) || (TempFromTeleheating.getRaw()) > SpTempSource-8) && enable)
		sufficientHeat = true;

		// Open valve if sufficient heat is available
		Valve.set(sufficientHeat&&(!disable));
		
		// Return if there is residual heat so that charging of boiler continues
		return sufficientHeat;
	}

  float TempSource()
  {
    return(TempOperation.getRaw());
  }
	
	void getData(JsonObject& root);
	
	boolean sufficientHeat;
	
	boolean enable(void);
	boolean disable(void);
	
	cTempSensor TempFromTeleheating;
	cTempSensor TempOperation;
	
	private:
	
	double Is;
	double Setpoint;
	double Power;
	
	cValve Valve;
};

#endif
