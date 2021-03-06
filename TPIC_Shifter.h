#ifndef TPIC_SHIFTER_H
#define TPIC_SHIFTER_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define LOSE_LIFE_MODE 0
#define WIN_POINT_MODE 1

class TPIC_Shifter
{
  public:
	  // Constructor
	  TPIC_Shifter(int dataPin,int clockPin, int latchPin, int clearPin, int ballReturnButton=0, int max_lives=3, int num_connected=8);

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

	  void ledsDance(int repeats = 10, int speed = 10);
	  /* BLOCKING
	  	Lights roll around quickly
	  	repeats = number of repeats you want (about 1 per second) | default = 10
	  	startPin = pin you want it to start from | default = 0
	  	speed = how fast you want them to dance (higher is slower) | default = 10, max = 5
	  */

		// Specific Game Functions

	  void gameMode(int gm=0);
	  /*
	  	Lights turn off one at a time by shifting on or off
	  	startPin = pin to start the shift from
	  	gm = 	0: lose lives.
	  					1: gain points.
	  */

		bool endOfGame();
		/*
			Check to see if the game has ended
		*/

		bool ballReturn();
		/*
			Manages the returned ball from the pinball machine
			returns true if button is pressed
		*/

		void updateUI();
		/*
			Updates the LEDs based on Game Mode
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
		int max_lives;
		int MAX_LIVES;
		int NUM_CONNECTED;
		int game_mode;

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
