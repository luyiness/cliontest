//
// Created by Administrator on 2025-12-31.
//

#ifndef CLIONTEST_DMA_H
#define CLIONTEST_DMA_H

#include "../sys/sys.h"
#include "../usart/usart2.h"

extern  DMA_HandleTypeDef g_dma_handle;
extern  uint8_t src_buf[10];
extern uint8_t dest_buf[10];

void dma_init();
void dma_enable_transmit(uint16_t cndtr);
void dma2_init();

#endif //CLIONTEST_DMA_H