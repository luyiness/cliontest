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

#endif //CLIONTEST_DAC_H