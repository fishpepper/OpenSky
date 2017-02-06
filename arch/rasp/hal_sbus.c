/*
    Copyright 2017 fishpepper <AT> gmail.com

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

#include <stdio.h>
#include <unistd.h>         // Used for UART
#include <fcntl.h>          // Used for UART
#include <asm/termios.h>        // Used for UART

#include "hal_sbus.h"
#include "debug.h"
#include "delay.h"
#include "sbus.h"

#if SBUS_ENABLED

static int uart0_filestream = -1;

void hal_sbus_init(EXTERNAL_MEMORY uint8_t *sbus_data_ptr) {
    // open in non blocking read/write mode
    uart0_filestream = open("/dev/ttyAMA0", O_WRONLY | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

    // USART configuration:
    // 100000bps inverted serial stream, 8 bits, even parity, 2 stop bits
    // no hw flow control
    struct termios2 options;
    options.c_cflag = BOTHER | CS8 | CLOCAL | PARENB | CSTOPB;
    options.c_iflag = 0;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_ispeed = 100000;
    options.c_ospeed = 100000;
    if (ioctl(uart0_filestream, TCSETS2, &options) != 0) {
        printf("ioctl TCSETS2 failed");
    }
}

void hal_sbus_start_transmission(uint8_t *data, uint8_t len) {
    if (uart0_filestream != -1) {
        int count = write(uart0_filestream, data, len);
        if (count < len) {
            printf("UART TX error sent %u of %u\n", count, len);
        }
        // printf("%d bytes written\n", len);
    }
}

#endif  // SBUS_ENABLED
