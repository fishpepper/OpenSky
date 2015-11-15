# OpenSky

Note: THIS IS WORK IN PROGRESS.
      USE AT YOUR OWN RISK !!!

This is an open source implementation for the frsky protocol on
a cc2510 chip. This is compatible to a DIY RX or can be flashed
onto a FrSky vd5m.
This implementation will give you full 8-Channel CPPM output,
full telemetry (2Channels Analog + RSSI) and much more :)

Note: this will not work on any other Frsky RXs as they do not use
a different processor.

_Features_:
* completely open source (compiles with the opensource sdcc compiler)
* fully compatible to frsky 2-way protocol
* 8 Channel CPPM output
* failsafe (constant, stopped ppm output)
* 2 analog telemetry channels
* RSSI telemetry
* builtin APA102 Led control (maps to any a ppm channel)

_WARNINGS_:
* USE AT YOUR OWN RISK!
* I AM NOT RESPONSIBLE FOR ANY DAMAGE/INJURIES CAUSED BY USING THIS CODE!
* IF YOU BRICK YOUR RX DURING THE FLASH UPGRADE IT IS YOUR FAULT
* THERE IS NO WAY TO GO BACK TO THE ORIGINAL FIRMWARE ONCE YOU FLASH THIS CODE!

This is for educational purposes only, running the VD5M with this code
will probably void its FCC compliance and might void any transmission laws
depending on your country!

Please note that the cc2510 without an external PA chip as on the
VD5M will have limited range, DO NOT use this on a bigger Quadcopter.
Its meant to be used on indoor and/or small vehicles.

When debug is enabled during build (default for now) you will
get a vast amount of debug info on the serial port (CH5).
Hook this up to a 3.3V serial to usb connector in order to
see the debug information.


# Connections

<pre>
CH1 = BIND MODE (short to GND on startup to enter bind mode)
CH2 = ADC0
CH3 = ADC1
CH4 = CPPM OUT
CH5 = Debug UART @115200 8N1 (if compiled with debug enabled)
</pre>

(CH1 is at the same side as the LEDs)

You can connect 6 APA102 LEDs to Pins 2 (P2_1 = APA CLOCK) and 3 (P2_2 = APA DATA).
I uploaded a small and compact design on oshpark:
https://oshpark.com/shared_projects/BSjfJDwT
(I use that as a led bar on my nano quadcopters)


# BUGS

please report any bugs!

# Flashing

You will need a CC debugger or an arduino flashed with this code in order to program the cc2510:
https://github.com/fishpepper/CC2510Lib
(theres a python script to flash the cc2510 in that repo as well)

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

# Notes to Developers

_DO NOT_ use int16/32 in interrupts! mul/div etc for 16bit are not reentrant in SDCC
and are _NOT_ interrupt safe! There are ways to fix this issue but we do not
want to have such long operations in interrupts anyway. so do not use them ;)
(@see http://fivedots.coe.psu.ac.th/~cj/masd/resources/sdcc-doc/SDCCUdoc-14.html)

# Random notes:

Just in case you need to mount a new antenna:
my vd5m came with a 3.5cm antenna wire (shouldn't this by ~3cm??)...


# Thanks

Thanks to midelic from rcgroups.com for the reverse engineering and
code for the atmega implementation of this protocol!

