#include <TPIC_Shifter.h>

// Add constants for the pins connected to the Arduino
// from the pinball circuit
const int dataPin = 9;
const int clockPin = 5;
const int latchPin = 6;
const int pinStatus = 7;
const int ballReturnPin = 2;
int max_lives = 3;
int num_connected = 8;

TPIC_Shifter pinballMachine = TPIC_Shifter(dataPin, clockPin, latchPin, pinStatus, ballReturnPin, max_lives, num_connected);

void setup() {
  // put your setup code here, to run once:
  pinballMachine.TPICBegin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pinballMachine.firstRun();

  // You may want to do something here to 
  if(pinballMachine.ballReturn() == true){
    pinballMachine.gameLives(0);
    pinballMachine.ledsFlash(10,20);
  }

  if(pinballMachine.currentLives == 8){
    delay(1000);
    pinballMachine.ledsDance(10,30);
    pinballMachine.resetLives();
  }
  

}