//
// Created by Administrator on 2026-01-05.
//

#include "adc1.h"

ADC_HandleTypeDef g_adc_handle;

void adc1_init() {
    g_adc_handle.Instance = ADC1;
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = DISABLE;       //是否扫描
    g_adc_handle.Init.ContinuousConvMode = ENABLE;     //单次/连续
    g_adc_handle.Init.NbrOfConversion = 1;
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;    //软件触发
    HAL_ADC_Init(&g_adc_handle);

    HAL_ADCEx_Calibration_Start(&g_adc_handle);     //开启校验

    //通道设置
    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_1;    //通道序号
    sConfig.Rank = ADC_REGULAR_RANK_1;  //rank
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  //采样sample时间
    HAL_ADC_ConfigChannel(&g_adc_handle, &sConfig);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        //PA1初始化
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef gpio_InitStruct = {GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};   //注意GPIO模式是模拟输入
        HAL_GPIO_Init(GPIOA, &gpio_InitStruct);

        //ADC时钟使能
        __HAL_RCC_ADC1_CLK_ENABLE();

        //设置扩展外设时钟，如：ADC、RTC
        RCC_PeriphCLKInitTypeDef  periphClkInit;
        periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        periphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;    //6分频
        HAL_RCCEx_PeriphCLKConfig(&periphClkInit);
    }
}

uint16_t getADCResult(void) {
    HAL_ADC_Start(&g_adc_handle);
    HAL_ADC_PollForConversion(&g_adc_handle, 10);   //等待10ms
    return HAL_ADC_GetValue(&g_adc_handle);     //获取DR寄存器的值
}