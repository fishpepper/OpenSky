#ifndef __LED_H__
#define __LED_H__

#include "config.h"

//set/clear/toggle
#define LED_GREEN_INIT()   { LED_GREEN_DIR |= (1<<LED_GREEN_PIN); LED_GREEN_OFF(); }
#define LED_GREEN_OFF() { LED_GREEN_BIT = 0; } //{ LED_GREEN_PORT &= ~(1<<LED_GREEN_PIN);  }
#define LED_GREEN_ON()  { LED_GREEN_BIT = 1; } //{ LED_GREEN_PORT |= (1<<LED_GREEN_PIN); }
//set/clear/toggle
#define LED_RED_INIT()   { LED_RED_DIR |= (1<<LED_RED_PIN); LED_RED_OFF(); }
#define LED_RED_OFF() { LED_RED_BIT = 0; }
#define LED_RED_ON()  { LED_RED_BIT = 1; }

//led init routines
#define LED_INIT() {  LED_RED_INIT(); LED_GREEN_INIT(); }

#endif


