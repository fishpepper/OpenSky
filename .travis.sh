#!/bin/bash
cat /usr/include/linux/spi/spidev.h
REVISION=$(git rev-parse --short HEAD)
make clean
make
