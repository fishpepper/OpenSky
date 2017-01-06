/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/
#include "hal_io.h"
#include "hal_defines.h"
#include "config.h"
#include "led.h"
#include  "stm32f10x_rcc.h"
#include  "stm32f10x_gpio.h"

void hal_io_init(void) {
    //configure bind io as input:
    GPIO_InitTypeDef gpio_init;

    //periph clock enable for port
    RCC_APBxPeriphClockCmd(BIND_JUMPER_GPIO_CLK_RCC, BIND_JUMPER_GPIO_CLK, ENABLE);

    //configure bind input as pullup
    gpio_init.GPIO_Pin = BIND_JUMPER_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode = GPIO_Mode_IPU;
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




