#ifndef __HAL_CLOCKSOURCE_H__
#define __HAL_CLOCKSOURCE_H__

#include "hal_cc25xx.h"

void hal_clocksource_init(void);

// bit mask used to check the stability of XOSC
#define CLOCKSOURCE_XOSC_STABLE_BIT   0x40
// bit mak used to check the stability of the High-frequency RC oscillator
#define CLOCKSOURCE_HFRC_STB_BIT      0x20
// bit maks used to power down system clock oscillators
#define CLOCKSOURCE_OSC_PD_BIT        0x04  

// bit mask used to control the system clock oscillator
#define CLOCKSOURCE_MAIN_OSC_BITS     0x7F  

// bit mask used to select/check the system clock oscillator
#define CLOCKSOURCE_OSC_BIT           0x40  

// macros to check for stable oscs:
#define CLOCKSOURCE_HFRC_OSC_STABLE() (SLEEP & CLOCKSOURCE_HFRC_STB_BIT)
#define CLOCKSOURCE_XOSC_STABLE()     (SLEEP & CLOCKSOURCE_XOSC_STABLE_BIT)



#define CLOCKSOURCE_XOSC 0
#define CLOCKSOURCE_HFRC 1

#endif
