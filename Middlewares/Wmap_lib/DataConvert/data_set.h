#ifndef __DATA_SET_H
#define	__DATA_SET_H



#include "stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>





#include <stdint.h>




// 1. 先定义函数指针类型（方便阅读）
typedef int8_t (*GetIncreaseValueFunc_t)(void);


void setInt16Array(int16_t *array, uint16_t len, GetIncreaseValueFunc_t func);



















#endif

