#include "TPIC_Shifter.h"
#include "config.h"

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
	if(runOnce == true) {
	  resetLives();
	  runOnce = false;
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
	
}

void TPIC_Shifter::ledsDance(int repeats, int startPin, int speed) {
	// Quick roll of the pins
	int s = startPin - 1;
	for (int i = 0; i < repeats; i++) {
			byte b = B11111111;
			for (int i = s; i < NUM_CONNECTED; i++){
		    b = 1<<i;
		    writeShift(b);
				delay(speed);
	    }
	  }
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
#if DEBUG
	Serial.print("Current lives = ");
	Serial.println(currentLives);
#endif
	//Let's dance!
	ledsDance(10, 1, 20);
	writeShift(specificPinOn[currentLives]);
	
	// RESET
	if(currentLives == 0){
		resetLives();
	}
}

long int previousMillis = 0;
long int currentMillis; 
int reading = LOW;
bool isReset = true;

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

void TPIC_Shifter::resetLives() {
	// Reset lives
	ledsFlash(5, 20);
	currentLives = MAX_LIVES;
	// Turn the MAX_LIVES pins on
	writeShift(specificPinOn[currentLives]);
}

