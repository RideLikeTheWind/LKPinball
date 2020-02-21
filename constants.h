#ifndef CONSTANTS_H
#define CONSTANTS_H

/****** ONLY USE IF YOU KNOW WHAT YOU ARE DOING ******/
/****** Basically, don't edit this file... ***********/

// Define as true if you're planning to shift using SPI
#define USE_SPI false

//Define if you want to debug (writes to Serial Monitor)
#define DEBUG true

#if USE_SPI
#include <SPI.h>
#endif

#define PULSE_DELAY 10

#endif