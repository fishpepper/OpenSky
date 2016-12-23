#ifndef __HAL_CC25XX_H__
#define __HAL_CC25XX_H__
#include <stdint.h>
#include "hal_defines.h"
#include "config.h"
#include <cc2510fx.h>


#define CC25XX_FIFO FIFO

#define hal_cc25xx_set_register(reg, val) { reg = val; }
#define hal_cc25xx_strobe(val) { RFST = val; }
#define hal_cc25xx_get_register(r) (r)
#define hal_cc25xx_get_register_burst(r) (r)


#ifdef RF_LNA_PORT
  #define RF_LNA_ENABLE()  { PORT2BIT(RF_LNA_PORT, RF_LNA_PIN) = RF_LNA_ON_LEVEL; }
  #define RF_LNA_DISABLE() { PORT2BIT(RF_LNA_PORT, RF_LNA_PIN) = ~RF_LNA_ON_LEVEL; }
  #define RF_PA_ENABLE()   { PORT2BIT(RF_PA_PORT, RF_PA_PIN) = RF_PA_ON_LEVEL; }
  #define RF_PA_DISABLE()  { PORT2BIT(RF_PA_PORT, RF_PA_PIN) = ~RF_PA_ON_LEVEL; }
#endif

#ifdef RF_HIGH_GAIN_MODE_PORT
  #define RF_HIGH_GAIN_MODE_ENABLE()  { PORT2BIT(RF_HIGH_GAIN_MODE_PORT, RF_HIGH_GAIN_MODE_PIN) = RF_HIGH_GAIN_MODE_ON_LEVEL; }
  #define RF_HIGH_GAIN_MODE_DISBALE() { PORT2BIT(RF_HIGH_GAIN_MODE_PORT, RF_HIGH_GAIN_MODE_PIN) = ~RF_HIGH_GAIN_MODE_ON_LEVEL; }
#endif

#define hal_cc25xx_set_antenna(x) (0)
#define hal_cc25xx_process_packet(packet_received, buffer, maxlen) {}

void hal_cc25xx_init(void);
#define hal_cc25xx_set_gdo_mode() {}
void hal_cc25xx_disable_rf_interrupt(void);

#define hal_cc25xx_rx_sleep() { delay_us(1000); }
#define hal_cc25xx_tx_sleep() { delay_us( 900); }

void hal_cc25xx_enter_rxmode(void);
void hal_cc25xx_enter_txmode(void);
void hal_cc25xx_setup_rf_dma(uint8_t mode);
void hal_cc25xx_enable_receive(void);
void hal_cc25xx_transmit_packet(volatile uint8_t *buffer, uint8_t len);
uint8_t hal_cc25xx_transmission_completed(void);

void hal_cc25xx_rf_interrupt(void) __interrupt RF_VECTOR;

#define hal_cc25xx_partnum_valid(p, v) ((p == 0x81) && (v = 0x04))

#define PERCFG_U0CFG (1<<0)
#define PERCFG_U1CFG (1<<1)
#define PERCFG_T4CFG (1<<4)
#define PERCFG_T3CFG (1<<5)
#define PERCFG_T1CFG (1<<6)

#define IEN0_RFTXRXIE (1<<0)
#define IEN0_ADCIE (1<<1)
#define IEN0_URX0IE (1<<2)
#define IEN0_URX1IE (1<<3)
#define IEN0_ENCIE (1<<4)
#define IEN0_STIE (1<<5)
#define IEN0_EA (1<<7)

//bit 7 - unused
//bit 6 - unused
#define PICTL_P2IEN  (1<<5)
#define PICTL_P0IENH (1<<4)
#define PICTL_P0IENL (1<<3)
#define PICTL_P2ICON (1<<2)
#define PICTL_P1ICON (1<<1)
#define PICTL_P0ICON (1<<0)

#define IEN1_P0IE  (1<<5)
#define IEN1_T4IE  (1<<4)
#define IEN1_T3IE  (1<<3)
#define IEN1_T2IE  (1<<2)
#define IEN1_T1IE  (1<<1)
#define IEN1_DMAIE (1<<0)

