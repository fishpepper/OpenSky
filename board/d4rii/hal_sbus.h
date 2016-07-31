#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__

void hal_sbus_init(void);
static void hal_sbus_init_gpio(void);
static void hal_sbus_init_rcc(void);
static void hal_sbus_init_mode(void);
static void hal_sbus_init_nvic(void);

void SBUS_USART_IRQHANDLER(void);

//not used here
#define HAL_SBUS_PREPARE_DATA(a) (a)

#endif // __HAL_SBUS_H__
