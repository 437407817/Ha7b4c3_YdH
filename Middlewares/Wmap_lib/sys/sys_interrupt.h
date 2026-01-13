#ifndef __SYS_INTERRUPT_H
#define	__SYS_INTERRUPT_H


#include "./stm32_FH_xxx_hal.h"


int is_interrupt_disabled(void);
int is_faultmask_active(void);



void Software_Reset(void);







#endif /* __SYS_INTERRUPT_H */
