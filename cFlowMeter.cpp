#include "cFlowMeter.h"


cFlowMeter::cFlowMeter(void):
Trigger(TriggerTimePeriod)
{
	//Set the time to read the MassFlowMeter
	_bFlowing = false;
	_iLastCounter = 0;
	_iCounter = 0;
}

boolean cFlowMeter::get()
{
	if(Trigger.get())
	{
		// If counter incremented, there is flow
		_bFlowing = (_iCounter-_iLastCounter > 1);
		
		_iLastCounter = _iCounter;
	}
	
	return _bFlowing;
}

void cFlowMeter::incCounter(void)
{
	_iCounter= _iCounter +1;
}
