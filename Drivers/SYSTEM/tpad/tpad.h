//
// Created by Administrator on 2025-12-26.
//

#ifndef CLIONTEST_TPAD_H
#define CLIONTEST_TPAD_H

#include "../sys/sys.h"

// static TIM_HandleTypeDef g_timx_ic_chy_handle;
// static uint8_t g_timxchy_cap_sta;
// static uint16_t g_timxchy_cap_val;
// static uint8_t g_timxchy_cap_spill;    //溢出信号，因为没考虑计数器在测量过程中溢出；

void gtim_ic_init(uint32_t prescaler, uint32_t rlr);
void tpad_init();
uint16_t tpad_get_val(void);

#endif //CLIONTEST_TPAD_H