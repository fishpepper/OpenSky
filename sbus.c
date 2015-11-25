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

   author: fishpepper <AT> gmail.com
*/

#include "main.h"
#include "config.h"
#include "debug.h"
#include "wdt.h"
#include "sbus.h"
#include "failsafe.h"

__xdata uint16_t sbus_data[24];

void sbus_init(void){
    debug("sbus: init\n"); debug_flush();

    //start in failsafe mode:
    failsafe_enter();

    /*
    //set up UART:
#define BAUD 100000//SBUS 1000000bauds
    #include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    //Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    while ( UCSR0A & (1 << RXC0) )//flush receive buffer
        UDR0;
    //enable reception and RC complete interrupt
    UCSR0B |= (1<<TXEN0);//tx enable


    sbus_nodata_counter = 0;*/

    debug("sbus: init done\n"); debug_flush();
}

void sbus_start_transmission(){

}


void sbus_update(__xdata uint16_t *data){
/*

    sbus[0]=SBUS_SYNCBYTE;
    sbus[1]=lowByte(channel[0]);//
    sbus[2]=highByte(channel[0]) | lowByte(channel[1])<<3;
    sbus[3]=(channel[1]>>5)|(channel[2]<<6);//
    sbus[4]=(channel[2]>>2)& 0xff;
    sbus[5]=(channel[2]>>10)|lowByte(channel[3])<<1;
    sbus[6]=(channel[3]>>7)|lowByte(channel[4])<<4;
    sbus[7]=(channel[4]>>4)|lowByte(channel[5])<<7;
    sbus[8]=(channel[5]>>1)& 0xff;
    sbus[9]=(channel[5]>>9)|lowByte(channel[6])<<2;
    sbus[10]=(channel[6]>>6)|lowByte(channel[7])<<3;
    sbus[11]=(channel[7]>>3)& 0xff;
    //
    sbus[12]=lowByte(channel[8]);
    sbus[13]=highByte(channel[8]) | lowByte(channel[9])<<3;
    sbus[14]=(channel[9]>>5)|(channel[10]<<6);
    sbus[15]=(channel[10]>>2)& 0xff;
    sbus[16]=(channel[10]>>10)|lowByte(channel[11])<<1;
    sbus[17]=(channel[11]>>7)|lowByte(channel[12])<<4;
    sbus[18]=(channel[12]>>4)|lowByte(channel[13])<<7;
    sbus[19]=(channel[13]>>1)& 0xff;
    sbus[20]=(channel[13]>>9)|lowByte(channel[14])<<2;
    sbus[21]=(channel[14]>>6)|lowByte(channel[15])<<3;
    sbus[22]=(channel[15]>>3)& 0xff;//end
     //Flags sbus[23]
     //bit 7=ch17(0x80)
     //bit 6=ch18(0x40)
    //bit 5=frame lost(0x20)
    //bit4=failsafe activated(0x10)
     //bit3=birt2=bit1=bit0=n/a
    //sbus[23]=0xC0;// 11000000/FS 11010000
    sbus[23]=0x10;// 11000000/FS 11010000
    sbus[24]=SBUS_ENDBYTE;//endbyte


*/
    //exit failsafe mode
    failsafe_exit();
}

void sbus_exit_failsafe(void){
    debug("sbus: exit FS\n");

}

void sbus_enter_failsafe(void){
    //failsafe is active
    debug("sbus: entered FS\n");
}





