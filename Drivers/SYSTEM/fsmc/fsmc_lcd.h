//
// Created by Administrator on 2025-12-28.
//

#ifndef CLIONTEST_FSMC_LCD_H
#define CLIONTEST_FSMC_LCD_H

#include "../sys/sys.h"

/* LCD用到引脚宏定义 */

/* 写引脚 */
#define LCD_WR_GPIO_PORT                    GPIOD
#define LCD_WR_GPIO_PIN                     GPIO_PIN_5
#define LCD_WR_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

/* 读引脚 */
#define LCD_RD_GPIO_PORT                    GPIOD
#define LCD_RD_GPIO_PIN                     GPIO_PIN_4
#define LCD_RD_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

/* 16个数据引脚 参考原理图 */

/* LCD_BL背光引脚 */
#define LCD_BL_GPIO_PORT                    GPIOB
#define LCD_BL_GPIO_PIN                     GPIO_PIN_0
#define LCD_BL_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* CS片选引脚 */
#define LCD_CS_GPIO_PORT                    GPIOG
#define LCD_CS_GPIO_PIN                     GPIO_PIN_12
#define LCD_CS_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)     /* PG口时钟使能 */

/* RS数据类型引脚 */
#define LCD_RS_GPIO_PORT                    GPIOG
#define LCD_RS_GPIO_PIN                     GPIO_PIN_0
#define LCD_RS_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)     /* PG口时钟使能 */


/* LCD背光控制，和HAL_GPIO_WritePin一样，只不过是直接操作BSRR寄存器 */
#define LCD_BL(x)       LCD_BL_GPIO_PORT->BSRR = LCD_BL_GPIO_PIN << (16 * (!x))

#define LCD_FSMC_NEX         4      //CS，FSMC_NE4 块1区4
#define LCD_FSMC_AX          10     //RS，FSMC_A10

#define FSMC_ADDR_DATA          ((uint32_t) 0X6C000800)
#define FSMC_ADDR_CMD           ((uint32_t) 0X6C000000)

void fsmc_lcd_init();

#endif //CLIONTEST_FSMC_LCD_H