#ifndef cValve_h
#define cValve_h

#include "Arduino.h"
#define DefaultDriveTime 2000

class cValve
{
	public:
	/// Creates a valve object with pin setting and period, the initial state is close.
	/** Creates a valve object with pin setting. The Valve is initialized with a closed state. */
	cValve(int pinOpen_, int pinClose_, unsigned long DriveTime_ = DefaultDriveTime, boolean bisOpen_ = false)
	{
		pinOpen=pinOpen_;
		pinMode(pinOpen, OUTPUT);
		
		pinClose=pinClose_;
		pinMode(pinClose, OUTPUT);
		
		LastTime = millis();
		DriveTime = DriveTime_;
		
		// Ensure that initial state is reached
		// by executing transition to desired initial state
		bisOpen = !bisOpen_;
		set(bisOpen_);
	}

	/// Reads the is state of the valve.
	/** \return True = open, False = closed. */
	boolean get(void) {return bisOpen;}
	
	/// Sets the state of the Valve and executes the valve (opens or closes it).
	/** If the set state of the valve is true, it shall be opened, if it is false, it is going to be closed*/
	void set(boolean bsetOpen)
	{
		// Reset timer to start driving if state of valve has changed
		if(bsetOpen!=bisOpen)
			LastTime = millis();
		
		unsigned long ElapsedTime = millis() - LastTime;
		
		bisOpen = bsetOpen;
		// Drive the valve for DriveTime, then stop motor
		if(ElapsedTime < DriveTime) {
			// drive valve motor to open direction if bsetOpen is true/high
			// otherwise drive to close
			digitalWrite(pinOpen, !bsetOpen);
			digitalWrite(pinClose, bsetOpen);
		}
		else {
			// stop valve motor
			digitalWrite(pinOpen, LOW);
			digitalWrite(pinClose, LOW);
		}
	}
	
	private:
	int pinOpen;
	int pinClose;
	boolean bisOpen;
	unsigned long LastTime;
	unsigned long DriveTime;
};

#endif
