#include "cRoom.h"


cRoom::cRoom(void)
{
  _iRoomNumber =1;
  _fTempActual = 20.0;
  _fTempRef = 20.0;
  _iMultiplexNumber = 0;
  _iMultiplexConnectorActual = 0;
  _iMultiplexConnectorRef = 0;
  //cValve Valve();
  
  //cRoom::readTemp();
}

void cRoom::setRoomNumber(int iRoomNumber)
{
  // Set RoomNumber
  _iRoomNumber = iRoomNumber;
  
  // Calculate Pinout
  _iMultiplexNumber = 1;
  _iMultiplexConnectorActual = 2*(_iRoomNumber);
  if (_iMultiplexConnectorActual>16)
  {
    _iMultiplexConnectorActual = _iMultiplexConnectorActual -16;
    _iMultiplexNumber =2;
  }
  // Adaption to multiplexer pinout
  _iMultiplexConnectorActual = 16-_iMultiplexConnectorActual;
  _iMultiplexConnectorRef = _iMultiplexConnectorActual +1;
  
  // Set the pin of the valve according to pinout scheme.
  Valve.setPinOpen(iRoomNumber+25);
}

void cRoom::setTempRefSchedule(float fTempRefSchedule)
{
  _fTempRefSchedule = fTempRefSchedule;
}

float cRoom::getTempActual(void)
{
  _fTempActual =0.7*_fTempActual + (1-0.7)*readTemperature(_iMultiplexNumber, _iMultiplexConnectorActual);
  return _fTempActual;
}

float cRoom::getTempRef(void)
{
  _fTempRef = readTemperature(_iMultiplexNumber, _iMultiplexConnectorRef);
  
  // Check if manual override is not valid
  if(!((_fTempRef<TempHigh)&&(_fTempRef>TempLow)))
  {
    _fTempRef = _fTempRefSchedule;
  }
  
  return _fTempRef;
}

float cRoom::getHeatflow(void)
{
  cRoom::getTempActual();
  cRoom::getTempRef();
  _Heatflow = _fTempRef - _fTempActual;
  
  return _Heatflow;
}
