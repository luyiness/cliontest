//
// Created by Administrator on 2025-12-23.
//

#include "btim.h"

TIM_HandleTypeDef g_timHandle;

void btim_init(uint32_t prescaler,uint32_t arr) {
    g_timHandle.Instance = TIM6;
    g_timHandle.Init.Prescaler = prescaler;
    g_timHandle.Init.Period = arr;
    HAL_TIM_Base_Init(&g_timHandle);

    HAL_TIM_Base_Start_IT(&g_timHandle);
}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
    }
}

void TIM6_IRQHandler(void) {
    HAL_TIM_IRQHandler(&g_timHandle);
}
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     if (htim->Instance == TIM6) {
//         HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
//     }
// }