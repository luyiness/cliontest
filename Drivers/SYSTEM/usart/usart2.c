#include "../sys/sys.h"
#include "../usart/usart2.h"

#include "stm32f1xx_hal_uart.h"

UART_HandleTypeDef g_huart;
uint8_t g_rx_buffer[1];
_Bool g_rx_flag = 0;

void usart_init(uint32_t baudrate) {
    //1、初始化串口1
    g_huart.Instance = USART1;
    g_huart.Init.BaudRate = baudrate;
    g_huart.Init.WordLength = UART_WORDLENGTH_8B;
    g_huart.Init.StopBits = UART_STOPBITS_1;
    g_huart.Init.Parity = UART_PARITY_NONE;
    g_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_huart.Init.Mode = UART_MODE_TX_RX;      //收和发
    g_huart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&g_huart);  //调用hal库的Uart初始化函数，其会间接调用msp callback；

    //3、开启接收中断
    HAL_UART_Receive_IT(&g_huart, (uint8_t*)g_rx_buffer, 1);
}

//2、msp callback
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        //1）配置CLOCK
        __HAL_RCC_USART1_CLK_ENABLE();      //使能usart1时钟，在_hal_rcc.h中
        __HAL_RCC_GPIOA_CLK_ENABLE();

        //2）初始化引脚
        //初始化PA9，TXD复用，复用推挽式输出模式，（因为是输出 上下拉设不设都无效）
        GPIO_InitTypeDef txd = {GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH};
        HAL_GPIO_Init(GPIOA, &txd);
        //初始化PA10，RXD复用，输入模式
        GPIO_InitTypeDef rxd = {GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH};
        HAL_GPIO_Init(GPIOA, &rxd);

        //3）NVIC
        HAL_NVIC_SetPriority(USART1_IRQn, 15,0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);     //enable
    }
}

//usart1中断服务函数
void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&g_huart);
    HAL_UART_Receive_IT(&g_huart, (uint8_t*)g_rx_buffer, 1);  //因为上一行会清除接收中断标志位，此处再开启；
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        g_rx_flag = 1;  //接收到数据了，g_rx_flag=1
    }
}