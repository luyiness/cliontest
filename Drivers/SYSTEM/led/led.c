
#include "../sys/sys.h"

void led_init(void) {
    //PB5: led0 red
    //PE5: led1 green
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH};
    GPIO_InitTypeDef GPIO_InitStruct2 = {GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH};
                                                                            //输出模式没有上下拉；速度选低速即可
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); //初始时设灯为灭
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); //初始时设灯为灭
}
