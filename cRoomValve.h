#ifndef cRoomValve_h
#define cRoomValve_h

#include "Arduino.h"


class cRoomValve
{
	public:
	 /// Creates a valve object without setting the pins and closed state (false). This is necessary for the rooms.
	cRoomValve(int pinOpen_ = 0, int pinClose_ = 0)
	{
		
		pinOpen = pinOpen_;
		//pinClose=0;
		pinMode(pinOpen, OUTPUT);
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
		digitalWrite(pinOpen, !bState); // Valve gets opened on low/false
// 		digitalWrite(pinClose, !!bState);
	}
	
	private:
	int pinOpen;
	//int pinClose;
};

#endif
