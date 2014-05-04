#include "cRoom.h"


cRoom::cRoom(void)
:pid(0.1, 0, 0, DIRECT)
{
	_dIsTemp = 0;
	_need = 0;
	_dSpTemp = 20.0;
	_dSpTempSchedule = 20.0;
	setRoomNumber(1);
}

cRoom::cRoom( int iRoomNumber ) /*~cRoom()*/
:pid(0.1, 0, 0, DIRECT),
Valve(RoomValvePin[iRoomNumber-1])
{
	init(iRoomNumber);
}


void cRoom::init( int iRoomNumber )
{
	_dIsTemp = 0;
	_need = 0;
	_dSpTemp = 20.0;
	_dSpTempSchedule = 20.0;
	setRoomNumber(iRoomNumber);
	//Set the pin of the valve according to pinout scheme.
	Valve.setPinOpen(RoomValvePin[iRoomNumber-1]);
	
	pid.SetOutputLimits(0, 1);
}


void cRoom::setRoomNumber(int iRoomNumber)
{
	int _iMultiplexNumber;
	int _iMultiplexConnectorIs;
	int _iMultiplexConnectorSp;
	
	// Set RoomNumber
	_iRoomNumber = iRoomNumber;
	
	//**********************
	// Calculate pinout
	_iMultiplexNumber = 1;
	_iMultiplexConnectorIs = 2*(_iRoomNumber);
	if (_iMultiplexConnectorIs>16)
	{
		_iMultiplexConnectorIs = _iMultiplexConnectorIs -16;
		_iMultiplexNumber =2;
	}
	// Adaption to multiplexer pinout
	_iMultiplexConnectorIs = 16-_iMultiplexConnectorIs;
	_iMultiplexConnectorSp = _iMultiplexConnectorIs +1;
	//**********************
	
	//IsTemp.set(_iMultiplexNumber,_iMultiplexConnectorIs,RoomIsOffset[iRoomNumber-1]);
	IsTemp.set(_iMultiplexNumber,_iMultiplexConnectorIs,0);
	SpTemp.set(_iMultiplexNumber,_iMultiplexConnectorSp,RoomSpOffset[iRoomNumber-1]);
	
}

double cRoom::getSpTempSchedule(void)
{
	return _dSpTempSchedule;
}

void cRoom::setSpTempSchedule(double TempRefSchedule)
{
	_dSpTempSchedule = TempRefSchedule;
}

double cRoom::getIsTemp(void)
{
	_dIsTemp = IsTemp.get();
	return _dIsTemp;
}

double cRoom::getSpTemp(void)
{
	// Check if manual override is valid
	if((SpTemp.get()<TempHigh)&&(SpTemp.get()>TempLow))
	{
		_dSpTemp = SpTemp.get();
	}
	else
	{
		_dSpTemp = getSpTempSchedule();
	}
	
	return _dSpTemp;
}

double cRoom::getNeed(void)
{
	// Compute need
	_need = pid.run(getSpTemp(),getIsTemp());
	
	// Open Valve if heat is needed
	Valve.set((_need>0));
	
	return _need;
}


cRooms::cRooms( void ):
IsTempHeatingLead(SystemMultiplexer,MultiplexTempHeatingLead,OffsetTempHeatingLead),
IsTempHeatingReturn(SystemMultiplexer,MultiplexTempHeatingReturn,OffsetTempHeatingReturn),
PIDPumpHeating( 0.2, 0.1, 0.05, DIRECT),
PIDMixer( 0.5, 0.0, 0.005, DIRECT),
PumpHeating(PinPumpHeating)
{
	// Initialize PID controllers for pumps
	PIDPumpHeating.SetOutputLimits(0.4, 1.0);
	
	PIDMixer.SetOutputLimits(-1.0, 1.0);
	PIDMixer.SetSampleTime(500);
	
	// Initialize room numbers and Pins
	for(int i = 0; i<16; i++)
	{
		// The rooms know their multiplexers and pin out by the room number
		Room[i].init(i+1);
	}
	
	dMaxDiff =0;
	dMaxSp = 0;
	dneedChargeRooms = 0;
}

void cRooms::ChargeRooms( boolean bneedChargeRooms, boolean BoilerCharges )
{
	_dSpTempHeatingLead = getSpHeating();
	_dIsTempHeatingLead = IsTempHeatingLead.get();
	_dSpTempHeatingReturn = _dIsTempHeatingLead-5;
	_dIsTempHeatingReturn = IsTempHeatingReturn.get();
	
	if (bneedChargeRooms)
	{
		// Run Mixer and PID
		Mixer.run(PIDMixer.run(_dSpTempHeatingLead, _dIsTempHeatingLead));
		
		// Run heating pump and PID
		if (BoilerCharges)
		{
			PIDPumpHeating.run();
			PumpHeating.setPower(1.0);
		}
		else
		{
			PumpHeating.setPower(PIDPumpHeating.run(_dSpTempHeatingReturn, _dIsTempHeatingReturn));
		}
	}
	else
	{
		// Stop Pump Heating and PID
		PIDPumpHeating.run();
		PumpHeating.setPower(0.0);
		// Stop mixer PID and run Mixer to closed position
		PIDMixer.run();
		Mixer.run(-1.0);
	}
}

boolean cRooms::need()
{
	// Read state of rooms
	for(int i = 0; i<nRooms; i++)
	{
		// Set setpoints
		Room[i].setSpTempSchedule(10);
		if(i==3)
		{
			Room[i].setSpTempSchedule(20);
		}
		
		// Store maximum set point
		if (Room[i].getSpTemp()>dMaxSp)
		{
			dMaxSp = Room[i].getSpTemp();
		}
		// Store maximum sp-is difference
		if ((Room[i].getSpTemp() - Room[i].getIsTemp()) > dMaxDiff)
		{
			dMaxDiff = Room[i].getSpTemp() - Room[i].getIsTemp();
		}
		// Check for rooms needing heat
		if(Room[i].getNeed()>dneedChargeRooms)
		{
			dneedChargeRooms = Room[i].getNeed();
		}
	}
	
	return (dneedChargeRooms>0);
}

double cRooms::getSpHeating()
{
	need();
	return SpHeating.get(dMaxSp, dMaxDiff);
}
