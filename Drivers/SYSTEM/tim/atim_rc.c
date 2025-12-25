//
// Created by Administrator on 2025-12-25.
//

#include "atim_rc.h"

TIM_HandleTypeDef g_timx_rc_chy_handle;

void atim_rc_init(uint32_t prescaler,uint32_t rlr) {
    g_timx_rc_chy_handle.Instance = TIM8;       //TIM8_CH1，是PC6的复用
    g_timx_rc_chy_handle.Init.Prescaler = prescaler;
    g_timx_rc_chy_handle.Init.Period = rlr;
    g_timx_rc_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_rc_chy_handle.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&g_timx_rc_chy_handle);

    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
    timx_oc_pwm_chy.Pulse = rlr / 2;      //CCRx 比较值, 占空比50%
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&g_timx_rc_chy_handle,&timx_oc_pwm_chy,TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&g_timx_rc_chy_handle,TIM_CHANNEL_1);

    __HAL_TIM_ENABLE_IT(&g_timx_rc_chy_handle, TIM_IT_UPDATE);
}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM8) {
        //初始化PC6
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH};
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);

        __HAL_RCC_TIM8_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM8_UP_IRQn,15,0);    //TIM8 update interrupt
        HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
    }
}

void TIM8_UP_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_rc_chy_handle);
}

uint32_t g_pwm_count = 0 ;              /* CNT溢出次数 */
void atim_timx_npwm_chy_set(uint8_t pwm_count) {
    g_pwm_count = pwm_count;    //注：pwm_count必须要>0
    HAL_TIM_GenerateEvent(&g_timx_rc_chy_handle, TIM_EVENTSOURCE_UPDATE);   //启动后马上手动产生一次软件update interrupt，进入下面callback
    __HAL_TIM_ENABLE(&g_timx_rc_chy_handle);    //启动counter
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {  //有update interrupt、进入此函数
    if (htim->Instance == TIM8) {
        if (g_pwm_count) {
            TIM8->RCR = g_pwm_count - 1;
            HAL_TIM_GenerateEvent(&g_timx_rc_chy_handle, TIM_EVENTSOURCE_UPDATE);   //产生一次软件update interrupt，使RCR值到其影子
            // __HAL_TIM_ENABLE(&g_timx_rc_chy_handle);    //启动counter
            g_pwm_count = 0;
        } else {
            TIM8->CR1 &= ~(1 << 0); //CR1的位0置0，就是关闭counter；（为什么不用HAL库的__HAL_TIM_DISABLE()：它执行关闭有前提条件）
        }
    }
}