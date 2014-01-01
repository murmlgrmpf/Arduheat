// 
// 
// 

#include "cSpHeating.h"

cSpHeating::cSpHeating(void):
TempOutside(SystemMultiplexer,MultiplexTempOutside,OffsetTempOutside)
{
	// Set default parameters for SpHeating curve
	_dsteil = 0.6;
	_dkh = 4;
	_dverst = 2;
	_dminvl = 30;
	_dmaxvl = 45;
}


double cSpHeating::get(double dMaxSp, double dMaxDiff )
{
	double t1;
	double t2;
	double swhk;
	double swhkLimit;
	
	t1 = (TempOutside.get()/(320-4*TempOutside.get()));
	t2 = pow(dMaxSp,t1);
	swhk = 0.55*_dsteil*t2*(-TempOutside.get()+20)*2+dMaxSp+_dkh+dMaxDiff*_dverst;
	swhkLimit = min(max(swhk,_dminvl),_dmaxvl);
	return swhkLimit;
}

