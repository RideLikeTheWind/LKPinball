/*##########################################*/
/****** ONLY CHANGE IF YOU KNOW WHAT YOU ARE DOING ******/
/****** Basically, don't edit this file... ***********/

// Define as true if you're planning to shift using SPI
#define USE_SPI false

//Define if you want to debug (writes to Serial Monitor)
#ifndef DEBUG
#define DEBUG true
#endif

#if USE_SPI
#include <SPI.h>
#endif

#define PULSE_DELAY 10
/*##########################################*/

#include "TPIC_Shifter.h"

int specificPinOn[] = {
  // Int converts to binary on shift-in
	0, //All off
	8, //Position 1 on 00001000
	12, //Position 2 on 00001100
	14, //Position 3 on 00001110
	15, //Position 4 on 00001111
	31, //Position 5 on 00011111
	63, //Position 6 on 001111111
	127, //Position 7 on 011111111
	255 //Position 8 on 111111111
};

int currentLives = 0;
int game_mode = 0;

TPIC_Shifter::TPIC_Shifter(int dataPin, int clockPin, int latchPin, int clearPin, int ballReturnButton, int max_lives, int num_connected)
{
  this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->latchPin = latchPin;
  this->clearPin = clearPin;
	this->ballReturnButton = ballReturnButton;
	this->MAX_LIVES = max_lives;
	currentLives = 0; // because the chip is zero-index
	this->NUM_CONNECTED = num_connected;
}

void TPIC_Shifter::TPICBegin()
{
#if USE_SPI
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.SetBitOrder(LSBFIRST);
#else
	//only defined in non-SPI mode
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
#endif
  //always set these to output
	pinMode(latchPin, OUTPUT);
	pinMode(clearPin, OUTPUT);
	pinMode(ballReturnButton, INPUT_PULLUP);

	Serial.begin(9600);
}

void TPIC_Shifter::firstRun() {
	if(this->runOnce == true) {
		if(this->MAX_LIVES == 0){
			Serial.println("Looks like you have set Max Lives to 0. This will cause an error.");
			Serial.println("Setting Max Lives to num_connected for now..");
			this->MAX_LIVES = this->NUM_CONNECTED;
		}

	  resetLives();
		displayCurrentLives();

	  this->runOnce = false;
	}
}

void TPIC_Shifter::writeShift(int b) {

#if USE_SPI
	SPI.transfer(b);
#else
	shiftOut(dataPin, clockPin, LSBFIRST, b);
#endif
	pulsePin();

#if DEBUG
	Serial.print("ByteOut = ");
	Serial.println(b);
#endif

}

// UI Functions

void TPIC_Shifter::ledsFlash(int repeats, int speed) {

	for (int i = 0; i < repeats; i++){
		writeShift(0);
		delay(speed);
		writeShift(255);
		delay(speed);
	}
	displayCurrentLives();

}

void TPIC_Shifter::ledsDance(int repeats, int speed) {
	// Quick roll of the pins
	for (int i = 0; i < repeats; i++) {
		writeShift(0);
		for (int a = 0; a <= this->NUM_CONNECTED; a++){
	    //writeShift(specificPinOn[a]);
			int randomNum = random(-1, 10);
			writeShift(specificPinOn[randomNum]);
			delay(speed);
    }

  }
	displayCurrentLives();
}

void TPIC_Shifter::specificPin(int pins) {

	int b = specificPinOn[pins];
	writeShift(b);

}

// Game Functions

void TPIC_Shifter::gameMode(int gm){
	this->game_mode = gm;
}

bool TPIC_Shifter::ballReturn() {

	bool result = false;
	reading = digitalRead(ballReturnButton);

	while(reading == LOW && isReset == true) {

#if DEBUG
	Serial.print("Button Pressed: PM= ");
	Serial.print(previousMillis);
	Serial.print(" & CM= ");
	Serial.println(currentMillis);
#endif

		if((millis() - previousMillis) > 20){
			result = true;
			break;
		}
		reading = digitalRead(ballReturnButton);
	}

	if(reading != LOW) {

#if DEBUG
	Serial.println("Button unpressed");
#endif

		previousMillis = millis();
		isReset = true;
	}else{
		isReset = false;
	}

	return result;
}

bool TPIC_Shifter::endOfGame() {

	if (this->game_mode == LOSE_LIFE_MODE) {
		if (currentLives == 0){
			Serial.println("All lives lost! Game over...");
			return true;
		}else{
			return false;
		}
	}else if (this->game_mode == WIN_POINT_MODE){
		if (currentLives == this->MAX_LIVES) {
			Serial.println("Max points! Game over...");
			return true;
		}else{
			return false;
		}
	}
	return false;
}

void TPIC_Shifter::updateUI() {
	if(this->game_mode == 0){
		currentLives--;
	}else{
		currentLives++;
	}
	displayCurrentLives();
	writeShift(specificPinOn[currentLives]);

#if DEBUG
	Serial.print("Current lives = ");
	Serial.println(currentLives);
#endif
}

void TPIC_Shifter::resetLives() {
	// Reset lives
	if(game_mode == LOSE_LIFE_MODE){
		currentLives = this->MAX_LIVES;
	}else if(game_mode == WIN_POINT_MODE){
		currentLives = 0;
	}
	displayCurrentLives();
}

void TPIC_Shifter::displayCurrentLives(){
	writeShift(specificPinOn[currentLives]);
}

// Utility
void TPIC_Shifter::pinClear() {
	// Turn all the lights off
	writeShift(0);
}

void TPIC_Shifter::pinDrains(int visibility) {
	//Turn the pins off or on
	if (visibility == HIGH){
	    digitalWrite(clearPin, LOW);
	  }else{
	    digitalWrite(clearPin, HIGH);
	  }
}

// Private

void TPIC_Shifter::pulsePin() {
	//pulse to transfer bits to the display register
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(PULSE_DELAY);
    digitalWrite(latchPin, LOW);
}
