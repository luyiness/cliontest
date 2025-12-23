//
// Created by Administrator on 2025-12-23.
//

#include "wwdg.h"

#include <stdio.h>

WWDG_HandleTypeDef g_wwdg_handle;

//1、初始化
void wwdg_init(uint8_t T,uint8_t W) {
    g_wwdg_handle.Instance = WWDG;
    g_wwdg_handle.Init.Prescaler = WWDG_PRESCALER_8;
    g_wwdg_handle.Init.Counter = T;
    g_wwdg_handle.Init.Window = W;
    g_wwdg_handle.Init.EWIMode = WWDG_EWI_ENABLE;
    HAL_WWDG_Init(&g_wwdg_handle);
}
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg) {
    //初始化中断：配置CLK、NVIC
    __HAL_RCC_WWDG_CLK_ENABLE();
    HAL_NVIC_SetPriority(WWDG_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

//2、中断函数
void WWDG_IRQHandler() {
    HAL_WWDG_IRQHandler(&g_wwdg_handle);
}
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg) {
    //注意此回调函数是 在WWDG计数器从0x40到0x3F之内运行，时间微秒级别，所以这里最好先喂狗、否则还没执行完就是复位了
    printf("reload wwdg early wakeup\r\n");
}