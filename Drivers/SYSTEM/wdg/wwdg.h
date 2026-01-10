//
// Created by Administrator on 2025-12-23.
//

#ifndef CLIONTEST_WWDG_H
#define CLIONTEST_WWDG_H

#include "../sys/sys.h"

extern WWDG_HandleTypeDef g_wwdg_handle;
void wwdg_init(uint8_t T,uint8_t W);

#endif //CLIONTEST_WWDG_H