//
// Created by Administrator on 2025-12-31.
//

#include "pvd.h"

void pvd_init(void) {
    __HAL_RCC_PWR_CLK_ENABLE();     //pwr clock

    PWR_PVDTypeDef sConfigPVD;
    sConfigPVD.PVDLevel = PWR_PVDLEVEL_7;   //PLS=111
    sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;     //上升沿和下降沿触发
    HAL_PWR_ConfigPVD(&sConfigPVD);     //上两句的设置：VDD大于2.88时中断，VDD小于2.78中断

    HAL_PWR_EnablePVD();

    HAL_NVIC_SetPriority(PVD_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(PVD_IRQn);
}

void PVD_IRQHandler(void) {
    HAL_PWR_PVD_IRQHandler();
}
void HAL_PWR_PVDCallback(void) {
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO)) {    //查看PVDO位
        pvdo = 1;
    } else {
        pvdo = 2;
    }
}