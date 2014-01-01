#include "cValve.h"

/// Creates a valve object with pin setting and period, the initial state is close.
/** Creates a valve object with pin setting. The Valve is initialized with a closed state. */
cValve::cValve(int iPinOpen, int iPinClose, unsigned long iTimePeriod)
{
  _iPinOpen=iPinOpen;
  _iPinClose=iPinClose;
  
  _iTimeStart = millis();
  _iTimePeriod = iTimePeriod;
  
  _bisOpen = false;
  set(false);
}

/// Sets the state of the Valve and executes the run function of the valve.
void cValve::set(boolean bsetOpen)
{
  _bsetOpen = bsetOpen;
  
  cValve::run();
}

/// Reads the is state of the valve.
/** \return True = open, False = closed. */
boolean cValve::get(void)
{
  return _bisOpen;
}

/// Executes the set state of the valve (opens or closes it).
/** If the set state of the valve is true, it shall be opened, if it is false, it is going to be closed*/
void cValve::run(void)
{
	// Reset timer and start driving if state of valve has changed
	if(_bsetOpen!=_bisOpen)
	{
		_iTimeStart = millis();
		_bisOpen = _bsetOpen;
	}

	// Drive the valve
	if(millis() < _iTimeStart + _iTimePeriod)
	{
		// drive valve motor to open direction if _bsetOpen is true/high
		// otherwise drive to close
		digitalWrite(_iPinOpen, !_bsetOpen);
		digitalWrite(_iPinClose, _bsetOpen);

	}
	else  // stop motor
	{
		// stop valve motor
		digitalWrite(_iPinOpen, LOW);
		digitalWrite(_iPinClose, LOW);
	}
}
