#ifndef __MAIN_H__
#define __MAIN_H__

#include<stdint.h>

#define sei() { IEN0 |= IEN0_EA; }
#define cli() { IEN0 &= ~IEN0_EA; }

#define NOP() { __asm nop __endasm; }

#define HI(a)     (uint8_t) ((uint16_t)(a) >> 8 )
#define LO(a)     (uint8_t)  (uint16_t)(a)
#define SET_WORD(H, L, val) { (H) = HI(val); (L) = LO(val); }
//necessary for timer registers. todo: check if necessary for others as well...
#define SET_WORD_LO_FIRST(H, L, val) {(L) = LO(val); (H) = HI(val);  }


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
#define U1CSR_TX_BYTE (1<<1)

#define RFST_SNOP    0x05
#define RFST_SIDLE   0x04
#define RFST_STX     0x03
#define RFST_SRX     0x02
#define RFST_SCAL    0x01
#define RFST_SFSTXON 0x00

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

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


#endif
