//
// Created by Administrator on 2025-12-25.
//

#ifndef CLIONTEST_GTIM_ECM1_H
#define CLIONTEST_GTIM_ECM1_H

#include "../sys/sys.h"

void gtim_ecm1_init(uint32_t prescaler,uint32_t rlr);
uint16_t getCounterValue();

#endif //CLIONTEST_GTIM_ECM1_H