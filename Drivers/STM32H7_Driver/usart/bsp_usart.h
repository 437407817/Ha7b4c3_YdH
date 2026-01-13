#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>

//串口波特率
#define USART_A_BAUDRATE                    115200

//引脚定义
/*******************************************************/
#define USART_A                             USART6
#define USART_A_CLK_ENABLE()                __USART6_CLK_ENABLE();

#define USART_A_RX_GPIO_PORT                GPIOC
#define USART_A_RX_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define USART_A_RX_PIN                      GPIO_PIN_7
#define USART_A_RX_AF                       GPIO_AF7_USART6

#define USART_A_TX_GPIO_PORT                GPIOC
#define USART_A_TX_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define USART_A_TX_PIN                      GPIO_PIN_6
#define USART_A_TX_AF                       GPIO_AF7_USART6

#define USART_A_IRQHandler                  USART6_IRQHandler
#define USART_A_IRQ                 		    USART6_IRQn
/************************************************************/
//DMA
#define SENDBUFF_SIZE                     		10//发送的数据量
#define DEBUG_USART_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	
#define DEBUG_USART_DMA_CHANNEL           		DMA_CHANNEL_4
#define DEBUG_USART_DMA_STREAM            		DMA2_Stream7





#define USE_UART 1
#define USE_UART_THIS_HANDLER 0



void USART_A_Config(void);

void USART_Config_A(void);


uint8_t Transfer_USART1_DMA_Send(uint8_t *data_p, uint16_t len);
uint8_t Transfer_USART1_DMA_Send2(uint8_t *data_p, uint16_t len);
HAL_StatusTypeDef DMA_NormalMode_ReStart_WithTimeout(uint8_t *data, uint16_t len, uint32_t timeout_ms);








extern UART_HandleTypeDef huart_a;



// 发送一个字符
void USART_A_SendChar(uint8_t ch);
// 接收一个字符
uint8_t USART_A_ReceiveChar(void);




void Usart_SendString(uint8_t *str);


#endif /* __BSP_USART_A_H */
