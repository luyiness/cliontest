//
// Created by Administrator on 2025-12-23.
//

#include "wdg.h"

IWDG_HandleTypeDef g_iwdg_handle;

void wdg_init(uint8_t prescaler,uint16_t rlr) {  //参数：预分频系数prescaler, 重装载值reload
    g_iwdg_handle.Instance = IWDG;
    g_iwdg_handle.Init.Prescaler = prescaler;
    g_iwdg_handle.Init.Reload = rlr;
    HAL_IWDG_Init(&g_iwdg_handle);
}

void wdg_reload(void) {
    HAL_IWDG_Refresh(&g_iwdg_handle);
}