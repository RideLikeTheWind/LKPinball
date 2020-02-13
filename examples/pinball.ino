#include "TPIC_Shifter.h"

const int dataPin = 9;
const int clockPin = 5;
const int latchPin = 6;
const int pinStatus = 7;
const int ballReturnPin = 2;

TPIC_Shifter myShifter = TPIC_Shifter(dataPin, clockPin, latchPin, pinStatus, ballReturnPin);

void setup() {
  myShifter.TPICBegin();
}

void loop() {

  myShifter.firstRun();  
  
  if(myShifter.ballReturn() == true){
    myShifter.gameLives(1);
  }

}