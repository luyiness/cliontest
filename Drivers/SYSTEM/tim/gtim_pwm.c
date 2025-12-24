//
// Created by Administrator on 2025-12-24.
//

#include "gtim_pwm.h"

TIM_HandleTypeDef g_timx_pwm_chy_handle;

void gtim_pwm_init(uint32_t prescaler,uint32_t rlr) {
    g_timx_pwm_chy_handle.Instance = TIM3;
    g_timx_pwm_chy_handle.Init.Prescaler = prescaler;
    g_timx_pwm_chy_handle.Init.Period = rlr;
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    //递增/递减/中心对齐
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);

    TIM_OC_InitTypeDef timx_oc_pwm_chy;
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
    timx_oc_pwm_chy.Pulse = rlr / 2;      //CCRx 比较值
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;    //输出极性：低电平有效
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle,&timx_oc_pwm_chy,TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle,TIM_CHANNEL_2);
}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
        //初始化PB5
        //1、IO口输出初始化
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        //2、开启重映射
        __HAL_AFIO_REMAP_TIM3_PARTIAL();

        //初始化TIM3时钟
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
}