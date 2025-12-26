//
// Created by Administrator on 2025-12-26.
//

#ifndef CLIONTEST_ATIM_PWM_SURVEY_H
#define CLIONTEST_ATIM_PWM_SURVEY_H

#include "../sys/sys.h"

void atim_pwm_survey_init();

extern uint16_t g_timxchy_pwmin_sta;    /* PWM输入状态 */
extern uint32_t g_timxchy_pwmin_hval;   /* PWM的高电平脉宽, CCR2 */
extern uint32_t g_timxchy_pwmin_cval;   /* PWM的周期宽度, CCR1 */

void atim_timx_pwmin_chy_restart(void);

#endif //CLIONTEST_ATIM_PWM_SURVEY_H