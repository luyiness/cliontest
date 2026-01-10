//
// Created by Administrator on 2026-01-09.
//

#include "can.h"
CAN_HandleTypeDef g_can_handle;

void can_init(void) {
    g_can_handle.Instance = CAN1;
    g_can_handle.Init.Mode = CAN_MODE_LOOPBACK;   //回环模式
    //波特率 = 36000 / [( 9 + 8 + 1 ) * 4] = 500Kbps
    g_can_handle.Init.Prescaler = 4;      //设置tq=4
    g_can_handle.Init.TimeSeg1 = CAN_BS1_9TQ; //设置tBS1=9
    g_can_handle.Init.TimeSeg1 = CAN_BS1_8TQ; //设置tBS1=8
    g_can_handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
    g_can_handle.Init.AutoBusOff           = DISABLE;  /* 禁止自动离线管理 */
    g_can_handle.Init.AutoRetransmission   = DISABLE;  /* 禁止自动重发 */
    g_can_handle.Init.AutoWakeUp           = DISABLE;  /* 禁止自动唤醒 */
    g_can_handle.Init.ReceiveFifoLocked    = DISABLE;  /* 禁止接收FIFO锁定 */
    g_can_handle.Init.TimeTriggeredMode    = DISABLE;  /* 禁止时间触发通信模式 */
    g_can_handle.Init.TransmitFifoPriority = DISABLE;  /* 禁止发送FIFO优先级 */

    HAL_CAN_Init(&g_can_handle);

    //配置filter
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;   //mask mode
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;  //32位
    sFilterConfig.FilterIdHigh = 0x0000;    //设置成接收所有报文，FxR1=0 FxR2=0
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterBank = 0;   //用第0个过滤器；共14个
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;      //关联FIFO0
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;    //(默认值)
    HAL_CAN_ConfigFilter(&g_can_handle, &sFilterConfig);

    HAL_CAN_Start(&g_can_handle);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_12;      //CAN外设的TX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_11;      //CAN外设的RX
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

CAN_TxHeaderTypeDef g_tx_header;
CAN_RxHeaderTypeDef g_rx_header;

//发送数据；
void can_send_message(uint32_t id, uint8_t *buf, uint8_t len) {     //参数id是ExtID,len是buf长度
    g_tx_header.ExtId = id;
    //赋值了ExtID，不用g_tx_header.StdId =xxx了
    g_tx_header.DLC = len;
    g_tx_header.IDE = CAN_ID_EXT;   //Standard Id or Extended Id
    g_tx_header.RTR = CAN_RTR_DATA;     //data or remote

    uint32_t tx_mailbox = CAN_TX_MAILBOX0;  //邮箱0
    HAL_CAN_AddTxMessage(&g_can_handle, &g_tx_header, buf, &tx_mailbox);

    while (HAL_CAN_GetTxMailboxesFreeLevel(&g_can_handle) != 3);    //==3时说明3个邮箱皆为空，说明数据发送完成
}

//接收数据，放到buf中
uint8_t can_receive_message(uint8_t *buf) {
    if (HAL_CAN_GetRxFifoFillLevel(&g_can_handle, CAN_RX_FIFO0) == 0) { //FIFO0没收到数据
        return 0;
    }
    HAL_CAN_GetRxMessage(&g_can_handle, CAN_RX_FIFO0, &g_rx_header, buf);
    return g_rx_header.DLC; //
}