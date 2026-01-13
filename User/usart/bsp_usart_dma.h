/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.h
  * @brief   This file contains all the function prototypes for
  *          the dma.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_USART_DMA_H__
#define __BSP_USART_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./user_config.h"
/* DMA memory to memory transfer handles -------------------------------------*/


/* USER CODE BEGIN Includes */
 #include "./buffer/p_data_queue_outer.h"
 #include "./buffer/queue2.h"
//#include "./usart/p_data_queue.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define TX_COMPLETE_TIMEOUT 3000
#define TX_WAITTING_TIMEOUT 5000


#define MAX_BUF_SIZE 5
#define MAX_RING_BUFF_SIZE MAX_BUF_SIZE*10

#define DMA_USARTx 														USART_SHELL
#define USARTx_DMA_IRQHandler                 USART_SHELL_IRQHandler
#define USARTx_DMA_IRQ                 		    USART_SHELL_IRQ

#define USE_UARTx_DMA 1
//#define USE_UART_DMA_RX 1
//#define USE_UART_DMA_TX 1


#define TEST_RingBuffer_SAVEANDREAD 0
#define TESTUsartDMASendSaveAndSend 0
#define TEST_DMA_RB_READ	0			//测试接收功能
#define huart_DMA_Handle huart_shell_Handle 




typedef struct {
uint8_t send_data[q_QUEUE_NODE_DATA_LEN_UsartDMAsend];
uint8_t	uart_tx_justSaveOver;	
uint8_t	uart_tx_complete;
uint16_t	read_out_len;
uint8_t		daret;
uint8_t		dbret;
uint32_t	complete_timeout;
uint32_t	current_time;
uint32_t last_tx_complete_time;
}str_DMA_usart_send;

typedef struct
{
//	QueueType_t g_rcvQueue;
	 RingBuffer_t g_uartRingBuf;  
	uint8_t g_ringBufData[MAX_RING_BUFF_SIZE]; // 缓冲区数据存储
	uint8_t g_rcvDataBuf[MAX_BUF_SIZE];
	uint8_t received_data_len;
} STR_RCV_DMA_que_data;


/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */














void HAL_USARTx_DMA_RxCpltCallback(void);
void HAL_USARTx_DMA_TxCpltCallback(void);
void HAL_USARTx_DMA_ErrorCallback(void);

void USART_RX_DMA_Config(UART_HandleTypeDef* uartHandle);
void USART_RX_DMA_Config(UART_HandleTypeDef* uartHandle);
void Usart_SendFUN_ALL(void);


void TEST_USART_RX_DMA_SEGGERprint(void);

void Usart_SendDMA_SaveFun(char *Sendbuff,uint16_t buff_len);

void USART_TX_RX_DMA_Config(UART_HandleTypeDef* uartHandle);
void USART_TX_RX_DMA_ConfigALL(void);


#if TESTUsartDMASendSaveAndSend
void Test_Usart_DMA_SendSaveFun(void);
void TEST_Usart_SendFUN_ALL(void);
#endif

#if TEST_DMA_RB_READ
void TEST_USART_RX_DMA_ALL(void);
void TEST_USART_RX_DMA_ALL_while(void);
#endif

#if TEST_RingBuffer_SAVEANDREAD
void Test_Usart_QueueSaveFun(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */

