//
// Created by Administrator on 2025-12-29.
//

#include "rtc.h"

RTC_HandleTypeDef g_rtc_handle;


uint8_t rtc_init(void) {
    __HAL_RCC_PWR_CLK_ENABLE();     //power clk
    __HAL_RCC_BKP_CLK_ENABLE();     //backup clk
    HAL_PWR_EnableBkUpAccess();     //取消备份区写保护

    g_rtc_handle.Instance = RTC;
    g_rtc_handle.Init.AsynchPrediv = 32767;
    g_rtc_handle.Init.OutPut = RTC_OUTPUTSOURCE_NONE;   //tamper pin的输出。没用到
    HAL_RTC_Init(&g_rtc_handle);

    if (rtc_read_bkr(0)!= 0x7777) {     //随便读和写一个数在后备区域SRAM就行，保证在掉电后不会重新set_time；测试：按下复位键时间不会重新set
        rtc_set_time(2025, 12, 29, 22, 24, 00);
        rtc_write_bkr(0, 0x7777);
    }
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc) {
    __HAL_RCC_RTC_ENABLE();     // RTC时钟使能

    //开启LSE
    RCC_OscInitTypeDef osc_init = {0};
    osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    osc_init.LSEState = RCC_LSE_ON;
    osc_init.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&osc_init);

    //设置RTC时钟源为LSE
    RCC_PeriphCLKInitTypeDef periph_clk_init = {0};
    periph_clk_init.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    periph_clk_init.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init);
}

uint32_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec) {
    uint32_t seccount = rtc_date2sec(syear, smon, sday, hour, min, sec);

    //（init()中已经使能了，但老师代码说还要执行一遍）
    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟 */
    __HAL_RCC_BKP_CLK_ENABLE(); /* 使能备份域时钟 */
    HAL_PWR_EnableBkUpAccess(); /* 取消备份域写保护 */

    RTC->CRL |= 1<<4;   //开启配置模式
    RTC->CNTL = seccount & 0xFFFF;  //赋值低16位
    RTC->CNTH = seccount >> 16;     //赋值高16位
    RTC->CRL &= ~(1<<4);    //关闭配置模式

    while (!__HAL_RTC_ALARM_GET_FLAG(&g_rtc_handle, RTC_FLAG_RTOFF));   //等待RTOFF=1、配置完成
    return seccount;
}

calendar_obj rtc_get_time(void) {
    uint32_t seccount = 0;
    //获取CNT值
    seccount = RTC->CNTH;
    seccount <<= 16;
    seccount |= RTC->CNTL;
    return sec2rtc_date(seccount);
}



//RTC写入后备区域SRAM； bkrx : 后备区寄存器编号,范围:0~41 对应 RTC_BKP_DR1~RTC_BKP_DR42
void rtc_write_bkr(uint32_t bkrx, uint16_t data)
{
    HAL_PWR_EnableBkUpAccess(); /* 取消备份区写保护 */
    HAL_RTCEx_BKUPWrite(&g_rtc_handle, bkrx + 1, data);
}

//RTC读取后备区域SRAM
uint16_t rtc_read_bkr(uint32_t bkrx)
{
    uint32_t temp = 0;
    temp = HAL_RTCEx_BKUPRead(&g_rtc_handle, bkrx + 1);
    return (uint16_t)temp; /* 返回读取到的值 */
}
