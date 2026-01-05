//
// Created by Administrator on 2026-01-05.
//

#ifndef CLIONTEST_ADC1_H
#define CLIONTEST_ADC1_H

#include "../sys/sys.h"

extern ADC_HandleTypeDef g_adc_handle;
void adc1_init();
uint16_t getADCResult(void);

#endif //CLIONTEST_ADC1_H