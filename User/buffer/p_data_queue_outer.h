#ifndef __P_DATA_QUEUE_OUTER_H_
#define __P_DATA_QUEUE_OUTER_H_

//#include "stm32fxxx.h"
#include "./sys/sysio.h"
#include <string.h>
#include <stdio.h>

#include "./buffer/p_data_queue.h"
#include "./stm32_FH_xxx_hal.h"







void p_tx_rx_queue_init_all(UART_HandleTypeDef* uartHandle);
void p_tx_rx_queue_init_dma(void);

void Test_get_all_data_from_queue(UART_HandleTypeDef* uartHandle);
void Test_all_queue(void);




extern Q_QueueBuffer q_tx_rx_queue_2;
extern Q_QueueBuffer q_tx_rx_queue_UsartDMAsend;


#endif



