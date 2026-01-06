//
// Created by Administrator on 2026-01-06.
//

#ifndef CLIONTEST_DAC_H
#define CLIONTEST_DAC_H

#include "../sys/sys.h"

extern DAC_HandleTypeDef g_dac_handle;
void dac_init();
void set_Voltage(uint16_t vol);

#endif //CLIONTEST_DAC_H