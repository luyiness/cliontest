//
// Created by Administrator on 2025-12-28.
//

#ifndef CLIONTEST_UTIL_H
#define CLIONTEST_UTIL_H

// const uint32_t g_point_color = 0XF800;    /* 画笔颜色 */
// const uint32_t g_back_color  = 0XFFFF;    /* 背景色 */

//#define LCD_WR(x)       LCD_WR_GPIO_PORT->BSRR = LCD_WR_GPIO_PIN << (16 * (!x))     /* 设置WR引脚 */

/* LCD结构体 */
typedef struct
{
    uint16_t width;     /* LCD 宽度 */
    uint16_t height;    /* LCD 高度 */
    uint16_t id;        /* LCD ID */
    uint8_t dir;        /* 横屏还是竖屏控制：0，竖屏；1，横屏。 */
    uint16_t wramcmd;   /* 开始写gram指令 */
    uint16_t setxcmd;   /* 设置x坐标指令 */
    uint16_t setycmd;   /* 设置y坐标指令 */
} _lcd_dev;

extern _lcd_dev lcddev;   //lcd结构体变量

/* 常用画笔颜色 */
#define WHITE           0xFFFF      /* 白色 */
#define BLACK           0x0000      /* 黑色 */
#define RED             0xF800      /* 红色 */
#define GREEN           0x07E0      /* 绿色 */
#define BLUE            0x001F      /* 蓝色 */
#define MAGENTA         0XF81F      /* 品红色/紫红色 = BLUE + RED */
#define YELLOW          0XFFE0      /* 黄色 = GREEN + RED */
#define CYAN            0X07FF      /* 青色 = GREEN + BLUE */

/* 非常用颜色 */
#define BROWN           0XBC40      /* 棕色 */
#define BRRED           0XFC07      /* 棕红色 */
#define GRAY            0X8430      /* 灰色 */
#define DARKBLUE        0X01CF      /* 深蓝色 */
#define LIGHTBLUE       0X7D7C      /* 浅蓝色 */
#define GRAYBLUE        0X5458      /* 灰蓝色 */
#define LIGHTGREEN      0X841F      /* 浅绿色 */
#define LGRAY           0XC618      /* 浅灰色(PANNEL),窗体背景色 */
#define LGRAYBLUE       0XA651      /* 浅灰蓝色(中间层颜色) */
#define LBBLUE          0X2B12      /* 浅棕蓝色(选择条目的反色) */

/* 函数声明 */
void lcd_init(void);
void lcd_wr_regno(volatile uint16_t cmd);   //写 命令
void lcd_wr_data(volatile uint16_t data);   //写 数据
uint16_t lcd_rd_data(void);     //读
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);    //画点
uint16_t lcd_read_point (uint16_t x, uint16_t y);
void lcd_clear(uint16_t color);
void lcd_write_reg(uint16_t regno, uint16_t data);

void lcd_ex_ili9341_reginit(void);

void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);
void lcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color);          /* 纯色填充矩形(32位颜色,兼容LTDC) */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);                  /* 画圆 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);                     /* 显示数字 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);      /* 扩展显示数字 */
void lcd_display_dir(uint8_t dir);          /* 设置屏幕显示方向 */
#endif //CLIONTEST_UTIL_H