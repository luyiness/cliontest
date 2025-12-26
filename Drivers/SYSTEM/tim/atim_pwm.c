//
// Created by Administrator on 2025-12-26.
//

#include "atim_pwm.h"

TIM_HandleTypeDef g_atimx_pwm_chy_handle;
TIM_BreakDeadTimeConfigTypeDef g_break_dead_time_config;

void atim_pwm_chy_init(uint32_t prescaler,uint32_t rlr) {
    g_atimx_pwm_chy_handle.Instance = TIM1;
    g_atimx_pwm_chy_handle.Init.Prescaler = prescaler;
    g_atimx_pwm_chy_handle.Init.Period = rlr;
    g_atimx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_atimx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;     //CKD=10，4分频
    HAL_TIM_PWM_Init(&g_atimx_pwm_chy_handle);

    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
    // timx_oc_pwm_chy.Pulse = rlr / 2; //后面设置
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;       //高有效
    timx_oc_pwm_chy.OCNPolarity = TIM_OCPOLARITY_HIGH;      //高有效
    timx_oc_pwm_chy.OCIdleState = TIM_OCIDLESTATE_RESET;    //当MOE=0，输出OCx=0
    timx_oc_pwm_chy.OCNIdleState = TIM_OCIDLESTATE_RESET;   //当MOE=0，输出OCxN=0
    HAL_TIM_PWM_ConfigChannel(&g_atimx_pwm_chy_handle,&timx_oc_pwm_chy,TIM_CHANNEL_1);

    g_break_dead_time_config.OffStateRunMode = TIM_OSSR_DISABLE;   //OSSR
    g_break_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;  //OSSI
    g_break_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;
    // break_dead_time_config.DeadTime = ;        //dead-time ；后面设置
    g_break_dead_time_config.BreakState = TIM_BREAK_ENABLE;      //BKE
    g_break_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;  //BKP
    g_break_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;    //AOE
    HAL_TIMEx_ConfigBreakDeadTime(&g_atimx_pwm_chy_handle,&g_break_dead_time_config);

    HAL_TIM_PWM_Start(&g_atimx_pwm_chy_handle,TIM_CHANNEL_1);       //OCx使能
    HAL_TIMEx_PWMN_Start(&g_atimx_pwm_chy_handle,TIM_CHANNEL_1);    //OCxN使能
}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        //1、IO口输出初始化
        __HAL_RCC_GPIOE_CLK_ENABLE();
        GPIO_InitTypeDef pe8 = {GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};
        GPIO_InitTypeDef pe9 = {GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};
        GPIO_InitTypeDef pe15 = {GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};
        HAL_GPIO_Init(GPIOE, &pe8);
        HAL_GPIO_Init(GPIOE, &pe9);
        HAL_GPIO_Init(GPIOE, &pe15);
        //2、开启重映射
        __HAL_RCC_AFIO_CLK_ENABLE();    //开启AFIO时钟
        __HAL_AFIO_REMAP_TIM1_ENABLE();

        //初始化TIM1时钟
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
}

//设置比较值，设置dead-time
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg) {
    if (ccr) {  //ccr!=0时
        __HAL_TIM_SET_COMPARE(&g_atimx_pwm_chy_handle,TIM_CHANNEL_1,ccr);
    }
    //我的代码：令DTG=0 不测这个了；
    g_break_dead_time_config.DeadTime = 0;
    if (dtg) {  //dtg!=0时
        g_break_dead_time_config.DeadTime = dtg;
        HAL_TIMEx_ConfigBreakDeadTime(&g_atimx_pwm_chy_handle,&g_break_dead_time_config);
    }
}