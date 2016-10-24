#ifndef __HAL_TIMEOUT__H_
#define __HAL_TIMEOUT__H_

#include <stdint.h>

void hal_timeout_init(void);
void hal_timeout_set(uint32_t ms);
void hal_timeout_set_100us(uint32_t hus);
uint8_t hal_timeout_timed_out(void);
void hal_timeout_delay_us(int32_t timeout);

void hal_timeout2_set(uint32_t ms);
void hal_timeout2_set_100us(uint32_t hus);
uint8_t hal_timeout2_timed_out(void);
void hal_timeout_add_ppm(uint32_t us);

//void hal_timeout_delay_100us(uint32_t timeout);
uint32_t hal_timeout_time_remaining(void);


#endif // __HAL_TIMEOUT__H_
