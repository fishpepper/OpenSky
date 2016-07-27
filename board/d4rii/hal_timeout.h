#ifndef __HAL_TIMEOUT__H_
#define __HAL_TIMEOUT__H_

#include "stm32f10x.h"

void hal_timeout_init(void);
void hal_timeout_set(__IO uint32_t ms);
uint8_t hal_timeout_timed_out(void);
void hal_timeout_delay_ms(uint32_t timeout);
void hal_timeout_delay_100us(uint32_t timeout);


void SysTick_Handler(void);

#endif // __HAL_TIMEOUT__H_