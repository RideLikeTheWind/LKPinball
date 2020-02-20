#ifndef TPIC_SHIFTER_H
#define TPIC_SHIFTER_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class TPIC_Shifter
{
  public:
	  // Constructor
	  TPIC_Shifter(int dataPin,int clockPin, int latchPin, int clearPin, int ballReturnButton=0);

	  // Setup pins
	  void TPICBegin(); 

		void firstRun();
		/*
			This runs on first loop through
			Turns all lights on
			To do: Checks the switch is wired correctly
		*/
		
		//UI Functions
	  
	  void specificPin(int pins);
	  /*
	  	Turn on or off a specific pin
	  	pin = integer of position of pin to turn on
	  */
	  
	  void ledsFlash(int repeats, int speed);
	  /* BLOCKING
	  	Flashes all lights on and off
	  	repeats = number of flashes
	  	speed = delay in ms for lights to remain on (1000 = 1 second)
	  */
	
	  void ledsDance(int repeats = 10, int startPin = 0, int speed = 10);
	  /* BLOCKING
	  	Lights roll around quickly
	  	repeats = number of repeats you want (about 1 per second) | default = 10
	  	startPin = pin you want it to start from | default = 0
	  	speed = how fast you want them to dance (higher is slower) | default = 10, max = 5
	  */
		
		// Specific Game Functions
	
	  void gameLives(int type = 1);
	  /* 
	  	Lights turn off one at a time by shifting on or off 
	  	startPin = pin to start the shift from
	  	type = 	1: remove lives. 
	  					2: add lives.
	  */
		
		bool ballReturn();
		/*
			Manages the returned ball from the pinball machine
			returns true if button is pressed
		*/
		
		// Utility Functions
		
		// main write function
	  void writeShift(int bits);
	  
		// Turn the pins off or on
	  void pinDrains(int visibility); //HIGH (ON) - LOW (OFF)
	  
		// Clear all pins of output
	  void pinClear();

		// Reset lives of the player to MAX_LIVES
		void resetLives();
		
		// Displays current lives in LEDs
		void displayCurrentLives();
		
		int currentLives;

		void resetLives();

	private:
		//General
		int dataPin;
		int clockPin;
		int latchPin;
		int clearPin;
		int previousPosition;
		int ballReturnButton;
		bool runOnce = true;
		
		long int previousMillis = 0;
		long int currentMillis; 
		int reading = LOW;
		bool isReset = true;
		
		//Storage
		byte IC[];
		byte ledOff[];
		byte ledOn[];
	
		//Functions
		void pulsePin();
};
#endif