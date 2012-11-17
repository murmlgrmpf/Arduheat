#ifndef cHeatExchanger_h
#define cHeatExchanger_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "Temperatures.h"

class cHeatExchanger
{
  private:
    float CalculateCr(void);
    float theta_c_soll;
    float C_r;
    float e;
    float dC_r;
    float deltaC_r;
    float CalculateCrLimitCh(float theta);
    float CalculateCrLimitCc(float theta);
    float SetpointTempWarmWater;
    float TempOutside;
    float TempBoilerHead;
    //float LimitCc;
    float theta_h_setpoint;
    
    
    
  public:
    /// Creates the heat exchanger
    cHeatExchanger(void);
    //cHeatExchanger(float *TempWarmWater());
    /// Get the actual mass flow rate in kg/second
    /** \return the actual mass flow rate in kg/second */
    //float get();
    /// Set the power in percent
    /** Drives the mixer with a certain power into a direction (open or close)
    \param Power the desired power in percent */
    //void incCounter(void);
    //float CalculateCr(float T_Bw_soll, float T_Bw_aussen, float T_Kuppel);
    float getMassflow(float T_Bw_soll, float T_Bw_aussen, float T_Kuppel, float Massflow_h, float Massflow_c);
    
};

#endif
