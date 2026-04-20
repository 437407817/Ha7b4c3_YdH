#ifndef __SEGGER_TOOLKIT_H
#define	__SEGGER_TOOLKIT_H


#include "stm32_FH_xxx_hal.h"
#include <stdlib.h>
#include "./usart/bsp_usart_shell.h"

#define SEGGER_UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */


#define SEGGER_hostDataEliminateSpace   1    // 1:去空格发送  0:不处理

#define SEGGER_PCsendDataCount   50 //PC端发送最大字节数，包括空格等其他非0-e字符



#define SEGGER_USART_A huart_shell_Handle


void SEGGER_HostDataToUsart(void);

//void SYSTEM_DEBUG_SEGGER_CANDataToHost(CanTxMsg* ptr_cantxmsg);


 void segger_outfun(uint8_t * tmp,uint16_t retlen);

#endif /* __SEGGER_TOOLKIT_H */

