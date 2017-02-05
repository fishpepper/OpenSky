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
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#include "hal_soft_serial.h"
#include "soft_serial.h"
#include "debug.h"
#include "delay.h"
#include "led.h"
#include "wdt.h"
#include "config.h"
#include "portmacros.h"
#include "hal_cc25xx.h"

#ifndef HUB_TELEMETRY_ON_SBUS_UART

void hal_soft_serial_init(void) {
    debug("hal_soft_serial: init\n"); debug_flush();
    hal_soft_serial_init_gpio();
    hal_soft_serial_init_interrupts();
}

void hal_soft_serial_init_gpio(void) {
    // set gpio as input
    PORT2DIR(HUB_TELEMETRY_PORT) &= ~(1<<HUB_TELEMETRY_PIN);
}

void hal_soft_serial_init_interrupts(void) {
     OVFIM = 0;

    // disable compare modes
    T4CCTL0 = 0;
    T4CCTL1 = 0;

    // tickspeed = 26MHz / 8 = 3,25MHz (TICKSPD is set in hal_timeout.c!)
    // 1us = 3.25 ticks -> DIV2 -> 1us = 1.625 ticks
    T4CTL = T4CTL_DIV_2 | // /2
            T4CTL_START | // start
            T4CTL_OVFIM | // OVInt enabled
            T4CTL_CLR   | // clear
            T4CTL_MODE_MODULO; // 01 = count to CC and the overflow

    // ch0 cmp: bit length
    T4CC0 = HAL_SOFTSERIAL_BIT_DURATION_TICKS;

    // clear pending interrupt flags (IRCON is reset by hw)
    T4OVFIF = 0;

    // overflow causes an int -> reload next channel data
    OVFIM = 1;

    // enable T4 interrups
    IEN1 |= IEN1_T4IE;

    // clear pending port ints
    P0IFG    = 0;
    P0IF     = 0;

    // enable interrupts on P0 4...7
    PICTL   |= PICTL_P0IENH;
    P0SEL &= ~(1<<6);

    // set edge:
#ifdef HUB_TELEMETRY_INVERTED
    // rising edge triggers isr
    PICTL   &= ~PICTL_P0ICON;
#else
    // falling edge triggers isr
    PICTL   |= PICTL_P0ICON;
#endif

    // T4 highest int prio (group4)
    IP0 |= (1<<4);
    IP1 |= (1<<4);
    // P0 highest int prio (group5)
    IP0 |= (1<<5);
    IP1 |= (1<<5);

    // enable interrupts from P0
    P0IF = 0;
    P0IFG = 0;
    IEN1 |= IEN1_P0IE;
}


void hal_soft_serial_update_interrupt(void) __interrupt T4_VECTOR{
    if (T4OVFIF) {
        // DEBUG_PIN_TOGGLE();

        // re-arm for the next bit
        HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(HAL_SOFTSERIAL_BIT_DURATION_TICKS-1);

        if (soft_serial_process_databit()) {
            // finished transmission, disable UP and enable IC isr
            IEN1 |= IEN1_P0IE;
            IEN1 &= ~IEN1_T4IE;
        }

        // clear pending interrupt flags (IRCON is reset by hw)
        T4OVFIF = 0;
        P0IFG = 0;
        P0IF  = 0;
    }
}

void hal_soft_serial_startbit_interrupt(void) __interrupt P0INT_VECTOR{
    uint8_t isr_cause = P0IFG;

    // clear int flags WARNING: order seems to be important! CLR first IFG then IF!
    P0IFG = 0;
    // clear P0 int flags (important: several P0 pins can cause the isr, clean all of them!)
    P0IF = 0;

    if (isr_cause & (1<<HUB_TELEMETRY_PIN)) {
        // DEBUG_PIN_TOGGLE();
        // reset t3 counter:
        T4CTL |= T4CTL_CLR;
        // disable IC interrupt (only compare match interrupts will follow)
        IEN1 &= ~IEN1_P0IE;
        // enable overflow isr
        IEN1 |= IEN1_T4IE;

        // this is the startbit -> re synchronize the timer to this
        // by setting the next cc interrupt to 1/2 bit length:
        HAL_SOFT_SERIAL_UPDATE_TOP_VALUE((HAL_SOFTSERIAL_BIT_DURATION_TICKS / 2)-1);

        // clear pending int flags
        P0IF    = 0;
        T4OVFIF = 0;

        // process
        soft_serial_process_startbit();
    }
}

#endif
