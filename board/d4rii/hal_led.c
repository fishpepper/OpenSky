#include "hal_led.h"
#include  "stm32f10x_rcc.h"

void _hal_led_init(uint16_t pin) {
	GPIO_InitTypeDef gpio_init;
	
	//periph clock enable for port
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	//configure led output as push-pull
	gpio_init.GPIO_Pin = pin;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_GPIO, &gpio_init);
}
