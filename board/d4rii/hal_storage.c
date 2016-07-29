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
#include "hal_storage.h"
#include "delay.h"
#include "debug.h"
#include "timeout.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"

#define HAL_STORAGE_I2C_DEBUG 1

#define EEPROM_I2C_TIMEOUT      20
#define EEPROM_I2C_FLAG_TIMEOUT 10

void hal_storage_init(void) {
    hal_storage_i2c_init();
}

void hal_storage_write(uint8_t *buffer, uint16_t len){
    //verify write size
    if (!hal_storage_check_len(len)) return;

    //disable write protection
    hal_storage_wp_disable();
    delay_ms(1);

    //execute write
    if (!hal_storage_i2c_write_buffer(0, buffer, (uint8_t)len)){
        debug("hal_storage: ERROR! failed to write buffer\n");
        debug_flush();
    }

    //re-enable write protection
    delay_ms(1);
    hal_storage_wp_enable();
}


void hal_storage_read(uint8_t *storage_ptr, uint16_t len) {
    //verify read size
    if (!hal_storage_check_len(len)) return;

    if (!hal_storage_i2c_read_buffer(0, storage_ptr, (uint8_t)len)){
        debug("hal_storage: ERROR! failed to read buffer\n");
        debug_flush();
    }
}

static void hal_storage_i2c_init(void){
    //disable i2c:
    I2C_Cmd(EEPROM_I2C, DISABLE);
    I2C_DeInit(EEPROM_I2C);

    hal_storage_i2c_gpio_init();
    hal_storage_i2c_mode_init();
}


static void hal_storage_i2c_mode_init(void) {
    I2C_InitTypeDef  i2c_init;

    i2c_init.I2C_Mode         = I2C_Mode_I2C;
    i2c_init.I2C_DutyCycle    = I2C_DutyCycle_2;
    i2c_init.I2C_OwnAddress1  = EEPROM_I2C_ADDRESS;
    i2c_init.I2C_Ack          = I2C_Ack_Enable;
    i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2c_init.I2C_ClockSpeed   = 200000;

    //enable i2c
    I2C_Cmd(EEPROM_I2C, ENABLE);

    //apply I2C configuration after enabling it
    I2C_Init(EEPROM_I2C, &i2c_init);
}

static void hal_storage_i2c_gpio_init(void) {
    GPIO_InitTypeDef gpio_init;

    //clock disable
    RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, DISABLE);

    //gpio init:
    //SDA & SCL
    gpio_init.GPIO_Pin   = EEPROM_I2C_SDA_PIN | EEPROM_I2C_SCL_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_OD;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EEPROM_GPIO, &gpio_init);

    //WP = write protection
    gpio_init.GPIO_Pin  = EEPROM_WP_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EEPROM_GPIO, &gpio_init);

    //WP = HI = protected
    hal_storage_wp_enable();
}

static uint32_t hal_storage_check_len(uint16_t len) {
    if (len>255){
        debug("hal_storage: ERROR, invalid data len ");
        debug_put_uint16(len);
        debug(" (max is 255)!\n");
        debug_flush();
        //invalid
        return 0;
    }else{
        //safe
        return 1;
    }
}

static uint32_t hal_storage_i2c_read_buffer(uint16_t address, uint8_t *buffer, uint8_t len){
    if (HAL_STORAGE_I2C_DEBUG){
        debug("hal_storage: i2c read_buffer(0x");
        debug_put_hex8(address>>8);
        debug_put_hex8(address&0xFF);
        debug(", ..., ");
        debug_put_uint16(len);
        debug(")\n");
        debug_flush();
    }

    timeout_set(EEPROM_I2C_TIMEOUT);
    while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY)){
        if (timeout_timed_out()) {
            debug("hal_i2c: bus busy... timeout!\n");
            return 0;
        }
    }

    //send start
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);

    // set on EV5 and clear it (cleared by reading SR1 then writing to DR)
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)){
        if (timeout_timed_out()) {
            debug("hal_i2c: master flag error... timeout!\n");
            return 0;
        }
    }

    // send EEPROM address for write
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Transmitter);

    // test on EV6 and clear it
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if (timeout_timed_out()) {
            debug("hal_i2c: transmitter flag error... timeout!\n");
            return 0;
        }
    }

    //send the EEPROM's internal address to read from: Only one byte address
    I2C_SendData(EEPROM_I2C, address);

    //test on EV8 and clear it
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BTF) == RESET){
        if (timeout_timed_out()) {
            debug("hal_i2c: btf flag error... timeout!\n");
            return 0;
        }
    }


    // send START condition a second time
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);

    // set on EV5 and clear it (cleared by reading SR1 then writing to DR)
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)){
        if (timeout_timed_out()) {
            debug("hal_i2c: master flag error... timeout!\n");
            return 0;
        }
    }

    // send address (READ)
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Receiver);

    if (HAL_STORAGE_I2C_DEBUG){
        debug("hal_storage: reading ");
        debug_put_uint8(len);
        debug("bytes: ");
        debug_flush();
    }

    //do not use dma etc, we do not need highspeed. do polling:
    uint16_t i;
    for(i=0; i<len; i++){

        // wait on ADDR flag to be set (ADDR is still not cleared at this level)
        timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

        while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR) == RESET){
            if (timeout_timed_out()) {
                debug("hal_i2c: addr flag error... timeout!\n");
                return 0;
            }
        }

        if (i == (len-1)){
            // last byte? -> NACK
            I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);

            // stop transmission
            // clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
            (void)EEPROM_I2C->SR2;

            // send STOP Condition
            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
        }else{
            // more bytes -> ACK
            I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
        }

        // wait for read to finish
        timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

        while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_RXNE) == RESET){
            if (timeout_timed_out()) {
                debug("hal_i2c: read error... timeout!\n");
                return 0;
            }
        }

        // read byte received:
        buffer[i] = I2C_ReceiveData(EEPROM_I2C);

        if (HAL_STORAGE_I2C_DEBUG){
            debug_put_hex8(buffer[i]);
            debug_putc(' ');
        }
    }

    if (HAL_STORAGE_I2C_DEBUG){
        debug(". done.\n");
        debug_flush();
    }

    // wait to make sure that STOP control bit has been cleared
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(EEPROM_I2C->CR1 & I2C_CR1_STOP){
        if (timeout_timed_out()) {
            debug("hal_i2c: stop flag error... timeout!\n");
            return 0;
        }
    }

    // re-enable Acknowledgement to be ready for another reception
    I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);

    if (HAL_STORAGE_I2C_DEBUG){
        debug("hal_storage: read done\n");
        debug_flush();
    }

    return 1;
}

