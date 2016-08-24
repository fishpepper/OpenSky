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
#include <string.h>
#include <stdio.h>
#include "frsky.h"
#include "telemetry.h"
#include "debug.h"
#include "timeout.h"
#include "led.h"
#include "delay.h"
#include "wdt.h"
#include "cc25xx.h"
#include "io.h"
#include "storage.h"
#include "adc.h"
#include "ppm.h"
#include "apa102.h"
#include "failsafe.h"
#include "sbus.h"

//this will make binding not very reliable, use for debugging only!
#define FRSKY_DEBUG_BIND_DATA 1
#define FRSKY_DEBUG_HOPTABLE 1

//hop data & config
//EXTERNAL_MEMORY uint8_t storage.frsky_txid[2] = {0x16, 0x68};
//EXTERNAL_MEMORY uint8_t storage.frsky_hop_table[FRSKY_HOPTABLE_SIZE] = {0x01, 0x42, 0x83, 0xC4, 0x1A, 0x5B, 0x9C, 0xDD, 0x33, 0x74, 0xB5, 0x0B, 0x4C, 0x8D, 0xCE, 0x24, 0x65, 0xA6, 0xE7, 0x3D, 0x7E, 0xBF, 0x15, 0x56, 0x97, 0xD8, 0x2E, 0x6F, 0xB0, 0x06, 0x47, 0x88, 0xC9, 0x1F, 0x60, 0xA1, 0xE2, 0x38, 0x79, 0xBA, 0x10, 0x51, 0x92, 0xD3, 0x29, 0x6A, 0xAB};
//EXTERNAL_MEMORY int8_t storage.frsky_freq_offset;
EXTERNAL_MEMORY uint8_t frsky_current_ch_idx;

//diversity counter
EXTERNAL_MEMORY uint8_t frsky_diversity_count;

//rssi
EXTERNAL_MEMORY uint8_t frsky_rssi;
EXTERNAL_MEMORY uint8_t frsky_link_quality;

//pll calibration
EXTERNAL_MEMORY uint8_t frsky_calib_fscal1_table[FRSKY_HOPTABLE_SIZE];
EXTERNAL_MEMORY uint8_t frsky_calib_fscal2;
EXTERNAL_MEMORY uint8_t frsky_calib_fscal3;
//EXTERNAL_MEMORY int16_t storage.frsky_freq_offset_acc;

//rf rxtx buffer
EXTERNAL_MEMORY volatile uint8_t frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE];
EXTERNAL_MEMORY volatile uint8_t frsky_packet_received;
EXTERNAL_MEMORY volatile uint8_t frsky_packet_sent;


void frsky_init(void){
    //uint8_t i;
    debug("frsky: init\n"); debug_flush();

    cc25xx_init();

    frsky_link_quality = 0;
    frsky_diversity_count = 0;
    frsky_packet_received = 0;
    frsky_packet_sent = 0;

    frsky_rssi = 100;

    // check if spi is working properly
    frsky_show_partinfo();

    // init frsky registersttings for cc2500
    frsky_configure();

    if (frsky_bind_jumper_set()){
        //do binding
        frsky_do_bind();
        //binding will never return/continue
    }

    //show info:
    debug("frsky: using txid 0x"); debug_flush();
    debug_put_hex8(storage.frsky_txid[0]);
    debug_put_hex8(storage.frsky_txid[1]);
    debug_put_newline();

    //init txid matching
    frsky_configure_address();

    //tune cc2500 pll and save the values to ram
    frsky_calib_pll();

    debug("frsky: init done\n");debug_flush();
}


void frsky_show_partinfo(void) {
    uint8_t partnum, version;
    //start idle
    cc25xx_strobe(RFST_SIDLE);

    //check version:
    debug("frsky: cc25xx partnum 0x");
    partnum = cc25xx_get_register_burst(PARTNUM);
    debug_put_hex8(partnum);

    debug(" version 0x");
    version = cc25xx_get_register_burst(VERSION);
    debug_put_hex8(version);
    debug_put_newline();

    if (cc25xx_partnum_valid(partnum, version)){
        debug("frsky: got valid part and version info\n");
    }else{
        debug("frsky: got INVALID part and version info?!\n");
    }
    debug_flush();
}

