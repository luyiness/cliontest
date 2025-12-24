//
// Created by Administrator on 2025-12-23.
//

#ifndef CLIONTEST_BTIM_H
#define CLIONTEST_BTIM_H

#include "../sys/sys.h"

extern TIM_HandleTypeDef g_timHandle;

void btim_init(uint32_t prescaler,uint32_t arr);

#endif //CLIONTEST_BTIM_H