#include "I2C0_hard.h"
#include "gd32f4xx_i2c.h"

void I2C0_hard_init() {
    // 硬实现：CPU操作I2C的寄存器，AF复用，初始化外设
    // GPIO -----------------------------------------
    GPIO_output_af(I2C0H_SCL_RCU, I2C0H_SCL_PORT, I2C0H_SCL_PIN, GPIO_OTYPE_OD, I2C0H_AF);
    GPIO_output_af(I2C0H_SDA_RCU, I2C0H_SDA_PORT, I2C0H_SDA_PIN, GPIO_OTYPE_OD, I2C0H_AF);

    // I2C  ------------------------------------------
    rcu_periph_clock_enable(RCU_I2C0);
    i2c_deinit(I2C0);
    i2c_clock_config(I2C0, I2C0H_SPEED, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

// 等待标记flag变为SET，成功return 0
static uint8_t I2C_wait(uint32_t flag) {
    uint32_t timeout = I2C0H_TIMEOUT;
    while(!i2c_flag_get(I2C0, flag)) {
        if(--timeout == 0) {
            return 1;
        }
    }
    return 0;
}

// 等待标记flag变为RESET，成功return 0
static uint8_t I2C_waitn(uint32_t flag) {
    uint32_t timeout = I2C0H_TIMEOUT;
    while(i2c_flag_get(I2C0, flag)) {
        if(--timeout == 0) {
            return 1;
        }
    }
    return 0;
}


/**********************************************************
 * @brief 写数据到I2C设备指定寄存器
 * @param  addr 设备地址 (写地址的前7位) 0x51
                写地址 (addr << 1) | 0     0xA2
 * @param  reg  寄存器地址
 * @param  dat  字节数组
 * @param  len  数据长度
 * @return 0 成功，>0为失败
 **********************************************************/
uint8_t I2C0_hard_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len) {
    if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;

    i2c_start_on_bus(I2C0);
    if(I2C_wait(I2C_FLAG_SBSEND)) return 2;

    i2c_master_addressing(I2C0, addr << 1, I2C_TRANSMITTER);
    if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    if(I2C_wait(I2C_FLAG_TBE)) return 4;
    i2c_data_transmit(I2C0, reg);

    for(uint32_t i = 0; i < len; i++) {
        if(I2C_wait(I2C_FLAG_TBE)) return 5;
        i2c_data_transmit(I2C0, dat[i]);
    }

    if(I2C_wait(I2C_FLAG_BTC)) return 6;
    i2c_stop_on_bus(I2C0);
    if(I2C_waitn(I2C_FLAG_I2CBSY)) return 7;

    return 0;
}

/**********************************************************
 * @brief 从I2C设备指定寄存器读取数据
 * @param  addr 设备地址  (写地址的前7位)   0x51
 * @param  reg  寄存器地址
 * @param  dat  字节数组
 * @param  len  数据长度
 * @return 0成功，>0为失败
 **********************************************************/
uint8_t I2C0_hard_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len) {
    if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;

    i2c_start_on_bus(I2C0);
    if(I2C_wait(I2C_FLAG_SBSEND)) return 2;

    i2c_master_addressing(I2C0, addr << 1, I2C_TRANSMITTER);
    if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    if(I2C_wait(I2C_FLAG_TBE)) return 4;
    i2c_data_transmit(I2C0, reg);
    if(I2C_wait(I2C_FLAG_BTC)) return 5;

    i2c_start_on_bus(I2C0);
    if(I2C_wait(I2C_FLAG_SBSEND)) return 6;

    i2c_master_addressing(I2C0, addr << 1 | 1, I2C_RECEIVER);
    if(I2C_wait(I2C_FLAG_ADDSEND)) return 7;
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    if(len == 1) {
        i2c_ack_config(I2C0, I2C_ACK_DISABLE);
    }

    for(uint32_t i = 0; i < len; i++) {
        if(i == len - 1) {
            i2c_ack_config(I2C0, I2C_ACK_DISABLE);
            i2c_stop_on_bus(I2C0);
        }
        if(I2C_wait(I2C_FLAG_RBNE)) return 8;
        dat[i] = i2c_data_receive(I2C0);
    }

    if(I2C_waitn(I2C_FLAG_I2CBSY)) return 9;
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return 0U;
}