void frsky_configure(void){
    debug("frsky: configure\n"); debug_flush();

    //start idle
    cc25xx_strobe(RFST_SIDLE);

    // IOCFG0,1,2 is set in hal code (it is specific to the board used)
    cc25xx_set_gdo_mode();

    //normal config
    cc25xx_set_register(MCSM1    ,0x0F); //go back to rx after transmission completed //0x0C = stay idle;
    cc25xx_set_register(MCSM0    ,0x18);
    cc25xx_set_register(PKTLEN   ,FRSKY_PACKET_LENGTH); //on 251x this has to be exactly our size
    cc25xx_set_register(PKTCTRL0 ,0x05);
    cc25xx_set_register(PA_TABLE0,0xFF);
    cc25xx_set_register(FSCTRL1  ,0x08); //D4R-II seems to set 0x68 here ?! instead of 0x08
    cc25xx_set_register(FSCTRL0  ,0x00);
    //set base freq 2404 mhz
    cc25xx_set_register(FREQ2    ,0x5C);
    cc25xx_set_register(FREQ1    ,0x76);
    cc25xx_set_register(FREQ0    ,0x27);
    cc25xx_set_register(MDMCFG4  ,0xAA);
    cc25xx_set_register(MDMCFG3  ,0x39);
    cc25xx_set_register(MDMCFG2  ,0x11);
    cc25xx_set_register(MDMCFG1  ,0x23);
    cc25xx_set_register(MDMCFG0  ,0x7A);
    cc25xx_set_register(DEVIATN  ,0x42);
    cc25xx_set_register(FOCCFG   ,0x16);
    cc25xx_set_register(BSCFG    ,0x6C);
    cc25xx_set_register(AGCCTRL2 ,0x03);
    cc25xx_set_register(AGCCTRL1 ,0x40); //D4R uses 46 instead of 0x40);
    cc25xx_set_register(AGCCTRL0 ,0x91);
    cc25xx_set_register(FREND1   ,0x56);
    cc25xx_set_register(FREND0   ,0x10);
    cc25xx_set_register(FSCAL3   ,0xA9);
    cc25xx_set_register(FSCAL2   ,0x05);
    cc25xx_set_register(FSCAL1   ,0x00);
    cc25xx_set_register(FSCAL0   ,0x11);
    //???FSTEST   , 0x59);
    cc25xx_set_register(TEST2    ,0x88);
    cc25xx_set_register(TEST1    ,0x31);
    cc25xx_set_register(TEST0    ,0x0B);
    //???FIFOTHR  = 0x07);
    cc25xx_set_register(ADDR     ,0x00);

    //for now just append status
    cc25xx_set_register(PKTCTRL1, CC2500_PKTCTRL1_APPEND_STATUS);
    debug("frsky: configure done\n"); debug_flush();
}

uint8_t frsky_bind_jumper_set(void){
    debug("frsky: BIND jumper set = "); debug_flush();
    if (io_bind_request()){
        debug("YES -> binding\n");
        return 1;
    }else{
        debug("NO -> no binding\n");
        return 0;
    }
}

void frsky_do_bind(void){
    debug("frsky: do bind\n"); debug_flush();

    //set txid to bind channel
    storage.frsky_txid[0] = 0x03;

    //frequency offset to zero (will do auto tune later on)
    storage.frsky_freq_offset = 0;

    //init txid matching
    frsky_configure_address();

    //set up leds:frsky_txid
    led_red_off();
    led_green_on();

    //start autotune:
    frsky_autotune();

    //now run the actual binding:
    frsky_fetch_txid_and_hoptable();

    //important: stop RF interrupts:
    cc25xx_disable_rf_interrupt();

    //save to persistant storage:
    storage_write_to_flash();

    //done, end up in fancy blink code
    debug("frsky: finished binding. please reset\n");
    led_green_on();

    while(1){
        led_red_on();
        delay_ms(500);
        wdt_reset();

        led_red_off();
        delay_ms(500);
        wdt_reset();
    }
}


