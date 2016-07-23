#include "hal_led.h"
#include  "stm32f10x_rcc.h"

void _hal_led_init(uint8_t pin) {
	GPIO_InitTypeDef gpio_init;
	
	//periph clock enable for port
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//Configure PA11 and PA12 as output pushpull:
	gpio_init.GPIO_Pin = (1<<pin);
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpio_init);
}
