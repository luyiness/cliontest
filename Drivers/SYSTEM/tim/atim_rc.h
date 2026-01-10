//
// Created by Administrator on 2025-12-25.
//

#ifndef CLIONTEST_ATIM_RC_H
#define CLIONTEST_ATIM_RC_H

#include "../sys/sys.h"

void atim_rc_init(uint32_t prescaler,uint32_t rlr);
void atim_timx_npwm_chy_set(uint8_t pwm_count);

#endif //CLIONTEST_ATIM_RC_H