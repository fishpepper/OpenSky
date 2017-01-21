# xsr-sb target

##LEDs

The XSR has a green and a red LED.

##SBUS

SBUS output is on USART1, TX pin is PA9, RX is PA10 but unused. SBUS is inverted leaving the board.

##CC2500 Radio

CC2500 is connected to SPI2, SCK is PB13, MISO is PB14 and MOSI is PB15.
The CS pin is PA8, and the GDO2 pin is PA11 

The XSR uses the Skyworks SE2431 range extender
See: http://www.skyworksinc.com/uploads/documents/SE2431L_202410J.pdf

Looks to be a similar design to this: http://www.ti.com/tool/CC2530-SE2431L

Which means the RX pin selection is not needed (it is RX if TX is off)

Either ANT can be used for both RX and TX.

High is logic 1, Low is logic 0

ANT (PB5) selection is 0 (left) or 1 (right)
CTX (PA15) is 0 then RX LNA mode active, or 1 then tx mode
CPS (PB3) is 0 then bypass mode is active, 1 is bypass off.
CSD (PB4) with 1 to enable chip (0 = sleep)

GDO2 is on PA11 

##BIND

Bind pin is attached to PC13, and pushing button pulls to ground.

##EEPROM
 
EEPROM is connected to PA0 (SCL) and PA1 (SDA) 

Note that this is **NOT** an I2C port on the F1 so must use SOFT I2C

Write enable pin is connected to PXX *TODO*

##Analogue

NOTE no analogue input for XSR, and no servo pins.

##CPPM Output

CPPM output is on PB12

##Telemetry (Smart Port)
    
Smart port telemetry input is RX USART3 - RX on PB11, TX on PB10, this is a polling protocol, so TX is used too.

The plan is to not enable the smart port protocol but use the older hub telemetry (D8) protocol, so that the unit can share a UART on the FC with SBUS!

PB11 for RX and PB10 for TX both inverted once leaving board.
Output Enable (OE) for TX is PA2, this will need to be pulled high to prevent transmissions on the sport line.

TX (PB10) can then be used for debugging output (requires soldering to board before inverter). 

