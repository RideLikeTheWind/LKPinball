#include "config_h.h"
#include "constants.h"
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

int currentLives = MAX_LIVES; // because the chip is zero-index

TPIC_Shifter::TPIC_Shifter(int dataPin, int clockPin, int latchPin, int clearPin, int ballReturnButton)
{
  this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->latchPin = latchPin;
  this->clearPin = clearPin;
	this->ballReturnButton = ballReturnButton;
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
	pinMode(ballReturnButton, INPUT);
	
#if DEBUG
	Serial.begin(9600);
#endif
}

void TPIC_Shifter::firstRun() {
	if(this->runOnce == true) {
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
		for (int a = 0; a <= NUM_CONNECTED; a++){
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

void TPIC_Shifter::gameLives(int type){
	// Add or remove lives
	// depending on parameters (see h file)
	// Set MAX_LIVES in the config file
	
	if(type == 1){
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

bool TPIC_Shifter::ballReturn() {
	
	bool result = false;
	reading = digitalRead(ballReturnButton);

	while(reading == HIGH && isReset == true) {
	
#if DEBUG
	Serial.println(previousMillis);
	Serial.println(currentMillis);
#endif

		if((millis() - previousMillis) > 20){
			result = true;
			break;
		}
		reading = digitalRead(ballReturnButton);
	}
	
	if(reading != HIGH) {
		previousMillis = millis();
		isReset = true;
	}else{
		isReset = false;
	}
	
	return result;
}

void TPIC_Shifter::resetLives() {
	// Reset lives
	currentLives = MAX_LIVES;
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
