# Arduino target

This target is tested on a Ardunio Pro Mini, but the arch will work on any board that runs Atmel Mega 328P if you clone and update the board specification.
My goal is to support an open firmware for all the microquads around.

Remember to run the arduino on 3.3 volts, as the CC2500 chip will die on 5V, mine also have a 8Mhz crystal as 16Mhz is considered unstable for 3.3 volt use.

You need to connect a CC2500 chip using SPI.

This version is using the hardware SPI implementation of the Arduino, now Software SPI is currently available so SPI can not be remapped for the moment.

Also there is no SBUS support, please feel free to help.

Here is how to build and flash:
```
make TARGET=arduino TOOLROOT=/path/to/toolchain BOARD_SPEED=8000000 V=1 SERIAL_PORT=/dev/tty.SLAB_USBtoUART SERIAL_SPEED=57600 clean flash
```

You will se debug serial output on the uart0 at 115200bps.

## Pin-Mapping CC2500:

```
D2  GDO2
D6  PA-Enable (For boards with output amplifier)
D7  LNA-Enable (For board with input amplifier)
D10 CSN
D11 MOSI
D12 MISO
D13 SCK
```

## Other:

```
D9 PPM out
D3 Bind (input), Put positive to enter bind mode
A1 Green led
A2 Red led
```

