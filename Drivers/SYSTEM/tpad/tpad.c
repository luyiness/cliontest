//
// Created by Administrator on 2025-12-26.
//

#include "tpad.h"
#include "../delay/delay.h"

 TIM_HandleTypeDef g_timx_ic_chy_handle;
 uint8_t g_timxchy_cap_sta;
 uint16_t g_timxchy_cap_val;
 uint8_t g_timxchy_cap_spill;    //溢出信号，因为没考虑计数器在测量过程中溢出；

void tpad_init() {
    gtim_ic_init(5,65535);
}

uint16_t tpad_get_val(void) {
    //PA1，推挽输出
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio_InitStruct = {GPIO_PIN_1, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM};
    HAL_GPIO_Init(GPIOA, &gpio_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   //C放电
    delay_ms(5);

    g_timxchy_cap_sta = 0;
    g_timxchy_cap_spill = 0;
    g_timx_ic_chy_handle.Instance->SR = 0;        //重置SR状态寄存器

    //改为浮空输入
    gpio_InitStruct.Mode = GPIO_MODE_INPUT;
    gpio_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio_InitStruct); //这时电容开始充电

    g_timx_ic_chy_handle.Instance->CNT = 0;        //先清空CNT，开始从0计数
    // while (1) {
    delay_ms(1000);
    if (g_timxchy_cap_sta) {
            return g_timxchy_cap_val;
        }
    if (g_timxchy_cap_spill) {
            return 0;
        }
    // }
}

void gtim_ic_init(uint32_t prescaler, uint32_t rlr) {
    //TIM5_CH2
    g_timx_ic_chy_handle.Instance = TIM5;
    g_timx_ic_chy_handle.Init.Prescaler = prescaler;
    g_timx_ic_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_ic_chy_handle.Init.Period = rlr;
    HAL_TIM_IC_Init(&g_timx_ic_chy_handle);

    TIM_IC_InitTypeDef ic_init={0};     //注意最好赋初始值0，否在没赋值的成员为随机值
    ic_init.ICPolarity = TIM_ICPOLARITY_RISING;  //上升沿捕获
    ic_init.ICSelection = TIM_ICSELECTION_DIRECTTI; //CC1S=01
    ic_init.ICFilter = 0;
    ic_init.ICPrescaler = TIM_ICPSC_DIV1;   //不分频
    HAL_TIM_IC_ConfigChannel(&g_timx_ic_chy_handle, &ic_init, TIM_CHANNEL_2);

    __HAL_TIM_ENABLE_IT(&g_timx_ic_chy_handle, TIM_IT_UPDATE);     //enable update interrupt
    HAL_TIM_IC_Start_IT(&g_timx_ic_chy_handle, TIM_CHANNEL_2);
}
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM5) {
        //    tpad_get_value()已经初始化IO口了
        // __HAL_RCC_GPIOA_CLK_ENABLE();
        // GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH};
        // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        //初始化TIM5时钟
        __HAL_RCC_TIM5_CLK_ENABLE();

        //NVIC
        HAL_NVIC_SetPriority(TIM5_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
}

void TIM5_IRQHandler(void) {
    HAL_TIM_IRQHandler(&g_timx_ic_chy_handle);
}

//Input capture callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {      //注意只要进入了这个函数，就是捕获到了上升/下降沿、这是中断判断的，用户代码只写：第一次进入是上升沿、写其相关逻辑，第二次进入是下降沿、写其相关逻辑
    if (htim->Instance == TIM5) {
        if (!(g_timxchy_cap_sta)) {
                //捕获到上升沿
                g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_ic_chy_handle, TIM_CHANNEL_2);
                __HAL_TIM_SET_COUNTER(&g_timx_ic_chy_handle,0); //计数器清零
                g_timxchy_cap_sta = 1;
        }
    }
}
//update event callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {       //只要CNT溢出，就进入此函数
    if (htim->Instance == TIM5) {
        if (!(g_timxchy_cap_sta & 0X80)) {
            if (!(g_timxchy_cap_sta)) {
                g_timxchy_cap_spill = 1;
            }
        }
    }
}