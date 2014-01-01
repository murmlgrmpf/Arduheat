#include "cRoomValve.h"

/// Creates a valve object without setting the pins and closed state (false). This is necessary for the rooms.
cRoomValve::cRoomValve(void)
{
  _iPinOpen=0;
  _iPinClose=0;
  _bState = false;
}

 cRoomValve::cRoomValve( int iPinOpen )
{
	cRoomValve::setPinOpen(iPinOpen);
	_iPinClose=0;
	
	set(false);
}


/// Creates a valve object with pin setting, the initial state is close.
/** Creates a valve object with pin setting. The Valve is initialized with a closed state. */
cRoomValve::cRoomValve(int iPinOpen, int iPinClose)
{
  cRoomValve::setPinOpen(iPinOpen);
  cRoomValve::setPinClose(iPinClose);
  
  set(false);
}

/// Creates a valve object with pin setting and an initial state.
cRoomValve::cRoomValve(int iPinOpen, int iPinClose, boolean bState)
{
	cRoomValve::setPinOpen(iPinOpen);
	cRoomValve::setPinClose(iPinClose);
	
	set(bState);
}

/// Sets the state of the Valve and executes the run function of the valve.
void cRoomValve::set(boolean bState)
{
  _bState = bState;
  // Execute state of valve
  digitalWrite(_iPinOpen, !_bState); // Valve gets opened on low/false
}

/// Sets the _PinOpen. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cRoomValve::setPinOpen(int iPinOpen)
{
  _iPinOpen = iPinOpen;
  pinMode(_iPinOpen, OUTPUT);
  
  set(_bState);
}

/// Sets the _PinClose. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cRoomValve::setPinClose(int iPinClose)
{
  _iPinClose = iPinClose;
  pinMode(_iPinClose, OUTPUT);
  
  set(_bState);
}

/// Reads the state of the valve.
/** \return True = open, False = closed. */
boolean cRoomValve::get(void)
{
  return _bState;
}