#define IEN2_RFIE (1<<0)
#define IEN2_P2IE (1<<1)
#define IEN2_UTX0IE (1<<2)
#define IEN2_UTX1IE (1<<3)
#define IEN2_P1IE   (1<<4)
#define IEN2_WDTIE  (1<<5)

#define U0GCR_ORDER (1<<5)
#define U0GCR_CPHA  (1<<6)
#define U0GCR_CPOL  (1<<7)
#define U0CSR_TX_BYTE (1<<1)

#define U1GCR_ORDER (1<<5)
#define U1GCR_CPHA  (1<<6)
#define U1GCR_CPOL  (1<<7)

#define UxCSR_RX_ENABLE (1<<6)
#define UxCSR_RX_BYTE (1<<2)
#define UxCSR_TX_BYTE (1<<1)


#define RFST_SNOP    0x05
#define RFST_SIDLE   0x04
#define RFST_STX     0x03
#define RFST_SRX     0x02
#define RFST_SCAL    0x01
#define RFST_SFSTXON 0x00
// statemachine on cc2510 is different.
// instead of SF*X we should use SIDLE
#define RFST_SFTX    RFST_SIDLE
#define RFST_SFRX    RFST_SIDLE

//append status
#define CC2500_PKTCTRL1_APPEND_STATUS     (1<<2)
//crc autoflush
#define CC2500_PKTCTRL1_CRC_AUTOFLUSH     (1<<3)
//adress checks
#define CC2500_PKTCTRL1_FLAG_ADR_CHECK_00 ((0<<1) | (0<<0))
#define CC2500_PKTCTRL1_FLAG_ADR_CHECK_01 ((0<<1) | (1<<0))
#define CC2500_PKTCTRL1_FLAG_ADR_CHECK_10 ((1<<1) | (0<<0))
#define CC2500_PKTCTRL1_FLAG_ADR_CHECK_11 ((1<<1) | (1<<0))


#define CLKCON_TICKSPD_001 (0b00001000)
#define CLKCON_TICKSPD_010 (0b00010000)
#define CLKCON_TICKSPD_011 (0b00011000)
#define CLKCON_TICKSPD_100 (0b00100000)
#define CLKCON_TICKSPD_101 (0b00101000)
#define CLKCON_TICKSPD_110 (0b00110000)
#define CLKCON_TICKSPD_111 (0b00111000)
#define CLKCON_OSC32K (1<<7)

#define ADCCON2_SREF_INT  (0b00<<6)
#define ADCCON2_SREF_EXT  (0b01<<6)
#define ADCCON2_SREF_AVDD (0b10<<6)
#define ADCCON2_SREF_EXTDIFF  (0b11<<6)
#define ADCCON2_SDIV_7BIT   (0b00<<4)
#define ADCCON2_SDIV_9BIT   (0b01<<4)
#define ADCCON2_SDIV_10BIT  (0b10<<4)
#define ADCCON2_SDIV_12BIT  (0b11<<4)
#define ADCCON2_SCH_AIN0   (0b0000<<0)
#define ADCCON2_SCH_AIN1   (0b0001<<0)
#define ADCCON2_SCH_AIN2   (0b0010<<0)
#define ADCCON2_SCH_AIN3   (0b0011<<0)
#define ADCCON2_SCH_AIN4   (0b0100<<0)
#define ADCCON2_SCH_AIN5   (0b0101<<0)
#define ADCCON2_SCH_AIN6   (0b0110<<0)
#define ADCCON2_SCH_AIN7   (0b0111<<0)
#define ADCCON2_SCH_AIN0AIN1   (0b1000<<0)
#define ADCCON2_SCH_AIN2AIN3   (0b1001<<0)
#define ADCCON2_SCH_AIN4AIN5   (0b1010<<0)
#define ADCCON2_SCH_AIN6AIN7   (0b1011<<0)
#define ADCCON2_SCH_GND        (0b1100<<0)
#define ADCCON2_SCH_POSVREF    (0b1101<<0)
#define ADCCON2_SCH_TEMP       (0b1110<<0)
#define ADCCON2_SCH_VDD3       (0b1111<<0)

#define ADCCON1_ST               (1<<6)
#define ADCCON1_STSEL_FULL_SPEED (0b01<<4)

