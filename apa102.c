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

#include "apa102.h"
#include "debug.h"
#include "delay.h"
#include "wdt.h"
#include "soft_spi.h"

//led data
EXTERNAL_MEMORY uint8_t apa102_txdata[APA102_TXDATA_LEN];
EXTERNAL_MEMORY uint8_t apa102_txdata_index;


#define APA102_DEBUG_STATEMACHINE 0

void apa102_init(){
    uint8_t i; //,j;
    debug("apa102: init\n"); debug_flush();

    apa102_txdata_index = 0;

    //init spi (will take care of I/O dir)
    soft_spi_init();

    //DISABLE LEDS:
    for(i=0; i<APA102_LED_COUNT; i++){
        apa102_set_rgb(i, 0, 0, 0);
    }

    //trigger transmission:
    apa102_flush();
}

void apa102_flush(void){
    if (!apa102_statemachine()){
        //still flushing data, abort
        return;
    }

    apa102_start_transmission();
    while(!apa102_statemachine()){}
}

void apa102_show_no_connection(void){
    uint8_t i;

    for(i=0; i<APA102_LED_COUNT; i++){
        apa102_set_rgb(i, 0, 0, 20);
    }

    //trigger transmission:
    apa102_flush();
}

void apa102_update_leds(EXTERNAL_MEMORY uint16_t *data, uint8_t link_qual){
    uint8_t i;
    uint8_t led[3] = {0,0,0};
    uint16_t throttle;

    if (!apa102_statemachine()){
        //still flushing data, abort
        return;
    }

    //outer leds are bright green:
    apa102_set_rgb(0, 0, 255, 0);
    apa102_set_rgb(APA102_LED_COUNT-1, 0, 255, 0);

    //inner leds show throttle
    //throttle is data[2] = (~1500...3000)
    #define APA102_NEUTRAL_THROTTLE 2000
    throttle = data[2];
    if (throttle > APA102_NEUTRAL_THROTTLE){
        //2200-3000 (800) ---> 0-120
        //throttle > 1/2 --> green lights!
        led[1] = (throttle-APA102_NEUTRAL_THROTTLE)>>4; //G
    }else{
        //1500-2200 (700) -> 0-100
        //throttle < 1/2 -> red
        led[0] = (APA102_NEUTRAL_THROTTLE-throttle)>>4; //R
    }

    if (link_qual < 40){
        //bad -> red
        led[0] = 180; //R
        led[1] =   0; //G
        led[2] =   0; //B
    }else if (link_qual < 60){
        //somewhat bad, yellow
        led[0] = 120; //R
        led[1] = 120; //G
        led[2] =   0; //B
    }

    //debug_put_uint8((data[2]>>5)&0xFF);

    //set leds in middle
    for (i=1; i<APA102_LED_COUNT-1; i++){
        apa102_set_rgb(i,led[0]>>1,led[1]>>1,led[2]>>1);
    }
}

void apa102_set_rgb(uint8_t index, uint8_t r, uint8_t g, uint8_t b){
    //check if not out of bounds:
    if (index >= APA102_LED_COUNT){
        //not a valid index, ignore & return
        return;
    }

    //rgb data starts at index 4
    index = (4*index) + 4;
    //global brightness
    apa102_txdata[index] = 0xFF;
    index++;
    //actually its BGR data:
    apa102_txdata[index] = b;
    index++;
    apa102_txdata[index] = g;
    index++;
    apa102_txdata[index] = r;
}

void apa102_start_transmission(void){
    uint8_t i;

    //make sure the framing data is 0x00 and 0xff:
    //header = 0x00
    apa102_txdata[0] = 0x00;
    apa102_txdata[1] = 0x00;
    apa102_txdata[2] = 0x00;
    apa102_txdata[3] = 0x00;

    //footer = 0xFF
    for (i=4+4*APA102_LED_COUNT; i<APA102_TXDATA_LEN; i++){
        apa102_txdata[i] = 0xFF;
    }

    apa102_txdata_index = 0;
}

//this is meant to be called regulary
//advantage: non-blocking + very short runtime,
//spreads full transfer over multiple calls
uint8_t apa102_statemachine(void){
    //transmit data if ongoing transfer
    if (apa102_txdata_index < APA102_TXDATA_LEN){
        /*debug_put_hex8(apa102_txdata[apa102_txdata_index]);
        debug_putc(' ');
        debug_flush();*/

        //send data
        soft_spi_tx(apa102_txdata[apa102_txdata_index]);
        apa102_txdata_index++;
    }else{
        //finished
        return 1;
    }

    //not finished
    return 0;
}

