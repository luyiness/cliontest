//
// Created by Administrator on 2026-01-06.
//

#include <math.h>

#include "dac.h"

uint16_t g_dac_sin_buf[4096] = {0};

void dac_creat_sin_buf(uint16_t maxval, uint16_t samples) {
    float curval;
    if (maxval > 4095) return;
    if (samples > (maxval + 1)*2) return;
    float w = (2*3.141592) / samples;   //角频率w

    for (int i=0;i<samples;i++) {
        curval = maxval/2 * sin(w * i) + maxval/2;
        g_dac_sin_buf[i] = curval;
    }
}

DMA_HandleTypeDef g_dma_dac_handle;
uint8_t g_dac_dma_flag = 0;

void dac_dma_init() {       //DMA2通道3
    __HAL_RCC_DMA2_CLK_ENABLE();

    g_dma_dac_handle.Instance = DMA2_Channel3;
    g_dma_dac_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_dac_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_dac_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_dac_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_dma_dac_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    g_dma_dac_handle.Init.Mode = DMA_CIRCULAR;                   //Circular mode
    g_dma_dac_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&g_dma_dac_handle);

    g_dac_handle.DMA_Handle1 = &g_dma_dac_handle;
    __HAL_LINKDMA(&g_dac_handle, DMA_Handle1, g_dma_dac_handle);

    //启动DMA  ；我的理解是假启动，因为参数DataLength=0，若不=0就真启动运输了。 在函数dac_dma_wave_enable初始化完TIM7后才会真启动DMA
    HAL_DMA_Start_IT(&g_dma_dac_handle,(uint32_t)&g_dac_sin_buf,(uint32_t)&DAC1->DHR12R1,0);      //（这里面执行了__HAL_DMA_ENABLE）
}

//初始化TIM7，启动DMA传输和DAC转换
//参数： cndtr:DMA数据传输量DMA_CNDTRx， arr:TIM重装载值，psc: TIM分频系数
void dac_dma_wave_enable(uint16_t cndtr, uint16_t arr, uint16_t psc) {
    __HAL_RCC_TIM7_CLK_ENABLE();

    TIM_HandleTypeDef htim = {0};
    htim.Instance = TIM7;
    htim.Init.Period = arr;
    htim.Init.Prescaler = psc;
    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;    //MMS位=010，更新事件用作触发输出TRGO
    HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

    HAL_TIM_Base_Start(&htim);

    //停止/启动 DMA传输和DAC转换
    HAL_DAC_Stop_DMA(&g_dac_handle,DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&g_dac_handle, DAC_CHANNEL_1, (uint32_t *)g_dac_sin_buf, cndtr, DAC_ALIGN_12B_R);
        //此函数做了很多事：设置回调函数，DAC_CR_DMAEN1=1，设置DMA搬运目的地是Instance->DHR12R1，HAL_DMA_Start_IT，__HAL_DAC_ENABLE
}