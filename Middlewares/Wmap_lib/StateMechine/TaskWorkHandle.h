#ifndef __TASKSINGLEWORK_H
#define	__TASKSINGLEWORK_H


#include "stm32_FH_xxx_hal.h"
#include <stdlib.h>
#include "string.h"












void Handle_watchdog_feed(void);
void Handle_IIC_TOUCH(void);
void Handle_DWT(void);

void Handle_usart(void);


void Handle_led(void);

void Handle_DMA_USART(void);

void Handle_DMA_USART_Save(void);

void Handle_DMA_USART_Send(void);

void Handle_letter_Shell_NoOsWhile(void);



#endif /* __TASKSINGLEWORK_H */




