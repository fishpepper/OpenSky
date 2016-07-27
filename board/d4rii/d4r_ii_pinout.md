
MAIN CPU: STM32F103C8
www.st.com/resource/en/datasheet/stm32f103c8.pdf

EEPROM: 24C02
http://www.onsemi.com/pub_link/Collateral/CAT24C01-D.PDF

RF Chip: CC2500
www.ti.com/lit/ds/swrs040c/swrs040c.pdf

2x HF-SWITCH: RDAES02
http://www.rcgroups.com/forums/attachment.php?attachmentid=2992597

PA/LNA: RDAT212
http://d1.amobbs.com/bbs_upload782111/files_35/ourdev_611063XH619C.pdf


LAYOUT:

STM32 <==> CC2500 <==> SW1 <===> LNA/PA <==> SW2 <===> ANT1/2

Pinout SW1:
1 = RX  = T212.4 (LNA OUT)
2 = GND = GND
3 = TX  = T212.2 (PA_IN_P)
4 = CTX = STM32.38 (PA15, PULLUP VCC, CAP TO GND)
5 = ANT = --> CC2500
6 = CRX = STM32.40 (PB4, PULLUP VCC, CAP TO GND)

Pinout T212:
 1 = PA_IN_N   = ?
 2 = PA_IN_P   = SW1.3
 3 = Power Det = ? OPEN?
 4 = LNA OUT   = SW1.1
 5 = SW_PA     = STM32.38 (PA15, PULLUP VCC, CAP TO GND)
 6 = SW_LNA    = STM32.40 (PB4, PULLUP VCC, CAP TO GND)
 7 = SW_RX     = STM32.40 (PB4, PULLUP VCC, CAP TO GND)
 8 = SW_TX     = STM32.38 (PA15, PULLUP VCC, CAP TO GND)
 9 = ANT       = SW2.5 (ANT)
10 = NC        = NC
11 = NC        = NC
12 = NC        = NC
13 = LNAVCC    = ...
14 = PAVCC1    = ...
15 = VCBias    = ...
16 = PAVCC2    = ...

Pinout SW1:
1 = RX  = ANTENNA E2
2 = GND = GND
3 = TX  = ANTENNA E1
4 = CTX = STM32.4 (PC15, NO PULLx, CAP TO GND)
5 = ANT = T212.9 (ANT)
6 = CRX = STM32.3 (PC14, NO PULLx, CAP TO GND)

Pinout 24C02:
1 = ADDRESS0 = VCC (PULLUP 10K)
2 = ADDRESS1 = GND
3 = ADDRESS2 = GND
4 = GND      = GND
5 = SDA      = STM32.43 (PB7, PULLUP 10K)
6 = SCL      = STM32.42 (PB6, PULLUP 10K)
7 = WP       = STM32.45 (PB8, PULLUP 10K)
8 = VCC      = VCC

Pinout CC2500:
 1 = SCLK      = STM32.15 (PA5, SPI1_SCK)
 2 = SO (GDO1) = STM32.16 (PA6, SPI1_MISO)
 3 = GDO2      = STM32.13 (PA3, misc)
 4 = DVDD      = ...
 5 = DCOUPL    = ...
 6 = GDO0 ATST = only connected to Testpad (PAD) ??
 7 = CSn       = STM32.14 (PA4, SPI1_NSS)
 8 = XOSC_Q1   = 26MHZ crystal
 9 = AVDD      = ...
10 = XOSC_Q2   = 26MHZ crystal
11 = AVDD      = ...
12 = RF_P      = SW1 via some filtering network
13 = RF_N      = SW1 via some filtering network
14 = AVDD      = ...
15 = AVDD      = ...
16 = GND       = ...
17 = RBIAS     = ...
18 = DGUARD    = ...
19 = GND       = ...
20 = SI        = STM32.17 (PA7, SPI1_MOSI)



Pinout STM32:
...
5 = OSC_IN  = 12MHZ crystal
6 = OSC_OUT = 12MHZ crystal
...
11 = PA1 = ADC12_IN1 = --[330]--- AIN Pin [4PIN]
12 = PA2 = ADC12_IN2 = --[330]--- Voltage divider to Vsupply
...
18 = PB0  = ---[220R]--- OUT1
19 = PB1  = ---[220R]--- OUT2
20 = PB2  = ---[220R]--- OUT3 (PU or PD from PB2 as well!)
21 = PB10 (USART3_TX) = ---[220]--- OUT4
...
29 = PA8  = BIND BUTTON (switches to GND)
30 = PA9  = UART TX --[INVERTER]--- TX Pin [4PIN]
31 = PA10 = UART RX --[INVERTER]--- RX Pin [4PIN]
32 = PA11 = LED D3 (IO --|>|---[330R]---GND)
33 = PA12 = LED D4 (IO --|>|---[330R]---GND)
34 = PA13 = SWDIO ---[R12]--- GND
...
37 = PA14 = SWDCLK ---[RP2.4 - 10K]--- VCC (?)
...
44 = BOOT0 = PULLDOWN 10K, jumperpad R19 has VCC, use to enter bootloader!
...


Observations
============
DIVERSITY:
- binding: wait 4 frametimes on ANT1, then 4 frametimes on ANT2
- whenever there is a missing/broken frame -> switch ANTENNA !


CC25 SETUP:
/*
02 = 06
00 = 06
17 = 0C
18 = 18
06 = 19
07 = 04
08 = 05
3E = FF
0B = 68
0C = 00
0D = 5C
0E = 76
0F = 27
10 = AA (!)
11 = 39
12 = 11
13 = 23
14 = 7A
15 = 42
19 = 16
1A = 6C
1B = 03
1C = 46
1D = 91
21 = 56
22 = 10
23 = A9
24 = 0A
25 = 00
26 = 11
29 = 59
2C = 88
2D = 31
2E = 0B
03 = 07
04 = 00
02 = 06

pause

09 = 03
0A = 00
07 = 05 , 34

GDO2 HI->LO
FB DB
FB DB
36 3A 34
*/


