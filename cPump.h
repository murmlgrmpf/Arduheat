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
    float _fMaxMassFlowRate;
    
    /// Execute the pump. 
    /** Each time, it is checked whether the period is over or whether the switch time is reached. 
        If the TimeSwitch is reached, the pump is switched off. When the TimePeriod is reached, the pump is switched on 
        and the new TimePeriod, TimeSwitch and LastTimePeriod get set. */ 
    void run(void);
    /// Calculate the Period
    /** A more elaborate way to calculate the Period of the "slow" PWM could be implemented in order
    to prevent the pump from being switched on and off for only short times in some race conditions.*/
    void calcPeriod(void);
    
  public:
    /// Creates a pump object without a certain pin.
    cPump(void);
    /// Creates a pump object with a certain pin.
    /** \param PinPump the pin used to drive the Pump*/
    /** \param fMaxMassFlowRate the maximum mass flow rate of the pump*/
    cPump(int PinPump, float fMaxMassFlowRate);
    /// Creates a pump object with a certain pin and a certain value
    /** \param PinPump the pin used to drive the Pump
        \param Value the power in percent    */
    cPump(int PinPump, float fMaxMassFlowRate, float fMassFlowRate);
    /// Get the actual mass flow rate in kg/s
    /** \return the actual mass flow rate in kg/s */
    float getMassFlowRate(void);
    /// Set the mass flow rate in kg/s
    /** Set the mass flow rate in kg/s and execute the run function
    \param Value the desired power in percent */
    void setMassFlowRate(float fMassFlowRate);
    /// Get the maximum mass flow rate in kg/s
    /** \return the maximum mass flow rate in kg/s for limiting the controler/regulator*/
    float getMaxMassFlowRate(void);
    /// Set the maximum mass flow rate in kg/s
    /** \return the maximum mass flow rate in kg/s for limiting the controler/regulator*/
    void setMaxMassFlowRate(float fMaxMassFlowRate);
    /// Set the Pin of the Pump
    /** Set the pin of the pump and execute the run function
    \param PinPump the pin of the pump */
    void setPinPump(int PinPump);
};

#endif
