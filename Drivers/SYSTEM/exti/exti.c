//
// Created by LuYi on 18/12/2025.
//

#include "../sys/sys.h"
#include "../delay/delay.h"

void exti_init(void) {
    //1、
    __HAL_RCC_GPIOE_CLK_ENABLE();

    //2、gpio初始化：上拉电阻，下降沿触发
    GPIO_InitTypeDef gpio_init = {GPIO_PIN_3,GPIO_MODE_IT_FALLING,
        GPIO_PULLUP, GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOE, &gpio_init);

    //3、NVIC配置，分组SetPriorityGrouping已经在HAL_Init()配了，HAL_NVIC_SetPriority, HAL_NVIC_EnableIRQ
    HAL_NVIC_SetPriority(EXTI3_IRQn, 15, 0);    //EXTI3线，pre和sub根据需求设即可、注意此时分组=4
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);     //enable
}

//4、中断服务函数
void EXTI3_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   //调用gpio的中断处理函数，此处reset了中断标志位
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);    //再手动reset中断标志位
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { //实现Callback()
    delay_ms(20);   //消抖
    if (GPIO_Pin == GPIO_PIN_3) {
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)==0) {   //确认按键按下了
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);      //控灯
        }
    }
}