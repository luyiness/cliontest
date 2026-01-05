//
// Created by Administrator on 2026-01-05.
//

#ifndef CLIONTEST_ADC1_H
#define CLIONTEST_ADC1_H

#include "../sys/sys.h"

extern ADC_HandleTypeDef g_adc_handle;
void adc1_init();
uint16_t getADCResult(void);
void adc_dma_init(uint32_t mar);    //初始化采集
extern uint8_t g_adc_dma_flag;  //标记DMA是否传输完成
void adc_dma_enable(uint16_t cndtr);    //启动采集

#endif //CLIONTEST_ADC1_H