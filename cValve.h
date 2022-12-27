#ifndef cValve_h
#define cValve_h

#include "Arduino.h"

class cValve
{
	public:
	// Creates a valve object without setting the pins
	cValve(int pinOpen_, boolean bisOpen_ = false)
	{
		pinOpen = pinOpen_;
		pinMode(pinOpen, OUTPUT);
		bisOpen = !bisOpen_;
		set(bisOpen_);
	}

	boolean get(void) {return bisOpen;}
		
	void set(boolean bsetOpen)
	{
		bisOpen = bsetOpen;
		digitalWrite(pinOpen, bsetOpen);
	}

	private:
	int pinOpen;
	boolean bisOpen;
};

#endif
