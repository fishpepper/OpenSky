#include "hal_soft_spi.h"
#include "soft_spi.h"

void hal_soft_spi_init(void) {
    GPIO_InitTypeDef gpio_init;

    //periph clock enable for port
    RCC_APB2PeriphClockCmd(SOFT_SPI_GPIO_CLK, ENABLE);

    //configure output as push-pull
    gpio_init.GPIO_Pin = SOFT_SPI_MOSI | SOFT_SPI_SCK;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SOFT_SPI_GPIO, &gpio_init);


}


