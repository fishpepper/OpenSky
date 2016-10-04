#ifndef __SBUS_H__
#define __SBUS_H__
#include <stdint.h>

#if SBUS_ENABLED

void sbus_init(void);
void sbus_update(EXTERNAL_MEMORY uint16_t *data);
void sbus_start_transmission(uint8_t frame_lost);
void sbus_exit_failsafe(void);
void sbus_enter_failsafe(void);

//best match for 100kbit/s = 99975.5859375 bit/s
//baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define SBUS_BAUD_E 11
#define SBUS_BAUD_M 248

#define SBUS_DATA_LEN 25
extern EXTERNAL_MEMORY uint8_t sbus_data[SBUS_DATA_LEN];

#define SBUS_SYNCBYTE 0x0F
#define SBUS_ENDBYTE  0x00
#define SBUS_FLAG_FRAME_LOST      (1<<2)
#define SBUS_FLAG_FAILSAFE_ACTIVE (1<<3)
#define SBUS_FRAME_LOST 1
#define SBUS_FRAME_NOT_LOST 0

#else

#define sbus_init() {}
#define sbus_update(a) {}
#define sbus_start_transmission(a) {}
#define sbus_exit_failsafe() {}
#define sbus_enter_failsafe() {}

#endif

#endif