#define WDCTL_EN (1<<3)
#define WDCTL_MODE (1<<2)
#define WDCTL_INT (0b11)
#define WDCTL_INT_1S (0b00)

#define FCTL_BUSY (1<<7)
#define FCTL_SWBUSY (1<<6)
#define FCTL_WRITE (1<<1)
#define FCTL_ERASE (1<<0)


#define T1CTL_MODE_SUSPEND (0b00<<0)
#define T1CTL_MODE_FREE_RUNNING (0b01<<0)
#define T1CTL_MODE_MODULO  (0b10<<0)
#define T1CTL_MODE_UPDOWN  (0b11<<0)
#define T1CTL_DIV_1     (0b00<<2)
#define T1CTL_DIV_8     (0b01<<2)
#define T1CTL_DIV_32    (0b10<<2)
#define T1CTL_DIV_128   (0b11<<2)
#define T1CTL_OVFIF     (1<<4)
#define T1CTL_CH0_IF    (1<<5)
#define T1CTL_CH1_IF    (1<<6)
#define T1CTL_CH2_IF    (1<<7)



#define T3CTL_MODE_SUSPEND (0b00<<0)
#define T3CTL_MODE_FREE_RUNNING (0b01<<0)
#define T3CTL_MODE_MODULO  (0b10<<0)
#define T3CTL_MODE_UPDOWN  (0b11<<0)
#define T3CTL_CLR          (1<<2)
#define T3CTL_OVFIM        (1<<3)
#define T3CTL_START        (1<<4)
#define T3CTL_DIV_1        (0b000<<5)
#define T3CTL_DIV_2        (0b001<<5)
#define T3CTL_DIV_4        (0b010<<5)
#define T3CTL_DIV_8        (0b011<<5)
#define T3CTL_DIV_16       (0b100<<5)
#define T3CTL_DIV_32       (0b101<<5)
#define T3CTL_DIV_64       (0b110<<5)
#define T3CTL_DIV_128      (0b111<<5)


#define T4CTL_MODE_SUSPEND (0b00<<0)
#define T4CTL_MODE_FREE_RUNNING (0b01<<0)
#define T4CTL_MODE_MODULO  (0b10<<0)
#define T4CTL_MODE_UPDOWN  (0b11<<0)
#define T4CTL_CLR          (1<<2)
#define T4CTL_OVFIM        (1<<3)
#define T4CTL_START        (1<<4)
#define T4CTL_DIV_1        (0b000<<5)
#define T4CTL_DIV_2        (0b001<<5)
#define T4CTL_DIV_4        (0b010<<5)
#define T4CTL_DIV_8        (0b011<<5)
#define T4CTL_DIV_16       (0b100<<5)
#define T4CTL_DIV_32       (0b101<<5)
#define T4CTL_DIV_64       (0b110<<5)
#define T4CTL_DIV_128      (0b111<<5)


#define T1CCTLx_CAP_NO       (0b00<<0)
#define T1CCTLx_CAP_RISING   (0b01<<0)
#define T1CCTLx_CAP_FALLING  (0b10<<0)
#define T1CCTLx_CAP_BOTH     (0b11<<0)
#define T1CCTLx_MODE_CAPTURE (0<<2)
#define T1CCTLx_MODE_COMPARE (1<<2)
#define T1CCTLx_CMP_SET      (0b000<<3)
#define T1CCTLx_CMP_CLEAR    (0b001<<3)
#define T1CCTLx_CMP_TOGGLE   (0b010<<3)
#define T1CCTLx_CMP_SETCLR0  (0b011<<3)
#define T1CCTLx_CMP_CLRSET0  (0b100<<3)
#define T1CCTLx_CMP_RES0     (0b101<<3)
#define T1CCTLx_CMP_RES1     (0b110<<3)
#define T1CCTLx_CMP_RES2     (0b111<<3)
#define T1CCTLx_IM           (1<<6)
#define T1CCTLx_CPSEL_RF     (1<<7)


//add missing defines
#include <compiler.h>
SFRX(TEST2,  0xDF23);
SFRX(TEST1,  0xDF24);
SFRX(TEST0,  0xDF25);


#endif // __HAL_CC25XX_H__
