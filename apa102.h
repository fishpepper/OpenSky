#ifndef __APA102_H__
#define __APA102_H__
#include <cc2510fx.h>
#include <stdint.h>
#include "config.h"

//how many leds?
#define APA102_LED_COUNT 6

//led data is:
//4   x 0x00
//n   x 0xFF BB GG RR
//n/2 x 0xFF
#define APA102_TXDATA_LEN (4+4*APA102_LED_COUNT+4+4)
extern __xdata uint8_t apa102_txdata[APA102_TXDATA_LEN];

void apa102_init(void);
void apa102_start_transmission(void);
uint8_t apa102_statemachine(void);
void apa102_flush(void);
void apa102_set_rgb(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void apa102_update_leds(__xdata uint16_t *data, uint8_t link_qual);
void apa102_show_no_connection(void);

#endif
