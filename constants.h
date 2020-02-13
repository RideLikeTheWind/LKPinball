/****** ONLY USE IF YOU KNOW WHAT YOU ARE DOING ******/

// Define as true if you're planning to shift using SPI
#define USE_SPI false

//Define if you want to debug (writes to Serial Monitor)
#define DEBUG true

#if USE_SPI
#include <SPI.h>
#endif

int currentLives = MAX_LIVES; // because the chip is zero-index
#define PULSE_DELAY 10

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