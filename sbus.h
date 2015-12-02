#ifndef __SBUS_H__
#define __SBUS_H__
#include <stdio.h>
#include <stdint.h>
#include <cc2510fx.h>
#include "main.h"
#include "config.h"

#if SBUS_ENABLED

//this helper routine will invert the data
//stored in buffer in case the sbus is set
//to inverted
#if SBUS_INVERTED
#define SBUS_PREPARE_DATA(a) (0xFF ^ (a))
#else
#define SBUS_PREPARE_DATA(a) (a)
#endif

void sbus_init(void);
void sbus_update(__xdata uint16_t *data);
void sbus_start_transmission(uint8_t frame_lost);
void sbus_exit_failsafe(void);
void sbus_enter_failsafe(void);
void sbus_uart_set_mode(__xdata union uart_config_t *cfg);

//best match for 100kbit/s = 99975.5859375 bit/s
//baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define SBUS_BAUD_E 11
#define SBUS_BAUD_M 248

#define SBUS_DATA_LEN 25
extern __xdata uint8_t sbus_data[SBUS_DATA_LEN];

#define SBUS_SYNCBYTE 0x0F
#define SBUS_ENDBYTE  0x00
#define SBUS_FLAG_FRAME_LOST      (1<<2)
#define SBUS_FLAG_FAILSAFE_ACTIVE (1<<3)
#define SBUS_FRAME_LOST 1
#define SBUS_FRAME_NOT_LOST 0

#endif

#endif
