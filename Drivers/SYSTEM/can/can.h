//
// Created by Administrator on 2026-01-09.
//

#ifndef CLIONTEST_CAN_H
#define CLIONTEST_CAN_H

#include "../sys/sys.h"

extern CAN_HandleTypeDef g_can_handle;

void can_init(void);
extern CAN_TxHeaderTypeDef g_tx_header;
extern CAN_RxHeaderTypeDef g_rx_header;
void can_send_message(uint32_t id, uint8_t *buf, uint8_t len);
uint8_t can_receive_message(uint8_t *buf);

#endif //CLIONTEST_CAN_H