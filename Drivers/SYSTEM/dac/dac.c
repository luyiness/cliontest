//
// Created by Administrator on 2026-01-06.
//

#include "dac.h"

DAC_HandleTypeDef g_dac_handle;

void dac_init() {
    g_dac_handle.Instance = DAC1;
    HAL_DAC_Init(&g_dac_handle);

    DAC_ChannelConfTypeDef sConfig;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;     //禁止触发
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;    //关闭缓冲
    HAL_DAC_ConfigChannel(&g_dac_handle,&sConfig,DAC_CHANNEL_1);

    HAL_DAC_Start(&g_dac_handle,DAC_CHANNEL_1);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance==DAC1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef gpio_InitStruct = {GPIO_PIN_4, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};   //注意GPIO模式是模拟输入
        HAL_GPIO_Init(GPIOA, &gpio_InitStruct);

        __HAL_RCC_DAC_CLK_ENABLE();
    }
}

//设置数字量
void set_Voltage(uint16_t vol) {
    //公式：DORx = vol * 4.096 / 3.3，见笔记
    uint32_t DORx = vol * 4.096 / 3.3;
    if (vol > 3300) DORx = 4095;    //要输出0~3,3v的一个电压；但设置的大于3.3v了、输出3.3v
    HAL_DAC_SetValue(&g_dac_handle,DAC_CHANNEL_1,DAC_ALIGN_12B_R,DORx);  //写入数字量，12位右对齐
}