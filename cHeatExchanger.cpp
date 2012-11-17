#include "cHeatExchanger.h"


cHeatExchanger::cHeatExchanger(void)
{
  C_r = 0.9;
  e = 0;
  dC_r= 0;
  deltaC_r = 0;
  theta_c_soll = 0;
}

float cHeatExchanger::CalculateCrLimitCc(float theta)
{
  //theta_c_soll = (T_Bw_soll-T_Bw_aussen)/(T_Kuppel-T_Bw_aussen);
  int N=0; // Bullshit
  // Newton Verfahren, Fixpunktiteration
  for(int i=0;i<=4;i++)
  {
    e= theta - (1-exp(-N*(1-C_r)))/(1-C_r*exp(-N*(1-C_r)));

    dC_r = (exp(N*C_r)*(exp(N)*(-C_r*N+N-1)+exp(N*C_r)))/(pow(exp(N)-C_r*exp(C_r*N),2));

    deltaC_r= - e/(dC_r);

    C_r = C_r + deltaC_r;
  }
  return(C_r);
}

float cHeatExchanger::CalculateCrLimitCh(float theta)
{
  //theta_c_soll = (T_Bw_soll-T_Bw_aussen)/(T_Kuppel-T_Bw_aussen);
  int N=0; // Bullshit
  // Newton Verfahren, Fixpunktiteration
  for(int i=0;i<=4;i++)
  {
    e= theta/C_r - (1-exp(-N*(1-C_r)))/(1-C_r*exp(-N*(1-C_r)));

    //dC_r = ???;

    deltaC_r= - e/(dC_r);

    C_r = C_r + deltaC_r;
  }
  return(C_r);
}

float cHeatExchanger::getMassflow(float T_Bw_soll, float T_Bw_aussen, float T_Kuppel, float Massflow_h, float Massflow_c)
{
  float theta_c = 0;
  // Current Values
  //theta_c = (TempWarmWater - TempOutside)/(TempBoilerHead - TempOutside);
  //theta_h = (TempBoilerHead-Temp_Rücklauf?)/(TempBoilerHead - TempOutside);
  float C_h = Massflow_h*CWater;
  float C_c = Massflow_c*CWater;
  // Approximated theta_h
  float theta_h = C_c/C_h*theta_c;
  
  // Setpoint Values
  float theta_c_setpoint = (SetpointTempWarmWater - TempOutside)/(TempBoilerHead - TempOutside);
  
  //C_r = cHeatExchanger::CalculateCrLimitCc(theta_c_setpoint);
  
  //theta_h_setpoint = 1/C_r*theta_c_setpoint;
  
  //if(C_r>=1)
  //{
  //  C_r = cHeatExchanger::CalculateCr(theta_h_setpoint);
  //}
  
  
  
}
