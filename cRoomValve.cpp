#include "cRoomValve.h"

/// Creates a valve object without setting the pins and closed state (false). This is necessary for the rooms.
cRoomValve::cRoomValve(void)
{
  _iPinOpen=0;
  _iPinClose=0;
  _bState = false;
}

/// Creates a valve object with pin setting, the initial state is close.
/** Creates a valve object with pin setting. The Valve is initialized with a closed state. */
cRoomValve::cRoomValve(int iPinOpen, int iPinClose)
{
  cRoomValve::setPinOpen(iPinOpen);
  cRoomValve::setPinClose(iPinClose);
  
  _bState = false;
}

/// Creates a valve object with pin setting and an initial state.
cRoomValve::cRoomValve(int iPinOpen, int iPinClose, boolean bState)
{
  cRoomValve::setPinOpen(iPinOpen);
  cRoomValve::setPinClose(iPinClose);
  
  _bState = bState;
}

/// Sets the state of the Valve and executes the run function of the valve.
void cRoomValve::set(boolean bState)
{
  _bState = bState;
  
  cRoomValve::run();
}

/// Sets the _PinOpen. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cRoomValve::setPinOpen(int iPinOpen)
{
  _iPinOpen = iPinOpen;
  pinMode(_iPinOpen, OUTPUT);
  
  cRoomValve::run();
}

/// Sets the _PinClose. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cRoomValve::setPinClose(int iPinClose)
{
  _iPinClose = iPinClose;
  pinMode(_iPinClose, OUTPUT);
  
  cRoomValve::run();
}

/// Reads the state of the valve.
/** \return True = open, False = closed. */
boolean cRoomValve::get(void)
{
  return _bState;
}

/// Executes the state of the valve (opens or closes it).
/** If the state of the valve is true, it shall be opened, if it is false, it is going to be closed*/
void cRoomValve::run(void)
{
  if(_bState==true)
  {
    digitalWrite(_iPinOpen, LOW); // Valve gets openedPinValveWarmWaterOpen
  }
  else
  {
    digitalWrite(_iPinOpen, HIGH); // Valve gets closed
  }
}
