#ifndef cFlowMeter_h
#define cFlowMeter_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "cTrigger.h"

#define LiterPerImpuls 0.009263548
#define TriggerTimePeriod 500

class cFlowMeter
{
	private:
	void run(void);
	/// Trigger to execute flow detection in fixed intervals
	cTrigger Trigger;

	unsigned int _iLastCounter;

	boolean _bFlowing;
	
	public:
	// For debugging make the counter public. move to private once it works
	unsigned int _iCounter;
	/// Creates the flow meter
	cFlowMeter(void);
	/// Get the actual mass flow rate in kg/second
	/** \return the actual mass flow rate in kg/second */
	boolean get();
	/// Set the power in percent
	/** Drives the mixer with a certain power into a direction (open or close)
	\param Power the desired power in percent */
	void incCounter(void);
	
};

#endif
