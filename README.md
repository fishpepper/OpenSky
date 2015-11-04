# OpenSky

this is an open source implementation for the frsky protocol on a cc2510 chip (e.g. on a vd5m)

WARNING: USE AT YOUR OWN RISK! I AM NOT RESPONSIBLE FOR ANY DAMAGE/INJURIES CAUSED BY USING THIS CODE!

Please note that the cc2510 withour a PA chip as on the VD5M has limited range,
DO NOT use this on a bigger Quadcopter. Its meant to be used on indoor and/or small vehicles.

# Features

Note: work in progress. not everything is implemented yet

* completely open source (compiles with the opensource sdcc compiler)
* fully compatible to frsky 2-way protocol
* [*] 8 Channel PPM output
* [*] failsafe (constant, stopped ppm output)
* 2 analog telemetry channels
* [*] builtin APA102 Led control

[*] = not yet implemented.

# Flashing

You will need a CC debugger or an arduino flashed with this code in order to program the cc2510:
https://github.com/fishpepper/CC2510Lib
(theres a python script to flash the cc2510 in that repo as well)
Connections: TODO


# Thanks

Thanks to midelic from rcgroups.com for the reverse engineering and 
code for the atmega implementation of this protocol!
 

