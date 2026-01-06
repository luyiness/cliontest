//
// Created by Administrator on 2026-01-06.
//

#include "dac.h"

DAC_HandleTypeDef g_dac_handle;

void dac_init() {
    g_dac_handle.Instance = DAC1;
    HAL_DAC_Init(&g_dac_handle);

    DAC_ChannelConfTypeDef sConfig;
    sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;     //禁止触发
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

/**
 * @brief       设置DAC_OUT1输出三角波
 *   @note      输出频率 ≈ 1000 / (dt * samples) Khz, 不过在dt较小的时候,比如小于5us时, 由于delay_us
 *              本身就不准了(调用函数,计算等都需要时间,延时很小的时候,这些时间会影响到延时), 频率会偏小.
 *
 * @param       maxval : 最大值(0 < maxval < 4096), (maxval + 1)必须大于等于samples/2
 * @param       dt     : 每个采样点的延时时间(单位: us)
 * @param       samples: 采样点的个数, samples必须小于等于(maxval + 1) * 2 , 且maxval不能等于0
 * @param       n      : 输出波形个数,0~65535
 *
 * @retval      无
 */
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n) {
    uint16_t i,j;
    float curval;    //递增量，当前值
    if (samples > (maxval+1)*2) return;
    float incval = (maxval + 1) / (samples / 2);  //相邻采样点的幅值

    for (i=0;i<n;i++) {
        curval = 0;
        HAL_DAC_SetValue(&g_dac_handle,DAC_CHANNEL_1,DAC_ALIGN_12B_R,curval);   //先输出0
        for (j=0;j<samples/2;j++) {
            curval += incval;
            HAL_DAC_SetValue(&g_dac_handle,DAC_CHANNEL_1,DAC_ALIGN_12B_R,curval);
            delay_us(dt);
        }
        for (j=0;j<samples/2;j++) {
            curval -= incval;
            HAL_DAC_SetValue(&g_dac_handle,DAC_CHANNEL_1,DAC_ALIGN_12B_R,curval);
            delay_us(dt);
        }
    }
}