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

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "hal_spi.h"

static void pabort(const char *s) {
    perror(s);
    abort();
}

static const char *device = "/dev/spidev0.0";
static uint8_t bits = 8;
static uint32_t speed = 1000000;
static int fd = -1;
static uint32_t mode = 0;
static uint16_t delay = 20;

void hal_spi_init(void) {
    int ret = 0;

    /*
     * Open device
     */
    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi: mode: 0x%x\n", mode);
    printf("spi: bits per word: %d\n", bits);
    printf("spi: max speed: %d Hz (%f MHz)\n", speed, speed/1000000.0);
}

void hal_spi_dma_xfer(uint8_t *buffer, uint8_t len) {
    int ret;
    uint8_t rx;
#if LOG_SPI
    int i;
    for (i=0; i < len; i++)
        fprintf(stderr, "> %x\n", buffer[i]);
#endif  // LOG_SPI
    struct spi_ioc_transfer tr = {
        .tx_buf = (uint64_t)buffer,
        .rx_buf = (uint64_t)buffer,
        .len = len,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");
#if LOG_SPI
    for (i=0; i < len; i++)
        fprintf(stderr, "< %x\n", buffer[i]);
    fprintf(stderr, "\n");
#endif  // LOG_SPI
}
