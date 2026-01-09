//
// Created by Administrator on 2026-01-09.
//

#ifndef CLIONTEST_NORFLASH_H
#define CLIONTEST_NORFLASH_H

#include "spi.h"

/* NORFLASH 片选信号 */
#define NORFLASH_CS(x)      do{ x ? \
                                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); \
                                }while(0)


void norflash_init();
uint8_t norflash_read_byte(uint32_t address);
uint32_t norflash_read_SR1();
void norflash_erase_sector(uint32_t address);
void norflash_write_page(uint32_t address, uint8_t data);

#endif //CLIONTEST_NORFLASH_H