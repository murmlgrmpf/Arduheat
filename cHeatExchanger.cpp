float BerechneC_r(void)
{
  float theta_c_soll = (T_Bw_soll-T_Bw_aussen)/(T_Kuppel-T_Bw_aussen);
  // Startwert für C_r
  float C_r = 0.9;
  float e = 0;
  float dC_r= 0;
  float deltaC_r = 0;
  
  //Serial.println("theta_c_soll");
  //Serial.println(theta_c_soll, DEC);
  
  // Newton Verfahren, Fixpunktiteration
  for(int i=0;i<=4;i++)
  {
    e= theta_c_soll - (1-exp(-N*(1-C_r)))/(1-C_r*exp(-N*(1-C_r)));

    dC_r = (exp(N*C_r)*(exp(N)*(-C_r*N+N-1)+exp(N*C_r)))/(pow(exp(N)-C_r*exp(C_r*N),2));

    deltaC_r= - e/(dC_r);

    C_r = C_r + deltaC_r;
  }
  return(C_r);
}
