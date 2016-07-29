#ifndef __WDT_H__
#define __WDT_H__
#include "hal_wdt.h"

void wdt_init(void);
#define wdt_reset() hal_wdt_reset();


#endif
