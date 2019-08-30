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

#include "hal_uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"

#define BAUD_PRESCALE (F_CPU / 4 / USART_BAUDRATE - 1) / 2

// &UBRRH, &UBRRL, &UCSRA, &UCSRB, &UCSRC, &UDR
// &UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0

volatile uint8_t hal_usart_txe_is_on;

ISR(USART_UDRE_vect)
{
  // Disable this interrupt
  UCSR0B &= ~(1<<UDRIE0);

  hal_uart_interrupt();
}

void hal_uart_init(void) {
    hal_usart_txe_is_on = 0;
    UCSR0A = 1 << U2X0;
    UBRR0H = BAUD_PRESCALE >> 8;
    UBRR0L = BAUD_PRESCALE;
    UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    UCSR0B |= (1<<RXCIE0);
    UCSR0B &= ~(1<<UDRIE0);
    sei();
}

void hal_uart_start_transmission(uint8_t ch) {
    hal_usart_txe_is_on = 1;

    // Atmel practice seems to enable EIE and wait for it before sending first char, but here we block until its free.
    while (( UCSR0A & ( 1 << UDRE0 ) ) == 0) {}
    UDR0 = ch;

    // Set Data Register Empty Interrupt Enable, will trigger hal_uart_interrupt() when buffer is ready.
    UCSR0B |= (1<<UDRIE0);
}

void hal_uart_tx_data(uint8_t ch) {
    UCSR0B &= ~(1 << UDRIE0);
    UDR0 = ch;
}

uint8_t hal_uart_isr_is_set() {
    return 1;
}

void hal_uart_isr_disable() {
    // Clear Data Register Empty Interrupt Enable, will not trigger hal_uart_interrupt() anymore.
    UCSR0B &= ~(1 << UDRIE0);
    hal_usart_txe_is_on = 0;
}

uint8_t hal_uart_int_enabled(void) {
    return hal_usart_txe_is_on;
}


