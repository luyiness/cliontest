//
// Created by Administrator on 2025-12-28.
//

#include "fsmc_lcd.h"
#include "pack/util.h"
#include "stm32f1xx_hal_sram.h"
#include "../delay/delay.h"

SRAM_HandleTypeDef g_sram_handle;   //句柄
_lcd_dev lcddev = {0};

void fsmc_lcd_init() {
    //1、HAL_SRAM_Init
    g_sram_handle.Instance = FSMC_NORSRAM_DEVICE;
    g_sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    g_sram_handle.Init.NSBank = FSMC_NORSRAM_BANK4;
    g_sram_handle.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    g_sram_handle.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    g_sram_handle.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    g_sram_handle.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    g_sram_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    g_sram_handle.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    g_sram_handle.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    g_sram_handle.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    g_sram_handle.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
    g_sram_handle.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    g_sram_handle.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

    FSMC_NORSRAM_TimingTypeDef fsmc_read_handle;
    FSMC_NORSRAM_TimingTypeDef fsmc_write_handle;

    fsmc_read_handle.AccessMode = FSMC_ACCESS_MODE_A;
    fsmc_read_handle.DataSetupTime = 15;    //相当于RD低电平持续时间
    fsmc_read_handle.AddressHoldTime = 0;  //地址保持时间(ADDHLD) 模式A是没有用到
    fsmc_read_handle.AddressSetupTime = 1;  //相当于RD高电平持续时间
    fsmc_write_handle.AccessMode = FSMC_ACCESS_MODE_A;
    fsmc_read_handle.DataSetupTime = 3;    //相当于WR低电平持续时间
    fsmc_read_handle.AddressHoldTime = 0;
    fsmc_read_handle.AddressSetupTime = 1;  //相当于WR高电平持续时间

    HAL_SRAM_Init(&g_sram_handle, &fsmc_read_handle, &fsmc_write_handle);

    //2、读取驱动IC的ID
    delay_ms(50);
    lcd_wr_regno(0xD3);
    lcddev.id = lcd_rd_data();  /* 假读 */
    lcddev.id = lcd_rd_data();  /* 00 */
    lcddev.id = lcd_rd_data();  /* 93 */
    lcddev.id <<= 8;
    lcddev.id |= lcd_rd_data();  /* 41 */

    //printf("lcddev_id:%#x \r\n", lcddev.id);

    /* 3、完成初始化序列 */
    if (lcddev.id == 0x9341)
        lcd_ex_ili9341_reginit();
    else
        lcd_ex_st7789_reginit();

    /* 4、对LCD控制结构体赋值 */
    lcddev.width = 240;
    lcddev.height = 320;
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    lcddev.wramcmd = 0x2C;

    lcd_wr_regno(lcddev.setxcmd);
    lcd_wr_data(0);
    lcd_wr_data(0);
    lcd_wr_data((lcddev.width - 1) >> 8);
    lcd_wr_data((lcddev.width - 1) & 0XFF);
    lcd_wr_regno(lcddev.setycmd);
    lcd_wr_data(0);
    lcd_wr_data(0);
    lcd_wr_data((lcddev.height - 1) >> 8);
    lcd_wr_data((lcddev.height - 1) & 0XFF);

    /* 5、设置扫描方向 */
    lcd_write_reg(0x36, 1 << 3);

    /* 6、点亮背光 */
    LCD_BL(1);

    /* 7、lcd_clear */
    lcd_clear(0xFFFF);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    __HAL_RCC_FSMC_CLK_ENABLE();    //FSMC时钟初始化

    //用到的GPIO口初始化
    LCD_WR_GPIO_CLK_ENABLE();
    LCD_RD_GPIO_CLK_ENABLE();
    LCD_BL_GPIO_CLK_ENABLE();
    LCD_CS_GPIO_CLK_ENABLE();
    LCD_RS_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = LCD_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_CS引脚 */
    gpio_init_struct.Pin = LCD_WR_GPIO_PIN;
    HAL_GPIO_Init(LCD_WR_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_WR引脚 */
    gpio_init_struct.Pin = LCD_RD_GPIO_PIN;
    HAL_GPIO_Init(LCD_RD_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_RD引脚 */
    gpio_init_struct.Pin = LCD_RS_GPIO_PIN;
    HAL_GPIO_Init(LCD_RS_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_RS引脚 */
    gpio_init_struct.Pin = LCD_BL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    HAL_GPIO_Init(LCD_BL_GPIO_PORT, &gpio_init_struct);     /* LCD_BL引脚模式设置(推挽输出) */
}

