//
// Created by Administrator on 2026-01-09.
//

#ifndef CLIONTEST_SPI_H
#define CLIONTEST_SPI_H

#include "../sys/sys.h"

extern SPI_HandleTypeDef g_spi_handle;

void spi_init();
uint8_t spi_read_write_byte(uint8_t data);

#endif //CLIONTEST_SPI_H