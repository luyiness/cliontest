//
// Created by Administrator on 2026-01-06.
//

#ifndef CLIONTEST_DAC_H
#define CLIONTEST_DAC_H

#include "../sys/sys.h"
#include "../delay/delay.h"

extern DAC_HandleTypeDef g_dac_handle;
void dac_init();
void set_Voltage(uint16_t vol);
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n);

extern uint16_t g_dac_sin_buf[4096];
void dac_creat_sin_buf(uint16_t maxval, uint16_t samples);
void dac_dma_init();
void dac_dma_wave_enable(uint16_t cndtr, uint16_t arr, uint16_t psc);

#endif //CLIONTEST_DAC_H