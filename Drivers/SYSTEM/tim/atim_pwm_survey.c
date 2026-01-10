//
// Created by Administrator on 2025-12-26.
//

#include "atim_pwm_survey.h"

TIM_HandleTypeDef g_atimx_pwm_survey_handle;

void atim_pwm_survey_init() {
    //TIM8_CH1（PC6）
    g_atimx_pwm_survey_handle.Instance = TIM8;
    g_atimx_pwm_survey_handle.Init.Prescaler = 0;
    g_atimx_pwm_survey_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_atimx_pwm_survey_handle.Init.Period = 65535;
    HAL_TIM_IC_Init(&g_atimx_pwm_survey_handle);

    //从模式 初始化，TI1FP1上升沿时 重置counter
    TIM_SlaveConfigTypeDef slave_config={0};
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;   //复位模式
    slave_config.InputTrigger = TIM_TS_TI1FP1;  //TI1FP1触发
    slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;  //上升沿触发
    HAL_TIM_SlaveConfigSynchro(&g_atimx_pwm_survey_handle,&slave_config);

    //ic部分，ic1、ic2输入初始化：  ic1捕获上升沿、得CCR1 算PWM周期，ic2捕获下降沿、得CCR2 算PWM高电平宽度
    TIM_IC_InitTypeDef ic1 = {0};
    ic1.ICPolarity = TIM_ICPOLARITY_RISING; //上升沿捕获
    ic1.ICSelection = TIM_ICSELECTION_DIRECTTI; //CC1S=01
    ic1.ICPrescaler = TIM_ICPSC_DIV1; //不分频
    HAL_TIM_IC_ConfigChannel(&g_atimx_pwm_survey_handle, &ic1, TIM_CHANNEL_1);
    TIM_IC_InitTypeDef ic2 = {0};
    ic2.ICPolarity = TIM_ICPOLARITY_FALLING; //下降沿捕获
    ic2.ICSelection = TIM_ICSELECTION_INDIRECTTI; //CC1S=10
    ic2.ICPrescaler = TIM_ICPSC_DIV1; //不分频
    HAL_TIM_IC_ConfigChannel(&g_atimx_pwm_survey_handle, &ic2, TIM_CHANNEL_2);

    // __HAL_TIM_ENABLE_IT(&g_atimx_pwm_survey_handle, TIM_IT_UPDATE); //enable update interrupt
    HAL_TIM_IC_Start_IT(&g_atimx_pwm_survey_handle, TIM_CHANNEL_1); //开启CH1，使能其CC中断（Capture Compare Interrupt）
    HAL_TIM_IC_Start(&g_atimx_pwm_survey_handle, TIM_CHANNEL_2);    //开启CH2，可不开其中断
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM8) {
        //初始化PC6
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH};
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        __HAL_RCC_TIM8_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM8_CC_IRQn, 15, 0);  //TIM8的CC中断
        HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
    }
}

void TIM8_CC_IRQHandler(void) {     //TIM8，CC中断函数
    HAL_TIM_IRQHandler(&g_atimx_pwm_survey_handle);
}

uint16_t g_timxchy_pwmin_sta;    /* PWM输入状态 */
uint32_t g_timxchy_pwmin_hval;   /* PWM的高电平脉宽, CCR2 */
uint32_t g_timxchy_pwmin_cval;

/* PWM输入模式 重新启动捕获 */
void atim_timx_pwmin_chy_restart(void) {
    sys_intx_disable();                     /* 关闭中断 */
    g_timxchy_pwmin_sta = 0;                /* 清零状态,重新开始检测 */
    g_timxchy_pwmin_hval=0;
    g_timxchy_pwmin_cval=0;
    sys_intx_enable();                      /* 打开中断 */
}

//Input capture callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {      //ic1上升沿进来
    if (htim->Instance == TIM8) {
        if (g_timxchy_pwmin_sta == 0) {
            if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
                g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_atimx_pwm_survey_handle, TIM_CHANNEL_1) + 2;    //+1是因为计n个数算n+1个计数时间，再+1是校正系数；（了解就行）
                g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_atimx_pwm_survey_handle, TIM_CHANNEL_2) + 2;
                g_timxchy_pwmin_sta = 1;
            }
        }
    }
}