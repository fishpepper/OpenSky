#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "hal_defines.h"
#include <stdint.h>

//this has to be a power of 2 (2,4,8,16,32,...)
#define TELEMETRY_BUFFER_SIZE 64
typedef struct {
    uint8_t data[TELEMETRY_BUFFER_SIZE];
    uint8_t read;
    uint8_t read_ok;
    uint8_t write;
} telemetry_buffer_t;

extern volatile EXTERNAL_MEMORY telemetry_buffer_t telemetry_buffer;
extern volatile EXTERNAL_MEMORY uint8_t telemetry_expected_id;

void telemetry_init(void);
void telemetry_rx_callback(uint8_t data);
static void telemetry_rx_echo_test(void);
void telemetry_fill_buffer(volatile EXTERNAL_MEMORY uint8_t *buffer, uint8_t telemetry_id);
uint8_t telemetry_pop(volatile EXTERNAL_MEMORY uint8_t *byte);

#endif // __TELEMETRY_H__
