#include "cWarmWater.h"

void cWarmWater::Control(void){
	// If Warm Water is needed, execute the pump with the static power plus power determined by PID controller.
	// Else switch PID to manual and stop pump.
	WarmWaterFlow = analogRead(PinWarmWaterFlow)/1023.0*Vcc*76/6-26/6; // WarmWaterFlow determined by Sensor converted to Liters/min


		double Factor1 = (0.6-1)/(75-50)*(IsTempBoilerTop.get()-50)+1; // Consideration of Heat Exchanger Behaviour at higher Boiler Temperatures
		double Factor2 = (1.6-1)/(75-50)*(IsTempBoilerTop.get()-50)+1;

	double Kwp = Factor1*((Factor2*0.025-0.0139)/(15-2.6)*(WarmWaterFlow-2.6)+0.0139); // Adaption of Pump Power to required primary Flow by PowerStatic value


	if (WarmWaterFlow > 2.0){
		// Modeling of Heat exchanger with proportional correction
		PowerStatic = Kwp*WarmWaterFlow*((SpTemp+11.5*min(2.2,(SpTemp-IsTemp.getRaw())))-IsTempWaterFromOutside.getRaw())/((max(30,IsTempWarmWaterFromBoiler.getRaw()))-19);
		Pump.run(SpTemp, max(45.0,IsTemp.getRaw()), PowerStatic);
		Period.restart();
	}
	else{
	Pump.run(); // manual mode (stop pump)
	PowerStatic = 0;	
	}
}

void cWarmWater::getSP( JsonObject& root )
{
	root["WTs"] = SpTemp;
}

int cWarmWater::setSP( JsonObject& root )
{
	if(root.containsKey("WTs")) {
		if(root["WTs"].is<double>()) {
			double SpTemp_ = root["WTs"].as<double>();
		if ((SpTemp_>40)&&(SpTemp_<60)&&(SpTemp_!=SpTemp)) {
			SpTemp = SpTemp_;
		return true;
	}
		}
	}
	
	return false;
}

void cWarmWater::getData( JsonObject& root )
{
	root["WP"] = Pump.get();
	root["WTi"] = IsTemp.getRaw();
	root["Wn"] = WarmWaterFlow;
	root["WS"] = PowerStatic;
	root["WG"] = Pump.get()+PowerStatic;		
	root["WTifromB"] = IsTempWarmWaterFromBoiler.getRaw();
	root["WTifromO"] = IsTempWaterFromOutside.getRaw();
}
