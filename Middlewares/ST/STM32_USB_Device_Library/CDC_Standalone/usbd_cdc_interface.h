/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Inc/usbd_cdc_interface.h
  * @author  MCD Application Team
  * @brief   Header for usbd_cdc_interface.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H
#define __USBD_CDC_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
//#define USARTx_TX_PIN                    GPIO_PIN_9
//#define USARTx_TX_GPIO_PORT              GPIOA
//#define USARTx_TX_AF                     GPIO_AF7_USART1
//#define USARTx_RX_PIN                    GPIO_PIN_10
//#define USARTx_RX_GPIO_PORT              GPIOA
//#define USARTx_RX_AF                     GPIO_AF7_USART1

///* Definition for USARTx's NVIC: used for receiving data over Rx pin */
//#define USARTx_IRQn                      USART1_IRQn
//#define USARTx_IRQHandler                USART1_IRQHandler

///* Definition for USARTx's DMA: used for transmitting data over Tx pin */
//#define USARTx_TX_DMA_CHANNEL            DMA_CHANNEL_4
//#define USARTx_TX_DMA_STREAM             DMA2_Stream7

//#define USARTx_DMA_TX_IRQHandler         DMA2_Stream7_IRQHandler
//#define USARTx_DMA_TX_IRQn               DMA2_Stream7_IRQn

///* Definition for TIMx clock resources */
//#define TIMx                             TIM3
//#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
//#define TIMx_FORCE_RESET()               __HAL_RCC_USART1_FORCE_RESET()
//#define TIMx_RELEASE_RESET()             __HAL_RCC_USART1_RELEASE_RESET()

///* Definition for TIMx's NVIC */
//#define TIMx_IRQn                        TIM3_IRQn
//#define TIMx_IRQHandler                  TIM3_IRQHandler

/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on CDC_POLLING_INTERVAL */
	 
#define USB_USART_REC_LEN       200                         /* USB串口接收缓冲区最大字节数 */

/* 轮询周期，最大65ms，最小1ms */
#define CDC_POLLING_INTERVAL    1                           /* 轮询周期，最大65ms，最小1ms */

extern uint8_t  g_usb_usart_rx_buffer[USB_USART_REC_LEN];   /* 接收缓冲,最大USB_USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usb_usart_rx_sta;                         /* 接收状态标记 */

extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

/******************************************************************************************/

void cdc_vcp_data_tx(uint8_t *buf, uint32_t len);
void cdc_vcp_data_rx(uint8_t *buf, uint32_t len);
void usb_printf(char *fmt,...); 	 
	 
	 
	 
	 
	 
	 
//#define CDC_POLLING_INTERVAL             1 /* in ms. The max is 65 and the min is 1 */

//extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __USBD_CDC_IF_H */
