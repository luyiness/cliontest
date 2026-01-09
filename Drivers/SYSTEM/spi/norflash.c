//
// Created by Administrator on 2026-01-09.
//

#include "norflash.h"

void norflash_init() {
    //初始化NSS线
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio_InitStruct;
    gpio_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     //注意NSS线模式：OUTPUT_PP
    gpio_InitStruct.Pull = GPIO_PULLUP;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_InitStruct.Pin = GPIO_PIN_12;      //NSS
    HAL_GPIO_Init(GPIOB, &gpio_InitStruct);

    spi_init();

    spi_read_write_byte(0xFF);      //清空DR寄存器
    NORFLASH_CS(1); //拉高CS（空闲状态）
}

uint8_t norflash_read_byte(uint32_t address) {
    uint8_t data;

    NORFLASH_CS(0);     //1、拉低CS
    spi_read_write_byte(0x03);  //2、03H读命令
    spi_read_write_byte(address >> 16);     //3、分三次发送地址
    spi_read_write_byte(address >> 8);
    spi_read_write_byte(address);
    data = spi_read_write_byte(0xFF);       //4、读取数据
    NORFLASH_CS(1);     //5、拉高CS
    return data;
}

//读取SR1
uint32_t norflash_read_SR1() {
    NORFLASH_CS(0);
    spi_read_write_byte(0x05);
    uint8_t sr1 = spi_read_write_byte(0xFF);    //读SR1
    NORFLASH_CS(1);
    return sr1;
}

void norflash_erase_sector(uint32_t address) {
    NORFLASH_CS(0);
    spi_read_write_byte(0x06);  //1、写使能
    NORFLASH_CS(1);

    //2、等待 直到为空闲状态：看SR1的BUSY位=0
    while (norflash_read_SR1() & 0x01) {}

    NORFLASH_CS(0);
    spi_read_write_byte(0x20);  //3、20H
    spi_read_write_byte(address >> 16);     //4、分三次发送地址
    spi_read_write_byte(address >> 8);
    spi_read_write_byte(address);
    NORFLASH_CS(1);
    while (norflash_read_SR1() & 0x01) {}   //5、等待 直到为空闲状态
}

void norflash_write_page(uint32_t address, uint8_t data) {
    norflash_erase_sector(address);     //1、erase
    NORFLASH_CS(0);
    spi_read_write_byte(0x06);      //2、写使能
    NORFLASH_CS(1);

    NORFLASH_CS(0);
    spi_read_write_byte(0x02);      //3、写操作
    spi_read_write_byte(address >> 16);     //4、分三次发送地址
    spi_read_write_byte(address >> 8);
    spi_read_write_byte(address);
    spi_read_write_byte(data);      //5、写入数据
    NORFLASH_CS(1);

    while (norflash_read_SR1() & 0x01) {}   //6、等待空闲（等待写入完成）
}