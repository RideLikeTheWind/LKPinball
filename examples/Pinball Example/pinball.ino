#include <TPIC_Shifter.h>


// Add constants for the pins connected to the Arduino
// from the pinball circuit
const int dataPin = 9;
const int clockPin = 5;
const int latchPin = 6;
const int pinStatus = 7;
const int ballReturnPin = 2;

TPIC_Shifter pinballMachine = TPIC_Shifter(dataPin, clockPin, latchPin, pinStatus, ballReturnPin);

void setup() {
  // put your setup code here, to run once:
  pinballMachine.TPICBegin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pinballMachine.firstRun();

  // You may want to do something here to 
  if(pinballMachine.ballReturn() == true){
    pinballMachine.gameLives(1);
    pinballMachine.ledsFlash(10,20);
  }

  if(pinballMachine.currentLives == 0){
    delay(100);
    pinballMachine.ledsDance(5,20);
    pinballMachine.resetLives();
  }
  

}