/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com, jimmyw <AT> github
*/

#include "arduino_pins.h"

#define USART_BAUDRATE 115200
#define DEFAULT_FSCAL_VALUE -69


/* MAP Arduino pins to functions */


#define DD_MOSI_PIN MOSI_PIN // PIN11
#define DD_MOSI_PORT MOSI_PORT
#define DD_MISO_PIN MISO_PIN // PIN12
#define DD_MISO_INP MISO_INP
#define DD_SCK_PIN  SCK_PIN // PIN13
#define DD_SCK_PORT  SCK_PORT
#define DD_SS_PIN D10_PIN
#define DD_SS_PORT D10_PORT

#define DD_GDO_PIN D2_PIN
#define DD_GDO_INP D2_INP

#define BIND_PIN D3_PIN
#define BIND_INP D3_INP
#define DEBUG_PIN D4_PIN
#define DEBUG_PORT D4_PORT
#define DEBUG2_PIN D5_PIN
#define DEBUG2_PORT D5_PORT
#define PA_EN_PIN D6_PIN
#define PA_EN_PORT D6_PORT
#define LNA_EN_PIN D7_PIN
#define LNA_EN_PORT D7_PORT
#define PPM_PIN D9_PIN
#define PPM_PORT D9_PORT

#define GREEN_LED_PIN A1_PIN
#define GREEN_LED_PORT A1_PORT
#define RED_LED_PIN A2_PIN
#define RED_LED_PORT A2_PORT

