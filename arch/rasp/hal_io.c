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
   along with this program.  If not, see <http:// www.gnu.org/licenses/>.

author: fishpepper <AT> gmail.com
*/



#include "hal_io.h"
#include "hal_cc25xx.h"
#include "delay.h"


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sched.h>

#define BLOCK_SIZE (4*1024)

// I/O access
volatile static unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

void setup_io()
{
    void *gpio_map;
    int  mem_fd;
    /* open /dev/mem */
    if ((mem_fd = open ("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC) ) < 1) {
        printf("can't open /dev/gpiomem \n");
        exit(-1);
    }

    /* mmap GPIO */
    gpio_map = mmap(
            NULL,             // Any adddress in our space will do
            BLOCK_SIZE,       // Map length
            PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
            MAP_SHARED,       // Shared with other processes
            mem_fd,           // File to map
            0x200000          // GPIO controller
            );

    close(mem_fd); // No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map);// errno also set!
        exit(-1);
    }

    // Always use volatile pointer!
    gpio = (volatile unsigned *)gpio_map;
}

#define GDO0  23
#define LNA 2
#define PA 3
#define PPM 21
#define BIND 26

int hal_spi_get_gdo0(void) {
    int res = GET_GPIO(GDO0);
    if (res != 0)
        return res != 0;
}

void hal_io_init(void) {
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &param )==-1) {
        printf("Unable to set scheduler priority, this might impact your performance.");
    }
    setup_io();
    INP_GPIO(GDO0); // must use INP_GPIO before we can use OUT_GPIO

    INP_GPIO(LNA);
    INP_GPIO(PA);
    OUT_GPIO(LNA);
    OUT_GPIO(PA);

    OUT_GPIO(PPM);
    OUT_GPIO(PPM);
    INP_GPIO(BIND);
}

void hal_set_amp(int pa) {
    if (pa == HAL_PA) {
        GPIO_CLR = 1 << LNA;
        delay_us(20);
        GPIO_SET = 1 << PA;
        delay_us(5);
    } else {
        GPIO_CLR = 1 << PA;
        delay_us(20);
        GPIO_SET = 1 << LNA;
        delay_us(5);
    }
}

void hal_set_ppm(int state) {
    if (state)
        GPIO_SET = 1 << PPM;
    else
        GPIO_CLR = 1 << PPM;
}

uint8_t hal_io_bind_request(void) {
    return !!GET_GPIO(BIND);
}
