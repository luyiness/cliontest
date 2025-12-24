//
// Created by Administrator on 2025-12-24.
//

#ifndef CLIONTEST_GTIM_IC_H
#define CLIONTEST_GTIM_IC_H

#include "../sys/sys.h"

void gtim_ic_init(uint32_t prescaler, uint32_t rlr);

/* 输入捕获状态(g_timxchy_cap_sta)
* [7]  =0准备捕获（就是t1之前）；=1成功获取到了两次捕获（就是t2之后）、这时就是测量完成一次了，等手动置0 就是下次测量了；
* [6]  =0,还没捕获到上升沿; =1,已经捕获到上升沿了.
* [5:0]  =溢出次数N, 最大63,所以最长捕获值 = 63*65536 + 65535 = 4194303； 若CNT频率1MHz、则约4.19秒
*/
extern uint8_t g_timxchy_cap_sta;

//存t2时的计数器值CCRx2
extern uint16_t g_timxchy_cap_val;

#endif //CLIONTEST_GTIM_IC_H