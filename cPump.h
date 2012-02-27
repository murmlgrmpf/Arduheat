#ifndef cPump_h
#define cPump_h

#include "Arduino.h"
#include "PinDefinitions.h"

class cPump
{
  private:
    int _PinPump;
    float _Power;
    float _Period;
    long _TimePeriod;
    long _TimeSwitch;
    long _LastTimePeriod;
    float _CPump;
    
    /// Execute the pump. 
    /** Each time, it is checked whether the period is over or whether the switch time is reached. 
        If the TimeSwitch is reached, the pump is switched off. When the TimePeriod is reached, the pump is switched on 
        and the new TimePeriod, TimeSwitch and LastTimePeriod get set. */ 
    void run(void);
    /// Calculate the Period
    /** A more elaborate way to calculate the Period of the "slow" PWM could be implemented in order
    to prevent the pump from being switched on and off for only short times.*/
    void calcPeriod(void);
    
  public:
    /// Creates a pump object without a certain pin.
    cPump(void);
    /// Creates a pump object with a certain pin.
    /** \param PinPump the pin used to drive the Pump*/
    cPump(int PinPump);
    /// Creates a pump object with a certain pin and a certain value
    /** \param PinPump the pin used to drive the Pump
        \param Value the power in percent    */
    cPump(int PinPump, float Value);
    //~Pump();
    /// Get the actual power in percent
    /** \return the actual power in percent */
    float get();
    /// Set the power in percent
    /** Set the power in percent and execute the run function
    \param Value the desired power in percent */
    void set(float Value);
    /// Set the Pin of the Pump
    /** Set the pin of the pump and execute the run function
    \param PinPump the pin of the pump */
    void setPinPump(int PinPump);
};

#endif
