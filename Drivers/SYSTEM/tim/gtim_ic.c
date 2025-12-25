//
// Created by Administrator on 2025-12-24.
//

#include "gtim_ic.h"

TIM_HandleTypeDef g_timx_ic_chy_handle;

void gtim_ic_init(uint32_t prescaler, uint32_t rlr) {
    g_timx_ic_chy_handle.Instance = TIM5;
    g_timx_ic_chy_handle.Init.Prescaler = 16;
    g_timx_ic_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_ic_chy_handle.Init.Period = rlr;
    HAL_TIM_IC_Init(&g_timx_ic_chy_handle);

    TIM_IC_InitTypeDef ic_init={0};     //注意最好赋初始值0，否在没赋值的成员为随机值
    ic_init.ICPolarity = TIM_ICPOLARITY_RISING;  //上升沿捕获
    ic_init.ICSelection = TIM_ICSELECTION_DIRECTTI; //CC1S=01
    ic_init.ICFilter = 0;
    ic_init.ICPrescaler = TIM_ICPSC_DIV1;   //不分频
    HAL_TIM_IC_ConfigChannel(&g_timx_ic_chy_handle, &ic_init, TIM_CHANNEL_1);

    __HAL_TIM_ENABLE_IT(&g_timx_ic_chy_handle, TIM_IT_UPDATE);     //enable update interrupt
    HAL_TIM_IC_Start_IT(&g_timx_ic_chy_handle, TIM_CHANNEL_1);
}
// void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
//     if (htim->Instance == TIM5) {
//         //初始化PA0
//         //1、IO口输出初始化
//         __HAL_RCC_GPIOA_CLK_ENABLE();
//         GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH};
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//         //初始化TIM5时钟
//         __HAL_RCC_TIM5_CLK_ENABLE();
//
//         //NVIC
//         HAL_NVIC_SetPriority(TIM5_IRQn, 15, 0);
//         HAL_NVIC_EnableIRQ(TIM5_IRQn);
//     }
// }

void TIM5_IRQHandler(void) {
    HAL_TIM_IRQHandler(&g_timx_ic_chy_handle);
}

uint8_t g_timxchy_cap_sta = 0;
uint16_t g_timxchy_cap_val = 0;

//Input capture callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {      //注意只要进入了这个函数，就是捕获到了上升/下降沿、这是中断判断的，用户代码只写：第一次进入是上升沿、写其相关逻辑，第二次进入是下降沿、写其相关逻辑
    if (htim->Instance == TIM5) {
        if (!(g_timxchy_cap_sta & 0X80)) {  //[7]位==0时
            if (g_timxchy_cap_sta & 0X40) {  //[6]位==1时
                //捕获到下降沿
                g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_ic_chy_handle, TIM_CHANNEL_1);    //获取CCRx
                //为下次测量做准备，标记此次测量完毕
                TIM_RESET_CAPTUREPOLARITY(&g_timx_ic_chy_handle, TIM_CHANNEL_1);    //清除原来的上/下沿触发配置；capture polarity
                TIM_SET_CAPTUREPOLARITY(&g_timx_ic_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);  //设上升沿触发；capture polarity
                g_timxchy_cap_sta |= 0x80;
            } else {
                //捕获到上升沿
                g_timxchy_cap_sta = 0;      //首先变量清零，清空上次测试的数据
                g_timxchy_cap_val = 0;      //首先变量清零，清空上次测试的数据
                __HAL_TIM_SET_COUNTER(&g_timx_ic_chy_handle,0); //计数器清零
                TIM_RESET_CAPTUREPOLARITY(&g_timx_ic_chy_handle, TIM_CHANNEL_1);    //清除原来的上/下沿触发配置；capture polarity
                TIM_SET_CAPTUREPOLARITY(&g_timx_ic_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);  //设下降沿触发；capture polarity
                g_timxchy_cap_sta |= 0X40;  //[6]位置1
            }
        }
    }
}

//update event callback
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {       //只要CNT溢出，就进入此函数
//     if (htim->Instance == TIM5) {
//         if (!(g_timxchy_cap_sta & 0X80)) {
//             if (g_timxchy_cap_sta & 0X40) {     //若已经捕获到上升沿后（t1后），开始N++、记录溢出次数
//                 if ((g_timxchy_cap_sta & 0X3F) == 0X3F){    //溢出次数N已达到最大63；（[6:0]位=1111111）
//                     //g_timxchy_cap_val直接=最大值，
//                     g_timxchy_cap_val = 0XFFFF;
//                     //为下次测量做准备，标记此次测量完毕
//                     TIM_RESET_CAPTUREPOLARITY(&g_timx_ic_chy_handle, TIM_CHANNEL_1);    //清除原来的上/下沿触发配置；capture polarity
//                     TIM_SET_CAPTUREPOLARITY(&g_timx_ic_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);  //设上升沿触发；capture polarity
//                     g_timxchy_cap_sta |= 0x80;
//                 }else {
//                     g_timxchy_cap_sta++;
//                 }
//             }
//         }
//     }
// }