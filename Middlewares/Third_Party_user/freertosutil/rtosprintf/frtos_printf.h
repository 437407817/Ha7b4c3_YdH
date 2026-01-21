#ifndef __FRTOS_PRINTF_H
#define	__FRTOS_PRINTF_H

#include "stm32_FH_xxx_hal.h"





void SYSTEM_i_print(const char *fmt, ...);
 void SYSTEM_d_print(const char *file, int line, const char *func, const char *fmt, ...);
// ========== 简化宏定义（可选，方便调用） ==========
#define SYSTEM_I_PRINT(...)  SYSTEM_i_print(__VA_ARGS__)

#define SYSTEM_D_PRINT(...)  \
    SYSTEM_d_print(__FILE__, __LINE__, __func__, __VA_ARGS__)


void SYSTEM_set_rtosShell_running(void);


#endif







