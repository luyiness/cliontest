//
// Created by Administrator on 2026-01-06.
//

#ifndef CLIONTEST_IIC_H
#define CLIONTEST_IIC_H

#include "../sys/sys.h"
#include "../delay/delay.h"

void iic_init(void);
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t data);
uint8_t iic_read_byte (uint8_t ack);

//-----------------总线操作---------------------------------------------
#define IIC_SCL(x)        do{ x ? \
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); \
}while(0)       /* SCL线 */

#define IIC_SDA(x)        do{ x ? \
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); \
}while(0)       /* SDA线 */

#define IIC_READ_SDA     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)  //读SDA线


void at24c02_write_one_byte(uint8_t addr, uint8_t data);
uint8_t at24c02_read_one_byte(uint8_t addr);

#endif //CLIONTEST_IIC_H