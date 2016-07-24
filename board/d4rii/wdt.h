#ifndef __WDT_H__
#define __WDT_H__

void wdt_init(void);
#define wdt_reset() hal_wdt_reset();


#endif
