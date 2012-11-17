#include "cValve.h"

/// Creates a valve object with pin setting, the initial state is close.
cValve::cValve(int iPinOpen, int iPinClose)
{
  _iPinOpen=iPinOpen;
  _iPinClose=iPinClose;
  
  _iTimePeriod = 1500;
  
  _bsetOpen = false;
  _bisOpen = false;
  _bisDriving = false;
}

/// Creates a valve object with pin setting, the initial state is close.
/** Creates a valve object with pin setting. The Valve is initialized with a closed state. */
cValve::cValve(int iPinOpen, int iPinClose, unsigned long iTimePeriod)
{
  _iPinOpen=iPinOpen;
  _iPinClose=iPinClose;
  
  _iTimePeriod = iTimePeriod;
  
  _bsetOpen = false;
  _bisOpen = false;
  _bisDriving = true;
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

/// Executes the state of the valve (opens or closes it).
/** If the set state of the valve is true, it shall be opened, if it is false, it is going to be closed*/
void cValve::run(void)
{
  unsigned long time = millis();
  
  // Reset timer and start driving if state of valve has changed
  if(_bsetOpen!=_bisOpen)
  {
    _iTimeStart = time;
    _bisDriving = true;
  }

  // Drive the valve
  if(_bisDriving==true)
  {
    // Check if motor has reached end position
    if(time - _iTimeStart < _iTimePeriod)
    {
      if(_bsetOpen == true)
      {
        // drive valve motor to open direction
        digitalWrite(_iPinOpen, LOW);
        digitalWrite(_iPinClose, HIGH);
      } 
      else
      {
        // drive valve motor to close direction
        digitalWrite(_iPinOpen, HIGH);
        digitalWrite(_iPinClose, LOW);
      }
      
      // Set state of valve
      _bisOpen = _bsetOpen;
    }
    else  // stop motor
    {
      // stop valve motor
      digitalWrite(_iPinOpen, LOW);
      digitalWrite(_iPinClose, LOW);
      
      // Set state of valve to not driving
      _bisOpen = _bsetOpen;
      _bisDriving = false;
    }
  }
}
