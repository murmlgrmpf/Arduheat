#include "cFlowMeter.h"


cFlowMeter::cFlowMeter(void)
{
  //Set the time to read the MassFlowMeter
  _lLastTime = millis();
  _bFlowing = false;
  _fMassFlowRate = 0.0;
}

float cFlowMeter::get()
{
  /* If the warm water was turned off, and now started then
       - set the state _bFlowing to true
       - calculate an initial _fmassflowRate
     Finally set new _lLasttime, so that either the averaging cycle begins or the 
     value is prepared for further listening to iCounter until it is used to calculate
     the initial _fMassflowRate
  */
  if(!_bFlowing)
  {
    if(_iCounter-_iLastCounter > 0)
    {
      _bFlowing = true;
      // Initial mass flow rate in [kg/s] is calculated
      _fMassFlowRate = (DensityWater*LiterPerImpuls*(_iCounter-_iLastCounter)/(float(millis() - _lLastTime))*1000);
      
    }
    _lLastTime = millis();
  }
  /* If the warm water was already turned on, check whether the time step is over, then
        - If the water is still flowing: calculate new filtered _fMassFlowRate
        - If the water has stopped flowing: shut down _fMassFlowRate and set the state _bFlowing to turned off.
  
  */
  else
  {
    if(millis()>_lLastTime+_lTimePeriod)
    {
      if (_iCounter-_iLastCounter > 0)
      {
        // Lowpass filtered mass flow rate in [kg/s] is calculated
        _fMassFlowRate = AlphaFilter*_fMassFlowRate + (1-AlphaFilter)*(DensityWater*LiterPerImpuls*(_iCounter-_iLastCounter)/(float(millis() - _lLastTime))*1000);
      }
      else
      {
        _fMassFlowRate = 0.0;
        _bFlowing = false;
      }
      _lLastTime = millis();
    }
  }
  
  return _fMassFlowRate;
}

void cFlowMeter::incCounter(void)
{
  _iCounter++;
  
  // Maybe write a hook to trigger Warmwater control to get faster response times
  // if (!_bFlowing)
  // {
  //   -> Warm water control
  // }
}
