#ifndef __FRSKY_H__
#define __FRSKY_H__

#include "main.h"
#include "cc2510fx.h"
#include "dma.h"

#define FRSKY_HOPTABLE_SIZE 47
//
//extern __xdata uint8_t frsky_txid[2];
//extern __xdata uint8_t frsky_hop_table[FRSKY_HOPTABLE_SIZE];
//extern __xdata int8_t frsky_freq_offset;
extern __xdata uint8_t frsky_current_ch_idx;
//rssi
extern __xdata uint8_t frsky_rssi;
extern __xdata uint8_t frsky_link_quality;
//pll calibration
extern __xdata uint8_t frsky_calib_fscal1_table[FRSKY_HOPTABLE_SIZE];
extern __xdata uint8_t frsky_calib_fscal2;
extern __xdata uint8_t frsky_calib_fscal3;
//extern __xdata int16_t frsky_freq_offset_acc;

#define FRSKY_PACKET_LENGTH 17
#define FRSKY_PACKET_BUFFER_SIZE (FRSKY_PACKET_LENGTH+3)
extern __xdata volatile uint8_t frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE];
extern __xdata volatile uint8_t frsky_packet_received;
extern __xdata volatile uint8_t frsky_packet_sent;
extern __xdata volatile uint8_t frsky_mode;

void frsky_init(void);
void frsky_configure(void);
void frsky_fetch_txid_and_hoptable(void);
void frsky_configure_address(void);
void frsky_calib_pll(void);
void frsky_rf_interrupt(void) __interrupt RF_VECTOR;
void frsky_handle_overflows(void);
void frsky_main(void);
void frsky_set_channel(uint8_t hop_index);
void frsky_update_ppm(void);
void frsky_increment_channel(int8_t cnt);
void frsky_setup_rf_dma(uint8_t);
uint8_t frsky_extract_rssi(uint8_t rssi_raw);
void frsky_enter_rxmode(uint8_t ch);

//binding
uint8_t frsky_bind_jumper_set(void);
void frsky_do_bind(void);
void frsky_store_config(void);
void frsky_send_telemetry(void);

#define FRSKY_MODE_RX 0
#define FRSKY_MODE_TX 1

//packet data example:
//BIND:   [11 03 01 16 68 14 7E BF 15 56 97 00 00 00 00 00 00 0B F8 AF ]
//NORMAL: [11 16 68 ... ]
#define FRSKY_VALID_FRAMELENGTH(_b) (_b[0] == 0x11)
#define FRSKY_VALID_CRC(_b)     (_b[19] & 0x80)
#define FRSKY_VALID_TXID(_b) ((_b[1] == storage.frsky_txid[0]) && (_b[2] == storage.frsky_txid[1]))
#define FRSKY_VALID_PACKET_BIND(_b) (FRSKY_VALID_FRAMELENGTH(_b) && FRSKY_VALID_CRC(_b) && (_b[2] == 0x01))
#define FRSKY_VALID_PACKET(_b)      (FRSKY_VALID_FRAMELENGTH(_b) && FRSKY_VALID_CRC(_b) && FRSKY_VALID_TXID(_b) )



#endif
