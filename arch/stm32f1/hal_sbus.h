#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__
#include <stdint.h>
#include "hal_defines.h"

void hal_sbus_init(EXTERNAL_MEMORY uint8_t *data_ptr);
static void hal_sbus_init_gpio(void);
static void hal_sbus_init_rcc(void);
static void hal_sbus_init_mode(void);
static void hal_sbus_init_nvic(void);
void hal_sbus_start_transmission(uint8_t *buffer, uint8_t len);

void SBUS_USART_IRQHANDLER(void);

//not used here
#define HAL_SBUS_PREPARE_DATA(a) (a)

#endif // __HAL_SBUS_H__
