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

#include "hal_soft_spi.h"
#include "soft_spi.h"

void hal_soft_spi_init(void) {
    GPIO_InitTypeDef gpio_init;

    //periph clock enable for port
    RCC_APBxPeriphClockCmd(SOFT_SPI_GPIO_CLK_RCC, SOFT_SPI_GPIO_CLK, ENABLE);

    //configure output as push-pull
    gpio_init.GPIO_Pin = SOFT_SPI_MOSI | SOFT_SPI_SCK;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SOFT_SPI_GPIO, &gpio_init);
}


