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
  if(!_bFlowing)
  {
    if(_iCounter-_iLastCounter > 0)
    {
      _bFlowing = true;
      // Initial mass flow rate in [kg/s] is calculated
      _fMassFlowRate = (DensityWater*LiterPerImpuls*(_iCounter-_iLastCounter)/(float(millis() - _lLastTime))*1000);
      _lLastTime = millis();
    }
  }
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
}
