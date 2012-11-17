#include "cValve.h"

/// Creates a valve object without setting the pins and closed state (false). This is necessary for the rooms.
cValve::cValve(void)
{
  _iPinOpen=0;
  _iPinClose=0;
  _iPinSense=0;
  
  _iTimePeriod = 1800;
  
  _bsetOpen = false;
  _bisOpen = false;
  _bisDriving = false;
}

/// Creates a valve object with pin setting, the initial state is close.
/** Creates a valve object with pin setting. The Valve is initialized with a closed state. */
cValve::cValve(int iPinOpen, int iPinClose, int iPinSense)
{
  cValve::setPinOpen(iPinOpen);
  cValve::setPinClose(iPinClose);
  cValve::setPinSense(iPinSense);
  
  _iTimePeriod = 1800;
  
  _bsetOpen = false;
  _bisOpen = false;
  _bisDriving = true;
}

/// Creates a valve object with pin setting and an initial state.
cValve::cValve(int iPinOpen, int iPinClose, int iPinSense, boolean bsetOpen)
{
  cValve::setPinOpen(iPinOpen);
  cValve::setPinClose(iPinClose);
  cValve::setPinSense(iPinSense);
  
  _iTimePeriod = 1800;
  
  _bsetOpen = bsetOpen;
  _bisOpen = false;
  _bisDriving = true;
}

/// Sets the state of the Valve and executes the run function of the valve.
void cValve::set(boolean bsetOpen)
{
  _bsetOpen = bsetOpen;
  
  cValve::run();
}

/// Sets the _PinOpen. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cValve::setPinOpen(int iPinOpen)
{
  _iPinOpen = iPinOpen;
  pinMode(_iPinOpen, OUTPUT);
}

/// Sets the _PinClose. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cValve::setPinClose(int iPinClose)
{
  _iPinClose = iPinClose;
  pinMode(_iPinClose, OUTPUT);
}

/// Sets the _iPinSense. This is necessary for the rooms, where the constructor cannot initialize the pins.
void cValve::setPinSense(int iPinSense)
{
  _iPinSense = iPinSense;
  pinMode(_iPinSense, INPUT);
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
  float fSense = analogRead(_iPinSense);
  
  // Reset timer and start driving if state of valve has changed
  if(_bsetOpen!=_bisOpen)
  {
    _iTimeStart = time;
    _bisDriving = true;
  }

  // Drive the valve
  if(_bisDriving==true)
  {
    // Check driving conditions
    if((time - _iTimeStart < _iTimePeriod) && (fSense < SenseThreshold))
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
