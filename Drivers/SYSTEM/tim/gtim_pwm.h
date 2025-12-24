//
// Created by Administrator on 2025-12-24.
//

#ifndef CLIONTEST_GTIM_PWM_H
#define CLIONTEST_GTIM_PWM_H

#include "../sys/sys.h"

extern TIM_HandleTypeDef g_timx_pwm_chy_handle;

void gtim_pwm_init(uint32_t prescaler,uint32_t rlr);

#endif //CLIONTEST_GTIM_PWM_H