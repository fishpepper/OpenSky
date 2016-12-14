# OpenSky


[![Build Status](https://travis-ci.org/fishpepper/OpenSky.svg?branch=master)](https://travis-ci.org/fishpepper/OpenSky)

This is an open source implementation for the pololar frsky protocol using
a cc25xx transceiver chip. This software can be flashed on a DIY RX,
a FrSky VD5M, or a FrSky D4R-ii receiver.
Support for other FrSky receivers could be added. E.g. porting this to a X4R should be really easy.

This custom firmware implementation will give you full 8-Channel SBUS or CPPM output,
full telemetry (2Channels Analog + RSSI) and much more :)

## Warning

*THIS SOFTWARE IS FOR EDUCATIONAL USE ONLY*  
*DO NOT* use this software to control real planes/quadrocopters.  

This is for educational use only, bad things could happen
if you run this on a real vehicle.
Its meant to be used on indoor and/or small vehicles.

Using this code will probably void its FCC compliance and might 
void any transmission laws depending on your country!

I AM NOT RESPONSIBLE FOR ANY DAMAGE/INJURIES CAUSED BY USING THIS CODE!


Additionally: Do not blame me if you brick your RX during the flash upgrade.
There is currently no way back to the original stock firmware once you erase/flash the devcie 
with my code.

# Features

* completely open source (compiles with the opensource arm gcc or sdcc compiler)
* fully compatible to frsky 2-way protocol
* 8 Channel CPPM output OR digital SBUS output (INVERTED or non-INVERTED mode!)
* failsafe (constant, stopped ppm output)
* 2 analog telemetry channels
* RSSI telemetry
* builtin APA102 Led control (maps to any a ppm channel)

# Supported platforms

* FrSky VD5M
* FrSky D4r-ii
* [DIY uSKY - a tiny 0.4g receiver](http://fishpepper.de/projects/usky/)
* raspberry pi + cc2500 module (for easy development)

# Debugging

When debug is enabled during build (default for now) you will
get a vast amount of debug info on the serial port (CH5 on VD5M, CH4 on D4R-ii).
Hook this up to a 3.3V serial to usb connector in order to see the debug information
on a terminal

# Connections

The connection depends on the receiver and the firmware configuration (main.h)

## VD5M:
<pre>
(CH1 is at the same side as the LEDs)
CH1 = BIND MODE (short to GND on startup to enter bind mode) / hub telemetry input (9600)
CH2 = ADC0
CH3 = ADC1
CH4 = CPPM OUT or SBUS (inverted or non-inverted, see main.h)
CH5 = Debug UART @115200 8N1 (if compiled with debug enabled)
</pre>

You can connect 6 APA102 LEDs to Pins 2 (P2_1 = APA CLOCK) and 3 (P2_2 = APA DATA).
NOTE: you can not flash the target as long as the APA leds are connected to P2_1,P2_2!

## D4R-ii
Servo Port:
<pre>
CH1 = PPM out (if sbus is disabled)
CH2 = APA102 DATA
CH3 = APA102 CLK
CH4 = Debug UART @115200 8N1 OR non-inverted SBUS (see main.h)
</pre>

Serial Port:
<pre>
(Pin 1 = left = the same side as the LEDs)
[1] = GND
[2] = AD2 input (max 3.3V!)
[3] = inverted SBUS (if enabled in main.h) or Debug UART 
[4] = NC
</pre>

## rasp
<pre>
Raspberry Pi V2        CC2500

16 GPIO23              GDO2
17 VCC                 VCC
19 MOSI                SI
21 MISO                SO
23 SCLK                SCLK
25 GND                 GND

03 GPIO02              LNA (Optional, if your chip have LNA)
05 GPIO03              PA  (Optional, if your chip have PA)

37 GPIO26              BIND (pull up to enter bind mode)
08 TXD                 SBUS (output using hardware uart)
40 GPIO21              PPM  (software bit banged, low quality)
</pre>




# BUGS

There are probably thousands of bugs. Again: DO NOT USE THIS ON A REAL PLANE/QUADROCOPTER!
Please report any bugs!

# Flashing

Depending on your target you will need a programmer or a serial converter to flash the software

## VD5M:

You will need a CC debugger or an arduino flashed with this code in order to program the cc2510:
https://github.com/fishpepper/CC2510Lib
(theres a python script to flash the cc2510 in that repo as well)

Alternatively, you can use a raspberry pi to flash. This does not require any voltage dividers:
https://github.com/jimmyw/CC2510Lib

Connections:

It is handy to mount a 5pin Molex Picoblade connector to the
5pin ISP connection on the side of the vd5m. This way it is easy
to upgrade firmware.

ISP Port connection on vd5m
<pre>
[1] = VCC (3.3V)
(2) = P2_1 = DBG DATA
(3) = P2_2 = DBG CLOCK
(4) = GND
(5) = RESET
</pre>

(pin 1 is on the same side as CH1-5)

The arduino code is rather slow (>30s) whereas the cc-tool flashes in 1s in fast mode!

Notes on using cc-tool on linux:
make sure to use the patched version from here:
https://github.com/dashesy/cc-tool
(this fix is mandatory, otherwise you get a pipe error msg
-> https://github.com/dashesy/cc-tool/commit/3ebc61763ff5d0dadbdc2f7163e85ca5d002bb0f)

## D4R-ii

This target can be flashed using either a ST-Link V2 or by using the STM32 serial bootloader.

### ST-Link

Connection:
<pre>
GND    = GND
SWDIO  = R12 (the non-gnd side connected to STM32 pin 34)
SWDCLK = RP2, pin4 (connected to to STM32 pin 37)
</pre>

Connect the STLink, power the target and run "make flash" to program the device.

### Serial bootloader

The STM32 series of chips has a ROM based un-brickable bootloader. In order to enter the bootloader
temporarily short circuit the Jumperpad R19 to enter the boot loader. The D4R-ii has a signal inverter
on board, thus the serial lines are inverted.

Connect an inverted serial cable to the 4pin connector (FTDI devices can be configured to invert rx and tx using mprog!)
<pre>
(Pin 1 = left = the same side as the LEDs)
[1] = GND
[2] = AD2 = do not connect for flashing
[3] = inverted TX ---> connect to RX 
[4] = inverted RX ---> connect to TX
</pre>

TODO: add flash command to makefile/doc

# Random notes:

Just in case you need to mount a new antenna: My vd5m came with a 3.5cm antenna wire. However it should be 31.5mm long (1/4 wavelength at 2.4GHz)


# Thanks

Thanks to midelic from rcgroups.com for the reverse engineering and
code for the atmega implementation of the frsky protocol!
Thanks to holger for the d4r-ii donation ;)

