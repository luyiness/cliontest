//
// Created by LuYi on 18/12/2025.
//

#include "../sys/sys.h"
#include "../delay/delay.h"

void key_init(void) {
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t key_scan(void) {
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)==0) {   //按键按下了
        delay_ms(10);   //消抖
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)==0) {   //按键还在按着
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)==0) {}   //等待按键松开
            delay_ms(10);
            return 1;
        }
    }
    return 0;
}