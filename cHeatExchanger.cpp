#include "cHeatExchanger.h"


cHeatExchanger::cHeatExchanger(void)
{
}

float cHeatExchanger::CrLimitCc(float theta_c_set)
{
  // Initial Values
  float Cr=0.5;
  float J =0;
  float gradJ = 0;

  for(int i=0;i<=maxIter;i++)
  {
    // Quadratic cost function
    J = pow(  ( theta_c_set - (1-exp(-N*(1-Cr)))/(1- Cr*exp(-N*(1-Cr))) )  ,2);
    // Gradient of cost function
    gradJ = 2*(theta_c_set - (1-exp(-N*(1-Cr)))/( 1- Cr*exp( -N*(1-Cr) ) )) *(exp(N*Cr)*(exp(N)*(-Cr*N+N-1)+exp(N*Cr)))/(pow((exp(N)-Cr*exp(Cr*N)), 2));
    // Gradient descent using relaxation
    Cr = Cr - (1-alpha)*J/gradJ;
  }
  
  // Check for convergence
  if (J < 0.001)
  {
    return(Cr);
  }
  else
  {
    return(100);
  }
}

float cHeatExchanger::CrLimitCh(float theta_c_set)
{
  // Initial Values
  float Cr=0.5;
  float J =0;
  float gradJ = 0;
  
  for(int i=0;i<=maxIter;i++)
  {
    // Quadratic cost function
    J= pow(  ( 1/Cr*theta_c_set - (1-exp(-N*(1-Cr)))/(1- Cr*exp(-N*(1-Cr))) ) , 2);
    // Gradient of cost function
    gradJ = 2*( 1/Cr*theta_c_set - (1-exp(-N*(1-Cr)))/(1- Cr*exp(-N*(1-Cr))) ) *( -1/(pow(Cr,2))*theta_c_set + (exp(N*Cr)*(exp(N)*(-Cr*N+N-1)+exp(N*Cr)))/(pow( (exp(N)-Cr*exp(Cr*N)), 2))  );
    // Gradient descent using relaxation
    Cr = Cr - (1-alpha)*J/gradJ;
  }
  
  // Check for convergence
  if (J < 0.001)
  {
    return(Cr);
  }
  else
  {
    return(100);
  }
}

float cHeatExchanger::calcMassflow(float T_Bw_soll, float T_Bw_aussen, float T_Kuppel, float Massflow_c)
{
  float Th  = T_Kuppel;
  float Tc  = T_Bw_aussen;
  float setTcp = T_Bw_soll;
  
  float setTheta_c = (setTcp - Tc)/(Th - Tc);
  float C_c = Massflow_c*CWater;

  if(setTheta_c <= setTheta_c_Threshold*0.999)
  {
    float Cr_h = cHeatExchanger::CrLimitCh(setTheta_c);
    float Massflow_h_h = C_c/CWater*Cr_h;
    if (Cr_h<1.0)
    {
      return(Massflow_h_h);
    }
  }
  else if(setTheta_c >= (setTheta_c_Threshold*1.001))
  {
    float Cr_c = cHeatExchanger::CrLimitCc(setTheta_c);
    float Massflow_h_c = C_c/CWater/Cr_c;
    if (Cr_c<1.0)
    {
      return(Massflow_h_c);
    }
    
  }
  else
  {
    float Cr = 1;
    float Massflow_h = C_c/CWater;
    return(Massflow_h);
  }
  
  // If the Cr_x values were out of range (larger than 1)
  return(0);
}
