//
// Created by Administrator on 2026-01-05.
//

#include "adc1.h"

DMA_HandleTypeDef g_dma_adc_handle;
uint8_t g_adc_dma_flag = 0;

void adc_dma_init(uint32_t mar) {
    __HAL_RCC_DMA1_CLK_ENABLE();

    g_dma_adc_handle.Instance = DMA1_Channel1;
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;  //从DR读，故不需自增
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;      //写内存地址，自增
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    //一次16位
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       //一次16位；注意易错DMA_MDATAALIGN_HALFWORD
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;    //Normal mode or Circular mode
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;      //Priority level
    HAL_DMA_Init(&g_dma_adc_handle);

    g_adc_handle.DMA_Handle = &g_dma_adc_handle;
    __HAL_LINKDMA(&g_adc_handle, DMA_Handle, g_dma_adc_handle);

    //DMA中断
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    //启动DMA
    HAL_DMA_Start_IT(&g_dma_adc_handle,(uint32_t)&ADC1->DR,mar,0);      //注意这个(uint32_t)强转

    //触发ADC转换、DMA传输数据
    HAL_ADC_Start_DMA(&g_adc_handle, &mar, 0);
}

//DMA传输完成后进入此中断
void DMA1_Channel1_IRQHandler(void) {
    if (DMA1->ISR & (1 << 1)) {     //判断DMA1的ISR寄存器的位1：TCIF1是否=1（是否传输完成）
        g_adc_dma_flag = 1;
        DMA1->IFCR |= 1 << 1;   //清除上一行的中断标志
    }
}

//启动DMA、ADC : (用寄存器)
 void adc_dma_enable(uint16_t cndtr) {
    ADC1->CR2 &= ~(1<<0);   //CR2的ADON置0，关闭ADON位触发转换

    DMA1_Channel1->CCR &= ~(1<<0);      //关闭DMA通道，EN位
    while(DMA1_Channel1->CCR & (1<<0)){};  //确保DMA通道关闭了
    DMA1_Channel1->CNDTR = cndtr;       //初始化CNDTR
    DMA1_Channel1->CCR |= (1<<0);      //开启DMA通道；HAL_DMA_Start_IT()P396有做这个

    ADC1->CR2 |= 1<<0;      //CR2的ADON置1
    ADC1->CR2 |= 1<<22;     //SWSTART置1；HAL_ADC_Start_DMA()P1655有做这个
 }
