#ifndef __HAL_DEBUG_H__
#define __HAL_DEBUG_H__

#include <stdint.h>

void hal_debug_init(void);
void hal_debug_start_transmission(uint8_t ch);
uint8_t hal_debug_int_enabled(void);
void hal_debug_int_enable(void);
#define hal_debug_int_disable() {}
#define DEBUG_ISR(void) hal_debug_tx_interrupt(void)
#define HAL_DEBUG_ISR_FLAG_SET() (1)
#define HAL_DEBUG_ISR_CLEAR_FLAG() { }
#define HAL_DEBUG_ISR_DISABLE() { }
#define HAL_DEBUG_TX_DATA(data) { }


#endif // __HAL_DEBUG_H__
