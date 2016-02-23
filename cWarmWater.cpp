#include "cWarmWater.h"

void cWarmWater::Control(void){
	// If Warm Water is needed, execute the pump
	// with the power determined by pid controller.
	// Else switch pid to manual and stop pump.
	if(!digitalRead(PinWarmWaterSwitch)){
            if (SpTemp-5.0 > IsTemp.get()) // Time optimal control
                Pump.run(1.0);
            else{
                if (not(Pump.GetMode())) //  when switching from manual mode
                    Pump.run(0.20); // add static offset for Power
            
                Pump.run(SpTemp, IsTemp.get()); // switch to automatic mode
            }
            Period.restart();
	}
	else
		Pump.run(); // manual mode
	
 	if(Trigger.get())
 		setParam();
}

void cWarmWater::setParam()
{
	// double P = min(0.5,max(0.1, 1.127-0.011*IsTempBoilerTop.get()));
  double P = 0.079;
  // double D = min(7.0,max(2.6,17.9-0.19*IsTempBoilerTop.get()));
  double D = 0.52; 
        // ax+b; 90 == 0.5;  55==1.0;
        // a = (0.5-1.0)/(90-60) = -0.0166; b = 1+0.0166*60 = 2
        // double MaxPower = min(1.0,max(0.5,2-0.0166*IsTempBoilerTop.get()));
        double MaxPower = 1.0;
	// SpTemp = max(45, 0.143*IsTempBoilerTop.get() + 37.1);
  SpTemp = 45; 
	Pump.SetTunings(P, Pump.GetKi(), D);
        Pump.SetOutputLimits(0.0, MaxPower);
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
	root["WTi"] =  IsTemp.get();
	
	root["Wn"] = static_cast<int>(!digitalRead(PinWarmWaterSwitch));
	root["WTitoB"] = IsTempWarmWaterToBoiler.get();
}