void frsky_autotune(void){
    uint8_t done = 0;
    uint8_t received_packet = 0;
    uint8_t state = 0;
    int8_t fscal0_min=127;
    int8_t fscal0_max=-127;
    int16_t fscal0_calc;

    debug("frsky: autotune\n"); debug_flush();

    //enter RX mode
    frsky_enter_rxmode(0);

    //find best offset:
    storage.frsky_freq_offset = 0;

    debug("frsky: entering bind loop\n"); debug_flush();

    led_red_off();

    //search for best fscal 0 match
    while(state != 5){
        //reset wdt
        wdt_reset();

        //handle any ovf conditions
        frsky_handle_overflows();

        //search full range quickly using binary search
        switch(state){
        default:
        case(0):
            //init left search:
            storage.frsky_freq_offset = -127;
            state = 1;
            break;

        case(1):
            //first search quickly through the full range:
            if (storage.frsky_freq_offset < 127-10){
                storage.frsky_freq_offset += 9;
            }else{
                //done one search, did we receive anything?
                if (received_packet){
                    //finished, go to slow search
                    storage.frsky_freq_offset = fscal0_min - 9;
                    state = 2;
                }else{
                    //no success, lets try again
                    state = 0;
                }
            }
            break;

        case(2):
            if (storage.frsky_freq_offset < fscal0_max+9){
                storage.frsky_freq_offset++;
            }else{
                //done!
                state = 5;
            }
            break;
        }

        //go to idle
        cc25xx_strobe(RFST_SIDLE);

        //set freq offset
        cc25xx_set_register(FSCTRL0, storage.frsky_freq_offset);

        FSCTRL0 = storage.frsky_freq_offset;
        led_red_off();

        //go back to RX:
        delay_ms(1);
        cc25xx_strobe(RFST_SRX);

        //set timeout
        timeout_set(50);
        done = 0;

        led_green_on();
        led_red_off();

        //debug("tune "); debug_put_int8(storage.frsky_freq_offset); debug_put_newline(); debug_flush();

        while((!timeout_timed_out()) && (!done)){
            //handle any ovf conditions
            frsky_handle_overflows();

            cc25xx_process_packet(&frsky_packet_received, (volatile uint8_t *)&frsky_packet_buffer, FRSKY_PACKET_BUFFER_SIZE);

            if (frsky_packet_received){
                //prepare for next packet:
                frsky_packet_received = 0;
                cc25xx_enable_receive();
                cc25xx_strobe(RFST_SRX);

                //valid packet?
                if (FRSKY_VALID_PACKET_BIND(frsky_packet_buffer)){
                    //bind packet!
                    debug_putc('B');

                    //packet received
                    received_packet = 1;

                    //this fscal value is done
                    done = 1;

                    //update min/max
                    fscal0_min = min(fscal0_min, storage.frsky_freq_offset);
                    fscal0_max = max(fscal0_max, storage.frsky_freq_offset);

                    //make sure we never read the same packet twice by crc flag
                    frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE-1] = 0x00;
                }

                /*debug("[");debug_flush();
        uint8_t cnt;
                for(cnt=0; cnt<FRSKY_PACKET_BUFFER_SIZE; cnt++){
                    debug_put_hex8(frsky_packet_buffer[cnt]);
                    debug_putc(' ');
                    debug_flush();
                }
                debug("]\n"); debug_flush();*/

            }
        }
        if (!done){
            debug_putc('-');
        }
    }

    //set offset to what we found out to be the best:
    fscal0_calc = (fscal0_max + fscal0_min)/2;

    debug("\nfrsky: fscal0 ");
    debug_put_int8(fscal0_min);
    debug(" - ");
    debug_put_int8(fscal0_max);
    debug_put_newline();
    debug_flush();

    //store new value
    storage.frsky_freq_offset = fscal0_calc;

    cc25xx_strobe(RFST_SIDLE);

    //set freq offset
    cc25xx_set_register(FSCTRL0, storage.frsky_freq_offset);

    //go back to RX:
    delay_ms(1);
    cc25xx_strobe(RFST_SRX);

    debug("frsky: autotune done. offset=");
    debug_put_int8(storage.frsky_freq_offset);
    debug_put_newline();
    debug_flush();
}


void frsky_enter_rxmode(uint8_t channel){
    cc25xx_strobe(RFST_SIDLE);

    cc25xx_enter_rxmode();

    //set & do a manual tuning for the given channel
    frsky_tune_channel(channel);

    cc25xx_enable_receive();

    //go back to rx mode
    cc25xx_strobe(RFST_SRX);
}


void frsky_configure_address(void){
    // start idle
    cc25xx_strobe(RFST_SIDLE);

    //freq offset
    cc25xx_set_register(FSCTRL0, storage.frsky_freq_offset);

    //never automatically calibrate, po_timeout count = 64
    //no autotune as (we use our pll map)
    cc25xx_set_register(MCSM0,0x08);

    //set address
    cc25xx_set_register(ADDR, storage.frsky_txid[0]);

    //append status, filter by address, autoflush on bad crc, PQT=0
    cc25xx_set_register(PKTCTRL1, CC2500_PKTCTRL1_APPEND_STATUS | CC2500_PKTCTRL1_CRC_AUTOFLUSH | CC2500_PKTCTRL1_FLAG_ADR_CHECK_01);
}


void frsky_tune_channel(uint8_t ch){
    //start idle
    cc25xx_strobe(RFST_SIDLE);

    //set channel number
    cc25xx_set_register(CHANNR, ch);

    //start Self calib:
    cc25xx_strobe(RFST_SCAL);

    //wait for scal end
    //either delay_us(800) or check MARCSTATE:
    while(cc25xx_get_register(MARCSTATE) != 0x01);

    //now FSCAL3..1 shold be set up correctly! yay!
}

void frsky_handle_overflows(void) {
    uint8_t marc_state;

    //fetch marc status
    marc_state = cc25xx_get_register(MARCSTATE) & 0x1F;
    if (marc_state == 0x11){
        debug("frsky: RXOVF\n");
        //flush rx buf
        cc25xx_strobe(RFST_SFRX);
        //cc25xx_strobe(RFST_SIDLE);
    }else if (marc_state == 0x16){
        debug("frsky: TXOVF\n");
        //flush tx buf
        cc25xx_strobe(RFST_SFTX);
        //cc25xx_strobe(RFST_SIDLE);
    }
}


void frsky_fetch_txid_and_hoptable(void){
    uint16_t hopdata_received = 0;
    uint8_t index;
    uint8_t i;

    //enter RX mode
    frsky_enter_rxmode(0);

#define MAX_BIND_PACKET_COUNT 10
    //DONE when n times a one:
#define HOPDATA_RECEIVE_DONE ((1<<(MAX_BIND_PACKET_COUNT))-1)

    //clear txid:
    storage.frsky_txid[0] = 0;
    storage.frsky_txid[1] = 0;

    //timeout to wait for packets
    timeout_set(9*3+1);

    //fetch hopdata array
    while(hopdata_received != HOPDATA_RECEIVE_DONE){
        //reset wdt
        wdt_reset();

        //handle any ovf conditions
        frsky_handle_overflows();

        //FIXME: this should be handled in a cleaner way.
        //as this is just for binding, stay with this fix for now...
        if (timeout_timed_out()){
            //do diversity
            cc25xx_switch_antenna();

            debug_putc('m');

            //next packet should be there ein 9ms
            //if no packet for 3*9ms -> reset rx chain:
            timeout_set(3*9+1);

            //re-prepare for next packet:
            cc25xx_strobe(RFST_SIDLE);
            //TESTME: moved to rx_sleep....
            //delay_ms(1);
            frsky_packet_received = 0;
            cc25xx_rx_sleep();
            cc25xx_enable_receive();
            cc25xx_strobe(RFST_SRX);
        }

        //process incoming data
        cc25xx_process_packet(&frsky_packet_received, (volatile uint8_t *)&frsky_packet_buffer, FRSKY_PACKET_BUFFER_SIZE);

        if (frsky_packet_received){
            debug_putc('p');

            //prepare for next packet:
            frsky_packet_received = 0;
            cc25xx_enable_receive();
            cc25xx_strobe(RFST_SRX);


#if FRSKY_DEBUG_BIND_DATA
            if (FRSKY_VALID_FRAMELENGTH(frsky_packet_buffer)){
                debug("frsky: RX ");
                debug_flush();
                for(i=0; i<FRSKY_PACKET_BUFFER_SIZE; i++){
                    debug_put_hex8(frsky_packet_buffer[i]);
                    debug_putc(' ');
                }
                debug_put_newline();
            }
#endif


            //do we know our txid yet?
            if (FRSKY_VALID_PACKET_BIND(frsky_packet_buffer)){
                //next packet should be ther ein 9ms
                //if no packet for 3*9ms -> reset rx chain:
                timeout_set(3*9+1);

                debug_putc('B');
                if ((storage.frsky_txid[0] == 0) && (storage.frsky_txid[1] == 0)){
                    //no! extract this
                    storage.frsky_txid[0] = frsky_packet_buffer[3];
                    storage.frsky_txid[1] = frsky_packet_buffer[4];
                    //debug
                    debug("frsky: got txid 0x");
                    debug_put_hex8(storage.frsky_txid[0]);
                    debug_put_hex8(storage.frsky_txid[1]);
                    debug_put_newline();
                }

                //this is actually for us
                index = frsky_packet_buffer[5];

                //valid bind index?
                if (index/5 < MAX_BIND_PACKET_COUNT){
                    //copy data to our hop list:
                    for(i=0; i<5; i++){
                        if ((index+i) < FRSKY_HOPTABLE_SIZE){
                            storage.frsky_hop_table[index+i] = frsky_packet_buffer[6+i];
                        }
                    }
                    //mark as done: set bit flag for index
                    hopdata_received |= (1<<(index/5));
                }else{
                    debug("frsky: invalid bind idx");
                    debug_put_uint8(index/5);
                    debug_put_newline();
                }

                //make sure we never read the same packet twice by crc flag
                frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE-1] = 0x00;
            }
        }
    }

#if FRSKY_DEBUG_BIND_DATA
    debug("frsky: hop[] = ");
    for(i=0; i<FRSKY_HOPTABLE_SIZE; i++){
        debug_put_hex8(storage.frsky_hop_table[i]);
        debug_putc(' ');
        debug_flush();
    }
    debug_putc('\n');
#endif

    //idle
    cc25xx_strobe(RFST_SIDLE);
}

void frsky_calib_pll(void){
    uint8_t i;
    uint8_t ch;

    debug("frsky: calib pll\n");

    //fine tune offset
    cc25xx_set_register(FSCTRL0, storage.frsky_freq_offset);

    debug("frsky: tuning hop[] =");

    //calibrate pll for all channels
    for(i=0; i<FRSKY_HOPTABLE_SIZE; i++){
        //reset wdt
        wdt_reset();

        //fetch channel from hop_table:
        ch = storage.frsky_hop_table[i];

        //debug info
        debug_putc(' ');
        debug_put_hex8(ch);

        //set channel number
        frsky_tune_channel(ch);

        //store pll calibration:
        frsky_calib_fscal1_table[i] = cc25xx_get_register(FSCAL1);
    }
    debug_put_newline();

    //only needed once:
    frsky_calib_fscal3 = cc25xx_get_register(FSCAL3);
    frsky_calib_fscal2 = cc25xx_get_register(FSCAL2);

    //return to idle
    cc25xx_strobe(RFST_SIDLE);

    debug("frsky: calib fscal1 = ");
    for(i=0; i<FRSKY_HOPTABLE_SIZE; i++){
        debug_put_hex8(frsky_calib_fscal1_table[i]);
        debug_putc(' ');
        debug_flush();
    }
    debug("\nfrsky: calib fscal2 = 0x");
    debug_put_hex8(frsky_calib_fscal2);
    debug("\nfrsky: calib fscal3 = 0x");
    debug_put_hex8(frsky_calib_fscal3);
    debug_put_newline();
    debug_flush();

    debug("frsky: calib pll done\n");
}

void frsky_main(void){
    uint8_t send_telemetry = 0;
    uint8_t requested_telemetry_id = 0;
    uint8_t missing = 0;
    uint8_t hopcount = 0;
    uint8_t stat_rxcount = 0;
    //uint8_t badrx_test = 0;
    uint8_t conn_lost = 1;
    uint8_t packet_received = 0;
    //uint8_t i;

    debug("frsky: starting main loop\n");

    //start with any channel:
    frsky_current_ch_idx = 0;

    //first set channel uses enter rxmode, this will set up dma etc
    frsky_enter_rxmode(storage.frsky_hop_table[frsky_current_ch_idx]);
    cc25xx_strobe(RFST_SRX); //D4R-II addition!

    //wait 500ms on the current ch on powerup
    timeout_set(500);

    //start with conn lost (allow full sync)
    conn_lost = 1;
    apa102_show_no_connection();

    //reset wdt once in order to have at least one second waiting for a packet:
    wdt_reset();

    //make sure we never read the same packet twice by crc flag
    frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE-1] = 0x00;
    conn_lost = 1;

    //start main loop
    while(1){
        if (timeout_timed_out()){
            //next hop in 9ms
            if (!conn_lost){
                timeout_set(9);
            }else{
                timeout_set(500);
            }

            //wdt reset
            wdt_reset();

            frsky_increment_channel(1);

            //diversity toggle on missing frame
            if (!packet_received){
                led_red_on();
                cc25xx_switch_antenna();
            }

            //go back to rx mode
            cc25xx_enable_receive();
            //cc25xx_enter_rxmode(); THIS BREAKS VD5M!
            cc25xx_strobe(RFST_SRX);

            //if enabled, send a sbus frame in case we lost that frame:
            if (!packet_received){
                //frame was lost, so there was no channel value update
                //and no transmission for the last frame slot.
                //therefore we will do a transmission now
                //(frame lost packet flag will be set)
                sbus_start_transmission(SBUS_FRAME_LOST);
            }

            //check for packets
            if (packet_received > 0){
                debug_putc('0' + cc25xx_get_current_antenna());
            }else{
                debug_putc('!');
                missing++;
            }
            packet_received = 0;

            if (hopcount++ >= FRSKY_COUNT_RXSTATS){
                uint16_t percent_ok;
                debug(" STATS: ");
                percent_ok = (((uint16_t)stat_rxcount) * 100) / 50;
                debug_put_uint8(percent_ok);
                debug_putc('%');
                debug_put_newline();

                //for testing
                //debug_put_uint16((uint16_t)(((frsky_packet_buffer[11] & 0x0F)<<8 | frsky_packet_buffer[8])));
                //debug_put_uint16(frsky_rssi);
                //debug_putc(' ');

                //link quality
                frsky_link_quality = stat_rxcount;

                if (stat_rxcount==0){
                    conn_lost = 1;
                    //enter failsafe mode
                    failsafe_enter();
                    debug("\nCONN LOST!\n");

                    //no connection led info
                    apa102_show_no_connection();
                }

                //statistics
                hopcount = 1;
                stat_rxcount = 0;
            }

            //led_red_off();

            //handle ovfs
            frsky_handle_overflows();
        }

        //process incoming data
        cc25xx_process_packet(&frsky_packet_received, (volatile uint8_t *)&frsky_packet_buffer, FRSKY_PACKET_BUFFER_SIZE);

        if (frsky_packet_received){
            led_red_off();

            //valid packet?
            if (FRSKY_VALID_PACKET(frsky_packet_buffer)){
                //ok, valid packet for us
                led_green_on();

                //we hop to the next channel in 0.5ms
                //afterwards hops are in 9ms grid again
                //this way we can have up to +/-1ms jitter on our 9ms timebase
                //without missing packets
                //500us delay:
                timeout2_delay_100us(5);

                //every 4th frame is a telemetry frame (transmits every 36ms)
                if ((frsky_packet_buffer[3] % 4) == 2){
                    //next frame is a telemetry frame
                    send_telemetry = 1;
                }

                //hal_timeout_delay_100us(5);
                if (send_telemetry){
                    timeout_set(9);
                }else{
                    timeout_set(0);
                }

                // start send timeout, in case we want to tx data,
                // this has to be done 2000us after rx
                // as we delayed the processing by 500us already, do the remaining 1500us
                timeout2_set_100us(15);

                //reset wdt
                wdt_reset();

                //reset missing packet counter
                missing = 0;

                //always store the last telemtry request id
                requested_telemetry_id   = frsky_packet_buffer[4];

                //stats
                stat_rxcount++;
                packet_received=1;
                conn_lost = 0;

                //extract rssi in frsky format
                frsky_rssi = frsky_extract_rssi(frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE-2]);

                //extract channel data:
                frsky_update_ppm();

                //debug_put_hex8(frsky_packet_buffer[3]);

                //make sure we never read the same packet twice by crc flag
                frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE-1] = 0x00;

                led_green_off();

                if (send_telemetry){
                    //change channel:
                    frsky_increment_channel(1);

                    while(!timeout2_timed_out()){
                        //wait for tx timeslot, do something useful here:
                        apa102_statemachine();
                    }

                   //build & send packet
                   frsky_send_telemetry(requested_telemetry_id);

                   //mark as done
                   send_telemetry = 0;
                }
            }
        }else{
            //invalid packet -> mark as not received
            frsky_packet_received = 0;
        }

        //process leds:
        apa102_statemachine();

    }
}



void frsky_set_channel(uint8_t hop_index){
    uint8_t ch = storage.frsky_hop_table[hop_index];
    //debug_putc('S'); debug_put_hex8(ch);

    //go to idle
    cc25xx_strobe(RFST_SIDLE);

    //fetch and set our stored pll calib data:
    cc25xx_set_register(FSCAL3, frsky_calib_fscal3);
    cc25xx_set_register(FSCAL2, frsky_calib_fscal2);
    cc25xx_set_register(FSCAL1, frsky_calib_fscal1_table[hop_index]);

    //set channel
    cc25xx_set_register(CHANNR, ch);
}



void frsky_increment_channel(int8_t cnt){
    int8_t next = frsky_current_ch_idx;
    //add increment
    next+=cnt;
    //convert to a safe unsigned number:
    if(next<0){
        next += FRSKY_HOPTABLE_SIZE;
    }
    if (next >= FRSKY_HOPTABLE_SIZE){
        next -= FRSKY_HOPTABLE_SIZE;
    }

    frsky_current_ch_idx = next;
    frsky_set_channel(frsky_current_ch_idx);
}


uint8_t frsky_extract_rssi(uint8_t rssi_raw){
#define FRSKY_RSSI_OFFSET 70
    if (rssi_raw >= 128){
        //adapted to fit better to the original values... FIXME: find real formula
        //return (rssi_raw * 18)/32 - 82;
        return ((((uint16_t)rssi_raw) * 18)>>5) - 82;
    }else{
        return ((((uint16_t)rssi_raw) * 18)>>5) + 65;
    }
}



void frsky_update_ppm(void){
    //build uint16_t array from data:
    EXTERNAL_MEMORY uint16_t channel_data[8];

    /*debug("[");debug_flush();
    for(cnt=0; cnt<FRSKY_PACKET_BUFFER_SIZE; cnt++){
        debug_put_hex8(frsky_packet_buffer[cnt]);
        debug_putc(' ');
        debug_flush();
    }
    debug("]\n"); debug_flush();
    */

    // extract channel data from packet:
    channel_data[0] = (uint16_t)(((frsky_packet_buffer[10] & 0x0F)<<8 | frsky_packet_buffer[6]));
    channel_data[1] = (uint16_t)(((frsky_packet_buffer[10] & 0xF0)<<4 | frsky_packet_buffer[7]));
    channel_data[2] = (uint16_t)(((frsky_packet_buffer[11] & 0x0F)<<8 | frsky_packet_buffer[8]));
    channel_data[3] = (uint16_t)(((frsky_packet_buffer[11] & 0xF0)<<4 | frsky_packet_buffer[9]));
    channel_data[4] = (uint16_t)(((frsky_packet_buffer[16] & 0x0F)<<8 | frsky_packet_buffer[12]));
    channel_data[5] = (uint16_t)(((frsky_packet_buffer[16] & 0xF0)<<4 | frsky_packet_buffer[13]));
    channel_data[6] = (uint16_t)(((frsky_packet_buffer[17] & 0x0F)<<8 | frsky_packet_buffer[14]));
    channel_data[7] = (uint16_t)(((frsky_packet_buffer[17] & 0xF0)<<4 | frsky_packet_buffer[15]));

    // set apa leds:
    apa102_update_leds(channel_data, frsky_link_quality);
    apa102_start_transmission();

    // exit failsafe mode
    failsafe_exit();

    // copy to output modules:
    sbus_update(channel_data);
    sbus_start_transmission(SBUS_FRAME_NOT_LOST);
    // and to ppm
    ppm_update(channel_data);

}




void frsky_send_telemetry(uint8_t telemetry_id){
    static uint8_t test = 0;

    //Stop RX DMA
    cc25xx_strobe(RFST_SFRX);

    //enable tx
    cc25xx_enter_txmode();

    //length of byte (always 0x11 = 17 bytes)
    frsky_packet_buffer[0] = 0x11;
    //txid
    frsky_packet_buffer[1] = storage.frsky_txid[0];
    frsky_packet_buffer[2] = storage.frsky_txid[1];
    //ADC channels
    frsky_packet_buffer[3] = adc_get_scaled(0);
    frsky_packet_buffer[4] = adc_get_scaled(1);
    //RSSI
    frsky_packet_buffer[5] = frsky_rssi;

    {
        uint8_t i;
        for(i=6; i<FRSKY_PACKET_BUFFER_SIZE; i++){
            frsky_packet_buffer[i] = 0;
        }
    }

    //append any received hub telemetry data
    telemetry_fill_buffer(&frsky_packet_buffer[6], telemetry_id);

    //re arm adc dma etc
    //it is important to call this after reading the values...
    adc_process();

    //arm dma channel
    cc25xx_transmit_packet(frsky_packet_buffer, FRSKY_PACKET_BUFFER_SIZE);

    //prepare for rx:
    cc25xx_setup_rf_dma(CC25XX_MODE_RX);
    cc25xx_enable_receive();
    cc25xx_strobe(RFST_SRX);
}


//useful for debugging/sniffing packets from anothe tx or rx
//make sure to bind this rx before using this...
void frsky_frame_sniffer(void){
    uint8_t send_telemetry = 0;
    uint8_t missing = 0;
    uint8_t hopcount = 0;
    uint8_t stat_rxcount = 0;
    uint8_t badrx_test = 0;
    uint8_t conn_lost = 1;
    uint8_t packet_received = 0;
    uint8_t i;

    debug("frsky: entering sniffer mode\n");

    //start with any channel:
    frsky_current_ch_idx = 0;
    //first set channel uses enter rxmode, this will set up dma etc
    frsky_enter_rxmode(storage.frsky_hop_table[frsky_current_ch_idx]);

    //wait 500ms on the current ch on powerup
    timeout_set(500);

    //start with conn lost (allow full sync)
    conn_lost = 1;

    //reset wdt once in order to have at least one second waiting for a packet:
    wdt_reset();

    //start main loop
    while(1){
        if (timeout_timed_out()){
            led_red_on();

            //next hop in 9ms
            if (!conn_lost){
                timeout_set(9);
            }else{
                timeout_set(500);
            }

            frsky_increment_channel(1);

            //strange delay
            //_delay_us(1000);
            cc25xx_rx_sleep();

            //go back to rx mode
            frsky_packet_received = 0;
            cc25xx_enable_receive();

            cc25xx_strobe(RFST_SRX);

            //check for packets
            if (!packet_received){
                if (send_telemetry){
                    debug("< MISSING\n");
                    send_telemetry = 0;
                }else{
                    debug("> MISSING\n");
                }
                send_telemetry = 0;
                missing++;
            }
            packet_received = 0;

            if (hopcount++ >= 100){
                if (stat_rxcount==0){
                    conn_lost = 1;
                    debug("\nCONN LOST!\n");
                }

                //statistics
                hopcount = 1;
                stat_rxcount = 0;
            }

            led_red_off();
        }

        //handle ovfs
        frsky_handle_overflows();

        //process incoming data
        cc25xx_process_packet(&frsky_packet_received, (volatile uint8_t *)&frsky_packet_buffer, FRSKY_PACKET_BUFFER_SIZE);


        if (frsky_packet_received){
            if (FRSKY_VALID_PACKET(frsky_packet_buffer)){
                //ok, valid packet for us
                led_green_on();

                //we hop to the next channel in 0.5ms
                //afterwards hops are in 9ms grid again
                //this way we can have up to +/-1ms jitter on our 9ms timebase
                //without missing packets
                delay_us(500);
                timeout_set(0);

                //dump all packets!
                if (send_telemetry){
                    debug("< ");
                    send_telemetry = 0;
                }else{
                    debug("> ");
                }

                for(i=0; i<FRSKY_PACKET_BUFFER_SIZE; i++){
                    debug_put_hex8(frsky_packet_buffer[i]);
                    debug_putc(' ');
                }
                debug("\n");

                //reset wdt
                wdt_reset();

                //reset missing packet counter
                missing = 0;

                //every 4th frame is a telemetry frame (transmits every 36ms)
                if ((frsky_packet_buffer[3] % 4) == 2){
                    send_telemetry = 1;
                }

                //stats
                stat_rxcount++;
                packet_received=1;
                conn_lost = 0;

                //make sure we never read the same packet twice by crc flag
                frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE-1] = 0x00;

                led_green_off();
            }
        }

    }
}


#if 0




uint8_t frsky_append_hub_data(uint8_t sensor_id, uint16_t value, uint8_t *buf){
    uint8_t index = 0;
    uint8_t val8;

    //add header:
    buf[index++] = FRSKY_HUB_TELEMETRY_HEADER;
    //add sensor id
    buf[index++] = sensor_id;
    //add data, take care of byte stuffing
    //low byte
    val8 = LO(value);
    if (val8 == 0x5E){
        buf[index++] = 0x5D;
        buf[index++] = 0x3E;
    }else if (val8 == 0x5D){
        buf[index++] = 0x5D;
        buf[index++] = 0x3D;
    }else{
        buf[index++] = val8;
    }
    //high byte
    val8 = HI(value);
    if (val8 == 0x5E){
        buf[index++] = 0x5D;
        buf[index++] = 0x3E;
    }else if (val8 == 0x5D){
        buf[index++] = 0x5D;
        buf[index++] = 0x3D;
    }else{
        buf[index++] = val8;
    }
    //add footer:
    buf[index] = FRSKY_HUB_TELEMETRY_HEADER;

    //return index:
    return index;
}
#endif
