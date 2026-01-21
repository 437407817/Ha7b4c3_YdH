#ifndef __HOOK_MEM_H
#define	__HOOK_MEM_H

#include "stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#include "FreeRTOS.h"



void vStartStackOverflowTest(void);

 void vPrintHeapState(const char *pcTag);


 void vPrintStack_TaskCreationResult(const char *pcTaskName, BaseType_t xCreationResult, uint32_t ulStackDepthInWords);






#endif







