//
// Created by Administrator on 2025-12-25.
//

#include "gtim_ecm1.h"

TIM_HandleTypeDef g_timx_ecm_chy_handle;

void gtim_ecm1_init(uint32_t prescaler,uint32_t rlr) {
    g_timx_ecm_chy_handle.Instance = TIM2;      //TIM2_CH1, 复用的PA10、其接的是KEY_UP按键
    g_timx_ecm_chy_handle.Init.Prescaler = prescaler;
    g_timx_ecm_chy_handle.Init.Period = rlr;
    g_timx_ecm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_IC_Init(&g_timx_ecm_chy_handle);

    TIM_SlaveConfigTypeDef slave_config={0};
    slave_config.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
    slave_config.InputTrigger = TIM_TS_TI1FP1;
    slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
    HAL_TIM_SlaveConfigSynchro(&g_timx_ecm_chy_handle,&slave_config);   //初始化ic部分

    HAL_TIM_IC_Start(&g_timx_ecm_chy_handle, TIM_CHANNEL_1);     //使能ic部分，开启counter
}
// void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
//     if (htim->Instance == TIM2) {
//         //初始化PA0
//         __HAL_RCC_GPIOA_CLK_ENABLE();
//         GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH};
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//         __HAL_RCC_TIM2_CLK_ENABLE();
//     }
// }

uint16_t getCounterValue() {
    uint16_t count = __HAL_TIM_GET_COUNTER(&g_timx_ecm_chy_handle);
    if (count > 20) {
        //记录到20时，重新记录
        //可用这个_HAL_TIM_SET_COUNTER(&handle,0);
    }
    return count;
}