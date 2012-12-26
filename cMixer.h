#ifndef cMixer_h
#define cMixer_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTemp.h"
#include "cValve.h"

class cMixer
{
  private:
    void run(void);
    void calcPeriod();
    
    long _TimePeriod;
    long _TimeSwitch;
    long _LastTimePeriod;
    float _Power;
    long _Period;
    
  public:
    /// Creates the mixer
    cMixer(void);
    /// Creates the mixer and sets an initial Power
    cMixer(float Power);
    /// Get the actual power in percent
    /** \return the actual power in percent */
    float get();
    /// Set the power in percent
    /** Drives the mixer with a certain power into a direction (open or close)
    \param Power the desired power in percent */
    void set(float Power);
    
};

#endif
