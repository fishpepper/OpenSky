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
#include "hal_defines.h"
#include "debug.h"
#include "uart.h"
#include "wdt.h"
#include "delay.h"
#include "config.h"
#include "sbus.h"
#include "ppm.h"
#include "frsky.h"
#include "failsafe.h"
#include "hal_sbus.h"

#ifdef SBUS_ENABLED
EXTERNAL_MEMORY uint8_t sbus_data[SBUS_DATA_LEN];

//SBUS is:
//100000bps inverted serial stream, 8 bits, even parity, 2 stop bits
//frame period is 7ms (HS) or 14ms (FS)
void sbus_init(void){
    debug("sbus: init\n"); debug_flush();

    hal_uart_init();

    //start in failsafe mode:
    failsafe_enter();

    debug("sbus: init done\n"); debug_flush();
}

void sbus_start_transmission(uint8_t frame_lost){
    uint8_t tmp;
    //debug("sbus: TX\n");

    //set up flags:
    //bit 7 = discrete channel 17
    //bit 6 = discrete channel 18
    //bit 5 = frame lost
    //bit 4 = Failsafe
    //failsafe + frame lost will be set below,
    //discrete channels are zero
    tmp = 0x00;

    //failsafe active?
    if (failsafe_active){
        //clear failsafe flag:
        tmp |= SBUS_FLAG_FAILSAFE_ACTIVE;
    }

    //check if this frame was lost
    if (frame_lost == SBUS_FRAME_LOST){
        tmp |= SBUS_FLAG_FRAME_LOST;
    }

    //copy flags to buffer
    sbus_data[23] = HAL_SBUS_PREPARE_DATA(tmp);

    //send data!
    uart_start_transmission(sbus_data, SBUS_DATA_LEN-1);
}


void sbus_update(EXTERNAL_MEMORY uint16_t *data){
    uint8_t i;
    EXTERNAL_MEMORY uint16_t rescaled_data[8];
    int16_t tmp;
    uint16_t rssi_scaled;

    //rescale input data:
    //frsky input is us*1.5
    //under normal conditions this is ~1480...3020
    //when tx is set to 125% this is  ~1290...3210
    //remap this to 0...2047 -> first substract 1290
    //center was at 2250us, is now at 960, remap this to ~1023:
    //1023/960 = 1,065625.. = approx by 17/16 = 1 1/16
    //sbus data = (input-1290)*(1 1/16) = (input-1290) + (input-1290)/16
    for(i=0; i<8; i++){
        tmp = data[i] - 1290;
        tmp = tmp + (tmp>>4);
        tmp = tmp - 25;  // move center to 1500
        if (tmp < 0){
            rescaled_data[i] = 0;
        }else if(tmp > 2047){
            rescaled_data[i] = 2047;
        }else{
            rescaled_data[i] = tmp;
        }
    }

    //rescale frsky rssi to 0..2047 (TODO: find correct conversion)
    rssi_scaled = frsky_rssi*20;
    rssi_scaled = min(2047, rssi_scaled);

    //sbus transmits up to 16 channels with 11bit each.
    //build up channel data frame:
    sbus_data[ 0] = HAL_SBUS_PREPARE_DATA( SBUS_SYNCBYTE );

    //bits ch 0000 0000
    sbus_data[ 1] = HAL_SBUS_PREPARE_DATA( LO(rescaled_data[0]) );
    //bits ch 1111 1000
    sbus_data[ 2] = HAL_SBUS_PREPARE_DATA( (LO(rescaled_data[1])<<3) | HI(rescaled_data[0]) );
    //bits ch 2211 1111
    sbus_data[ 3] = HAL_SBUS_PREPARE_DATA( (rescaled_data[1]>>5) | (rescaled_data[2]<<6) );
    //bits ch 2222 2222
    sbus_data[ 4] = HAL_SBUS_PREPARE_DATA( (rescaled_data[2]>>2) & 0xFF );
    //bits ch 3333 3332
    sbus_data[ 5] = HAL_SBUS_PREPARE_DATA( (rescaled_data[2]>>10) | (LO(rescaled_data[3])<<1) );
    //bits ch 4444 3333
    sbus_data[ 6] = HAL_SBUS_PREPARE_DATA( (rescaled_data[3]>>7) | (LO(rescaled_data[4])<<4) );
    //bits ch 5444 4444
    sbus_data[ 7] = HAL_SBUS_PREPARE_DATA( (rescaled_data[4]>>4) | (LO(rescaled_data[5])<<7) );
    //bits ch 5555 5555
    sbus_data[ 8] = HAL_SBUS_PREPARE_DATA( (rescaled_data[5]>>1) & 0xFF );
    //bits ch 6666 6655
    sbus_data[ 9] = HAL_SBUS_PREPARE_DATA( (rescaled_data[5]>>9) | (LO(rescaled_data[6])<<2) );
    //bits ch 7776 6666
    sbus_data[10] = HAL_SBUS_PREPARE_DATA( (rescaled_data[6]>>6) | (LO(rescaled_data[7])<<5) );
    //bits ch 7777 7777
    sbus_data[11] = HAL_SBUS_PREPARE_DATA( (rescaled_data[7]>>3) & 0xFF );
    //bits ch 8888 8888
    sbus_data[12] = HAL_SBUS_PREPARE_DATA( rssi_scaled & 0xFF);
    //bits ch 9999 9888
    sbus_data[13] = HAL_SBUS_PREPARE_DATA( HI(rssi_scaled) );
    //fill up to ch15 with zero
    for(i=14; i<23; i++){
        sbus_data[i] = HAL_SBUS_PREPARE_DATA(0x00);
    }
    //sbus flags, will be set by start transmission...
    sbus_data[23] = HAL_SBUS_PREPARE_DATA(0x00);

    //EOF frame:
    sbus_data[24] = HAL_SBUS_PREPARE_DATA(SBUS_ENDBYTE);
}

void sbus_exit_failsafe(void){
    //debug("sbus: exit FS\n");
}

void sbus_enter_failsafe(void){
    //failsafe is active
    //debug("sbus: entered FS\n");
}

#endif


