#ifndef cFlowMeter_h
#define cFlowMeter_h

#include "Arduino.h"
#include "PinDefinitions.h"

#define LiterPerImpuls 0.009263548

class cFlowMeter
{
  private:
    void run(void);
    //void calcPeriod();
    
    unsigned long _lTimePeriod;
    unsigned long _lLastTime;
    float _fMassFlowRate;
    unsigned int _iLastCounter;

    boolean _bFlowing;
    
  public:
  // For debugging make the counter public. move to private once it works
      unsigned int _iCounter;
    /// Creates the flow meter
    cFlowMeter(void);
    /// Get the actual mass flow rate in kg/second
    /** \return the actual mass flow rate in kg/second */
    float get();
    /// Set the power in percent
    /** Drives the mixer with a certain power into a direction (open or close)
    \param Power the desired power in percent */
    void incCounter(void);
    
};

#endif
