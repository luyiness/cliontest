//
// Created by Administrator on 2026-01-09.
//

#include "spi.h"

SPI_HandleTypeDef g_spi_handle;

void spi_init() {
    g_spi_handle.Instance = SPI2;
    g_spi_handle.Init.Mode = SPI_MODE_MASTER;   //stm32作为主机
    g_spi_handle.Init.Direction = SPI_DIRECTION_2LINES; //双线全双工
    g_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
    g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;  //工作模式11
    g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;       //工作模式11
    g_spi_handle.Init.NSS = SPI_NSS_SOFT;       //片选：软件
    g_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;    //波特率
    g_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
    g_spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_spi_handle.Init.CRCPolynomial = 7;    //（默认值）
    HAL_SPI_Init(&g_spi_handle);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef gpio_InitStruct;
        gpio_InitStruct.Mode = GPIO_MODE_AF_PP;     //注意引脚都设为复用推挽输出
        gpio_InitStruct.Pull = GPIO_PULLUP;
        gpio_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;      //CLK,MISO,MOSI
        HAL_GPIO_Init(GPIOB, &gpio_InitStruct);
    }
}

uint8_t spi_read_write_byte(uint8_t data) {     //data发送的数据
    uint8_t rec_data;   //接收的数据
    HAL_SPI_TransmitReceive(&g_spi_handle, &data, &rec_data, 1, 1000);
    return rec_data;
}