#ifndef _HAL_EXT_H_
#define _HAL_EXT_H_

#include <stdint.h>


#include "./stm32_FH_xxx_hal.h"

















void uart_debug_dump(UART_HandleTypeDef *huart);


void uart_debug_checkerror(UART_HandleTypeDef *huart,FunctionalState ablestate);

void lpuart_disable_fifo_hal(UART_HandleTypeDef *huart);












#endif
