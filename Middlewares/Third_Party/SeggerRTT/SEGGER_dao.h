#ifndef __SEGGER_DAO_H
#define	__SEGGER_DAO_H


#include <stdlib.h>

#include "stm32_FH_xxx_hal.h"

//void SEGGER_PcDao_McuToCanTransform(CAN_TypeDef* CANx,uint8_t *array, uint16_t num);



void SEGGER_PcDao_UsartToMcuTransform(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num);
//void SEGGER_PcDao_McuToUsartTransform(USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void SEGGER_PcDao_McuToUsartTransform(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num);






void SEGGER_PcDao_Test256DataPrint(void);










#endif /* __SEGGER_DAO_H */

