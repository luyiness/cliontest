//
// Created by Administrator on 2026-01-06.
//

#include "iic.h"

void at24c02_write_one_byte(uint8_t addr, uint8_t data) {
    iic_start();
    iic_send_byte(0xA0);
    iic_wait_ack();
    iic_send_byte(addr);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
    delay_ms(10);   //发完1B后，要等待
}

uint8_t at24c02_read_one_byte(uint8_t addr) {
    uint8_t data;
    iic_start();
    iic_send_byte(0xA0);
    iic_wait_ack();
    iic_send_byte(addr);
    iic_wait_ack();

    iic_start();
    iic_send_byte(0xA1);
    iic_wait_ack();
    data = iic_read_byte(0);    //接收数据，并发送nack
    iic_stop();
    return data;
}