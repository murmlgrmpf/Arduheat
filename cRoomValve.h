#ifndef cRoomValve_h
#define cRoomValve_h

#include "Arduino.h"


class cRoomValve
{
	public:
	 /// Creates a valve object without setting the pins and closed state (false). This is necessary for the rooms.
	cRoomValve(const int* pinOpen_ , int pinClose_ = 0)
	{
		
		pinOpen = pinOpen_;
		//pinClose=0;
		pinMode(pgm_read_word(pinOpen), OUTPUT);
		set(false);
	}
	
	/// Sets the _PinClose. This is necessary for the rooms, where the constructor cannot initialize the pins.
	//void setPinClose(int pinClose_);
	// {
	// 	pinClose = pinClose_;
	// 	pinMode(pinClose, OUTPUT);
	// }
	
	void set(boolean bState)
	{
		digitalWrite(pgm_read_word(pinOpen), !bState); // Valve gets opened on low/false
// 		digitalWrite(pinClose, !!bState);
	}
	
	private:
	const int* pinOpen;
	//int pinClose;
};

#endif