static uint32_t hal_storage_i2c_write_buffer(uint8_t address, uint8_t *buffer, uint8_t len){
    if (HAL_STORAGE_I2C_DEBUG){
        debug("hal_storage: i2c write_buffer(0x");
        debug_put_hex8(address>>8);
        debug_put_hex8(address&0xFF);
        debug(", ..., ");
        debug_put_uint16(len);
        debug(")\n");
        debug_flush();
    }

    uint8_t i;

    // check for out of bound condition
    uint16_t last_byte = (uint16_t)address + (uint16_t) len;
    if (last_byte > 255){
        debug("hal_storage: ERROR write request invalid. out of memory!\n");
        debug_flush();
        return 0;
    }

    // write data
    for (i = 0; i<len; i++){
        if (!hal_storage_i2c_write_byte(address + i, buffer[i])){
            return 0;
        }
    }

    return 1;
}

// single byte write is slow and ugly but will do
// we only use this once during binding...
static uint32_t hal_storage_i2c_write_byte(uint8_t address, uint8_t data){
    if (HAL_STORAGE_I2C_DEBUG){
        debug("hal_storage: i2c write_byte(0x");
        debug_put_hex8(address>>8);
        debug_put_hex8(address&0xFF);
        debug(", 0x");
        debug_put_hex8(data);
        debug(")\n");
        debug_flush();
    }

    timeout_set(EEPROM_I2C_TIMEOUT);
    while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY)){
        if (timeout_timed_out()) {
            debug("hal_i2c: bus busy... timeout!\n");
            return 0;
        }
    }

    //send start
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);

    // set on EV5 and clear it (cleared by reading SR1 then writing to DR)
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)){
        if (timeout_timed_out()) {
            debug("hal_i2c: master flag error... timeout!\n");
            return 0;
        }
    }

    // send EEPROM address for write
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Transmitter);

    // test on EV6 and clear it
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if (timeout_timed_out()) {
            debug("hal_i2c: transmitter flag error... timeout!\n");
            return 0;
        }
    }

    //send the EEPROM's internal address to write to
    I2C_SendData(EEPROM_I2C, address);

    //test on EV8 and clear it
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BTF) == RESET){
        if (timeout_timed_out()) {
            debug("hal_i2c: btf flag error... timeout!\n");
            return 0;
        }
    }

    //send data byte
    I2C_SendData(EEPROM_I2C, data);

    //test on EV8 and clear it
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(!I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BTF) == RESET){
        if (timeout_timed_out()) {
            debug("hal_i2c: btf flag error... timeout!\n");
            return 0;
        }
    }

    // stop transmission
    // clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)EEPROM_I2C->SR2;

    // send STOP Condition
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

    // wait to make sure that STOP control bit has been cleared
    timeout_set(EEPROM_I2C_FLAG_TIMEOUT);

    while(EEPROM_I2C->CR1 & I2C_CR1_STOP){
        if (timeout_timed_out()) {
            debug("hal_i2c: stop flag error... timeout!\n");
            return 0;
        }
    }

    //wait for write cycle time (5ms)
    delay_ms(5+1);

    if (HAL_STORAGE_I2C_DEBUG){
        debug("hal_storage: read done\n");
        debug_flush();
    }

    return 1;
}


