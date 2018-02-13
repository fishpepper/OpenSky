#ifndef __HAL_TIMEOUT__H_
#define __HAL_TIMEOUT__H_

#include <stdint.h>
#include "hal_defines.h"

void hal_timeout_init(void);
void hal_timeout_set(__IO uint32_t ms);
uint8_t hal_timeout_timed_out(void);

void hal_timeout2_set_100us(__IO uint32_t hus);
uint8_t hal_timeout2_timed_out(void);


#endif // __HAL_TIMEOUT__H_
