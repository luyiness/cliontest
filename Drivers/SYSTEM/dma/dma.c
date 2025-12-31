//
// Created by Administrator on 2025-12-31.
//

#include "dma.h"

DMA_HandleTypeDef g_dma_handle = {0};
uint8_t src_buf[10]={0};
uint8_t dest_buf[10]={0};

void dma_init() {
    __HAL_RCC_DMA1_CLK_ENABLE();

    g_dma_handle.Instance = DMA1_Channel1;
    g_dma_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;   //MEMORY_TO_MEMORY
    g_dma_handle.Init.PeriphInc = DMA_PINC_ENABLE;   //increment
    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;   //increment
    g_dma_handle.Init.PeriphDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle.Init.Mode = DMA_NORMAL;    //是否circle；注意MEMORY_TO_MEMORY不支持circle模式
    g_dma_handle.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&g_dma_handle);

    HAL_DMA_Start_IT(&g_dma_handle, (uint32_t)src_buf, (uint32_t)dest_buf, 0);
}

void dma_enable_transmit(uint16_t cndtr) {      //传输数据量cndtr
    __HAL_DMA_DISABLE(&g_dma_handle);
    DMA1_Channel1->CNDTR = cndtr;
    __HAL_DMA_ENABLE(&g_dma_handle);
}

//M to P模式
void dma2_init() {
    __HAL_RCC_DMA1_CLK_ENABLE();

    __HAL_LINKDMA(&g_huart, hdmatx, g_dma_handle);           /* 将DMA与USART1联系起来(发送DMA) */
    g_dma_handle.Instance = DMA1_Channel4;
    g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;             /* DIR = 1 , 存储器到外设模式 */
    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;                 /* 外设非增量模式 */
    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                     /* 存储器增量模式 */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle.Init.Mode = DMA_NORMAL;
    g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&g_dma_handle);
}