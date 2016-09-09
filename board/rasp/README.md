# Raspberry target

This is a target to build OpenSky and run using a raspberry pi, with an CC2500 chip connected directly to the spi bus. Using the raspberry target is an easy way for experimenting with the code.

OpenSky requires good timing to be able to do frequency hoping correctly, but an ideling raspberry pi seems to do just fine.

Here is how i connected up my module:

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

