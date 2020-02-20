
## TPIC6B595 Library

### Description
This library is designed for the Texas Instruments TPIC6C595 Serial to Parallel Shift Register. It was adapted from a similar library by Derek Chafin to suit a specific need.
It utilises a circuit designed to control a Pinball Machine at Loreto Kirribilli. The library will not work correctly without that chip. If you require more information please contact the author.

### Authorship
Author: J Mascorella  
Github: @iridelikethewind
Year: 2020  

### Config
Open the Config.h file and set parameters. You will need to supply
NUM_CONNECTED as an integer of the number of lights connected to the system
MAX_LIVES as an integer of the number of lives the pinball machine allows
Do not edit or add other parameters in that fonfig file
```c
#define NUM CONNECTED 8
#define MAX_LIVES 3
```

### Functions(params)

#### Setup
Constructor
```c
TPIC_Shifter(int dataPin, int clockPin, int latchPin, int clearPin, int ballReturnButton);
```

Setup function 
Must run in the void setup() loop in Arduino
```c
void TPICBegin();
```

#### UI functions
**BLOCKING**
Flashes all lights on and off
repeats = number of flashes
speed = delay in ms for lights to remain on (1000 = 1 second)
```c
void TPICFlash(int repeats, int speed);
```

**BLOCKING**
Lights roll around quickly
repeats = number of repeats you want (about 1 per second)
startPin = pin you want it to start from
```c 
void ledsDance(int repeats, int startPin);
```

Turn on or off a specific pin
pin = decimal number that converts to an 8 bit byte (00000000 - 11111111 as 0 - 255)
```c
void specificPin(int pins);
```

#### Game Functions
Manages the returned ball from the pinball machine returns true if button is pressed
Use to fire other utility or UI functions
```c
bool ballReturn();
```

Reset lives in the game
Only used internally
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

### To do
* SPI is not implemented yet. It is experimental and may not work fully, or at all. 
* Updates required to provide other UI Functions
* Need to explore improvements to the circuit to which this library is associated


### License
License Copyright: Copyright © 2007 Free Software Foundation, Inc.
License License: "Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed." (www.gnu.org/licenses/gpl-3.0.en.html).
License Contact: Free Software Foundation (fsf.org).

SPDX short identifier: GPL-3.0-only
