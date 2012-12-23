#ifndef cHeatExchanger_h
#define cHeatExchanger_h

#include "Arduino.h"
#include "PinDefinitions.h"
#include "Temperatures.h"

// solving Cr
#define maxIter 15  // max number of iterations
#define alpha 0.1   // relaxation coefficient

// Properties of heat exchanger
// Identified by measurements & regression
#define N 4.3       // Übertragungsfähigkeit
#define setTheta_c_Threshold 0.81135 // threshold between limiting Cc and Ch

class cHeatExchanger
{
  private:
    float CrLimitCc(float theta);
    float CrLimitCh(float theta);
    
    
    
  public:
    /// Creates the heat exchanger
    cHeatExchanger(void);
    //cHeatExchanger(float *TempWarmWater());
    /// Get the actual mass flow rate in kg/second
    /** \return the actual mass flow rate in kg/second */
    float calcMassflow(float T_Bw_soll, float T_Bw_aussen, float T_Kuppel, float Massflow_c);
    
};

#endif
