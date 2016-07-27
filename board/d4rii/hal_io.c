#include "hal_io.h"
#include "pin_config.h"
#include  "stm32f10x_rcc.h"
#include  "stm32f10x_gpio.h"

void hal_io_init(void) {
    //configure bind io as input:
    GPIO_InitTypeDef gpio_init;

    //periph clock enable for port
    RCC_APB2PeriphClockCmd(BIND_JUMPER_GPIO_CLK, ENABLE);

    //configure led output as push-pull
    gpio_init.GPIO_Pin = BIND_JUMPER_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(BIND_JUMPER_GPIO, &gpio_init);
}

uint8_t hal_io_bind_request(void) {
    //Returs pin state (1 if HIGH, 0 if LOW)
    if (GPIO_ReadInputDataBit(BIND_JUMPER_GPIO, BIND_JUMPER_PIN)){
        //HIGH -> button not pressed
        return 0;
    }else{
        //LOW -> button pressed
        return 1;
    }
}




