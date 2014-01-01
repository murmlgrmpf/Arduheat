#include "cRoom.h"


cRoom::cRoom(void)
:pid(&_dIsTemp, &_need, &_dSpTemp, 0.01, 0.0, 0.05, DIRECT)
{
	_dIsTemp = 0;
	_dSpTemp = 0;
	_need = 0;
	_dSpTemp = 20.0;
	_dSpTempSchedule = 20.0;
	setRoomNumber(1);
}

 cRoom::cRoom( int iRoomNumber ) /*~cRoom()*/
 :pid(&_dIsTemp, &_need, &_dSpTemp, 0.01, 0.0, 0.05, DIRECT),
 Valve(RoomValvePin[iRoomNumber-1])
 {
	 init(iRoomNumber);
 }


void cRoom::init( int iRoomNumber )
{
	_dIsTemp = 0;
	_dSpTemp = 0;
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
	getSpTemp();
	getIsTemp();
	// Compute need
	pid.SetMode(AUTOMATIC);
	pid.Compute();
	
	// Open Valve if heat is needed
	Valve.set((_need>0));
	
	return _need;
}
