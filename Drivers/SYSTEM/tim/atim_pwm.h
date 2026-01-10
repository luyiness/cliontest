//
// Created by Administrator on 2025-12-26.
//

#ifndef CLIONTEST_ATIM_PWM_H
#define CLIONTEST_ATIM_PWM_H

#include "../sys/sys.h"

extern TIM_BreakDeadTimeConfigTypeDef g_break_dead_time_config;

void atim_pwm_chy_init(uint32_t prescaler,uint32_t rlr);
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg);

#endif //CLIONTEST_ATIM_PWM_H