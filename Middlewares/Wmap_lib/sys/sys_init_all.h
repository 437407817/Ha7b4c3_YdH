#ifndef __SYS_INIT_ALL_H
#define	__SYS_INIT_ALL_H


#include "./stm32_FH_xxx_hal.h"






#define NVIC_PriorityGroup_x  NVIC_PRIORITYGROUP_4




//..\..\Libraries\CMSIS;..\..\User;..\..\Libraries\FWlib\inc;
//..\..\FreeRTOS\include;..\..\FreeRTOS\portable\RVDS\ARM_CM3;..\..\FreeRTOS;

//..\..\User\spi\flash;..\..\SYSTEM\sys
//..\..\User\Led;..\..\User\TimBase;..\..\User\spi;																					











void ALL_Init_cfg(void);

void NeedToOnceOperate_Pro(void);
void NeedToRepeatOperate_Pro(void);





















#endif /* __SYS_INIT_ALL_H */
