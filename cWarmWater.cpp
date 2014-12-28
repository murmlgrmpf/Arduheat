#include "cWarmWater.h"

cWarmWater::cWarmWater(void):
pid(0.5, 0.01, 0.1, DIRECT),
IsTemp(SystemMultiplexer,MultiplexTempWarmWater,OffsetTempWarmWater),
Pump(PinPumpWarmWater)
{
	TempSchedule[0].time.set(0, 6,0,0);
	TempSchedule[0].temp = SpTempWarmWater;
	TempSchedule[1].time.set(0,20,0,0);
	TempSchedule[1].temp = SpTempWarmWater + SpTempWarmWaterLower;
	// Initialize PID controllers for pump
	pid.SetOutputLimits(0.0, 1.0);
}

void cWarmWater::Control(){
	// Warm Water needed
	if(digitalRead(PinWarmWaterSwitch))
	{
		// Execute the pump with the power determined by pid controller
		Pump.setPower(pid.run(getSpTemp(), IsTemp.get()));
	}
	else
	{
		// switch pid to manual
		pid.run();
		// stop pump
		Pump.setPower(0.0);
	}
}

void cWarmWater::getSP( JsonObject& root )
{
	root["WWTempSchedule0time"] = TempSchedule[0].time.totalseconds();
	root["WWTempSchedule0temp"] = TempSchedule[0].temp;
	root["WWTempSchedule1time"] = TempSchedule[1].time.totalseconds();
	root["WWTempSchedule1temp"] = TempSchedule[1].temp;
}

int cWarmWater::setSP( JsonObject& root )
{
	int fail = 0;
	int posReturn =0;
	
	if(root.containsKey("WWTempSchedule0time")) {
		if(root["WWTempSchedule0time"].is<long>()) {
			TempSchedule[0].time.set(root["WWTempSchedule0time"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WWTempSchedule0temp")) {
		if(root["WWTempSchedule0temp"].is<double>()) {
			TempSchedule[0].temp = root["WWTempSchedule0temp"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WWTempSchedule1time")) {
		if(root["WWTempSchedule1time"].is<long>()) {
			TempSchedule[1].time.set(root["WWTempSchedule1time"].as<long>());
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	if(root.containsKey("WWTempSchedule1temp")) {
		if(root["WWTempSchedule1temp"].is<double>()) {
			TempSchedule[1].temp = root["WWTempSchedule1temp"].as<double>();
			posReturn++;
		}
		else fail=1;
	}
	else fail=1;
	
	if (fail == 0) { // If all three parameter objects were successfully filled
		return posReturn;
	}
	else
	{
		return 0;
	}
}

void cWarmWater::getData( JsonObject& root )
{
	root["WarmWaterPump"] = Pump.getPower();
	root["WarmWaterIsTemp"] =  IsTemp.get();
}

double cWarmWater::getSpTemp( void )
{
	double _SpTemp = 0;
	TimeSpan rel;
	rel.set(0, TimeNow.hour(), TimeNow.minute(), 0);
	
	for(int iSwitch=0; iSwitch<2; iSwitch++)
	{
		if (TempSchedule[iSwitch].time.totalseconds() < rel.totalseconds())
		_SpTemp = TempSchedule[iSwitch].temp;
	}
	return _SpTemp;
}
