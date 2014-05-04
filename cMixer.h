#ifndef cMixer_h
#define cMixer_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cLFPWM.h"

class cMixer
{
  private:
	cLFPWM PWM;
    
  public:
    /// Creates the mixer
    cMixer(void);
	
	/// Set the power in percent
    /** Drives the mixer with a certain power (abs) into a direction (sign: open or close)
    \param Power the desired power in percent */
	void run(double Power);
    
};

#endif
