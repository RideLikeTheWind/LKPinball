
## TPIC6B595 Library

### Description
This library is designed for the Texas Instruments TPIC6C595 Serial to Parallel Shift Register. It was adapted from a similar library by Derek Chafin to suit a specific need.
It utilises a circuit designed to control a Pinball Machine at Loreto Kirribilli. The library will not work correctly without that chip. If you require more information please contact the author.

### Authorship
Author: J Mascorella  
Github: @iridelikethewind
Year: 2020  

### Configuration
You will need to supply the following parameters at the top of the main .ino file in Arduino.

dataPin: The pin "Ser. In" on the white board is connected to on Arduino

clockPin: *NOTE* this should be the number the _LATCH_ pin is connected to on the arduino. It was a mistake in wiring the original board.

latchPin: *NOTE* this should be the number the _CLOCK_ pin is connected to on the arduino. It was a mistake in wiring the original board.

pinStatus: Pin G(bar) is connected to on Arduino

* ballReturnPin: Pin the switch is connected to on Arduino
* max_lives: If game mode is 0 (lose life mode) then you will set this to the max_lives for the game. Otherwise, if game mode is 1 (gain points mode) then set this to 0 so the lights count up.
* num_connected: the number of lights you have connected to the Arduino (up to 8).

```c
const int dataPin = 9;
const int clockPin = 5; // See note above
const int latchPin = 6; // See note above
const int pinStatus = 7;
const int ballReturnPin = 2;
int max_lives = 3;
int num_connected = 8;
```

### Functions(params)

#### Initialisation
Constructor: Call this before the void setup() and loop() functions
```c
TPIC_Shifter(int dataPin, int clockPin, int latchPin, int clearPin, int ballReturnButton, int max_lives, int num_connected);
```

Setup function
Must run in the void setup() loop in Arduino
```c
void TPICBegin();
```

Game mode options
Change the game mode so you can either lose points or gain them.
**Important** - Make sure you have set max_lives to something greater than 0, and not more than the number of lights connected (num_connected)
The options are:
* LOSE_LIFE_MODE
* WIN_POINT_MODE
```c
void gameMode(int mode);
```

#### UI functions
**BLOCKING**
Flashes all lights on and off
repeats = number of flashes
speed = delay in ms for lights to remain on (1000 = 1 second)
```c
void ledsFlash(int repeats, int speed);
```

**BLOCKING**
Lights roll around quickly
repeats = number of repeats you want (about 1 per second)
speed = how quickly the lights should change. Slower is a bigger number (> 20 starts getting slow)
```c
void ledsDance(int repeats, int speed);
```

Turn on or off a specific pin
pin = decimal number that converts to an 8 bit byte (00000000 - 11111111 as 0 - 255)
```c
void specificPin(int pins);
```

#### Game Functions

Update the UI to display the current lives remaining or count up the points available.
```c
void updateUI()
```

Check if the game has ended. Returns true if all lives lost, true if all points gained.
```c
void endOfGame()
```

Manages the returned ball from the pinball machine returns true if button is pressed
Use to fire other utility or UI functions
Returns: bool true || false
```c
bool ballReturn();
```

Reset lives in the game
```c
void TPIC_Shifter::resetLives();
```

#### Utility functions

Main write function
Can be used to override LED displays
Argument is a decimal number that converts to an 8 bit byte (00000000 - 11111111 as 0 - 255)
_Only use if another function is unsuitable._
```c
void writeShift(int bits);
```

Turn the pins off or on
HIGH = on
LOW = off
```c
void pinDrains(int visibility);
```

Clear all pins of output
```c
void pinClear();
```

###Example
In the Arduino API
```c
#include <TPIC_Shifter.h>

// Add constants for the pins connected to the Arduino
// from the pinball circuit
const int dataPin = 9;
const int clockPin = 5;
const int latchPin = 6;
const int pinStatus = 7;
const int ballReturnPin = 2;
int max_lives = 6;
int num_connected = 8;

TPIC_Shifter pinballMachine = TPIC_Shifter(dataPin, clockPin, latchPin, pinStatus, ballReturnPin, max_lives, num_connected);

void setup() {
  // put your setup code here, to run once:
  pinballMachine.TPICBegin();
  pinballMachine.gameMode(LOSE_LIFE_MODE);
}

void loop() {
  // put your main code here, to run repeatedly:
  pinballMachine.firstRun();

  // You may want to do something here too
  if(pinballMachine.ballReturn() == true){
    pinballMachine.updateUI();
    pinballMachine.ledsFlash(10,20);
  }

  if(pinballMachine.endOfGame() == true){
    delay(1000);
    pinballMachine.ledsDance(10,30);
    pinballMachine.resetLives();
  }


}
```

### To do
* SPI is not implemented yet. It is experimental and may not work fully, or at all.
* Updates required to provide other UI Functions
* Need to explore improvements to the circuit to which this library is associated
* Refactor code to be more agnostic toward game mode


### License
License Copyright: Copyright © 2007 Free Software Foundation, Inc.
License License: "Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed." (www.gnu.org/licenses/gpl-3.0.en.html).
License Contact: Free Software Foundation (fsf.org).

SPDX short identifier: GPL-3.0-only
