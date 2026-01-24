#ifndef __BSP_USART_SHELL_H
#define	__BSP_USART_SHELL_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./user_config.h"

/* COM define */
#define USE_COM_LOG                         0U
#define USE_BSP_COM_FEATURE                 1U



//串口波特率
#define USART_SHELL_BAUDRATE                    115200



/************************ USART1 配置（替换原USART6） ************************/
#define USART_SHELL                             USART1                  // 外设名称改为USART1
#define USART_SHELL_CLK_ENABLE()                __USART1_CLK_ENABLE();  // USART1 外设时钟使能

// USART1 RX引脚：PA10（复用AF7）
#define USART_SHELL_RX_GPIO_PORT                GPIOA                   // RX引脚端口：GPIOA
#define USART_SHELL_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()    // GPIOA 时钟使能
#define USART_SHELL_RX_PIN                      GPIO_PIN_10             // RX引脚：PA10
#define USART_SHELL_RX_AF                       GPIO_AF7_USART1         // RX引脚复用功能：AF7（F429 USART1固定AF7）

// USART1 TX引脚：PA9（复用AF7）
#define USART_SHELL_TX_GPIO_PORT                GPIOA                   // TX引脚端口：GPIOA
#define USART_SHELL_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()    // GPIOA 时钟使能
#define USART_SHELL_TX_PIN                      GPIO_PIN_9              // TX引脚：PA9
#define USART_SHELL_TX_AF                       GPIO_AF7_USART1         // TX引脚复用功能：AF7

// USART1 中断配置
#define USART_SHELL_IRQHandler                  USART1_IRQHandler       // 中断服务函数名
#define USART_SHELL_IRQ                         USART1_IRQn             // 中断向量号












#if 0
//引脚定义
/*******************************************************/
#define USART_SHELL                             USART6
#define USART_SHELL_CLK_ENABLE()                __USART6_CLK_ENABLE();
			  
#define USART_SHELL_RX_GPIO_PORT                GPIOC
#define USART_SHELL_RX_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define USART_SHELL_RX_PIN                      GPIO_PIN_7
#define USART_SHELL_RX_AF                       GPIO_AF8_USART6
			  
#define USART_SHELL_TX_GPIO_PORT                GPIOC
#define USART_SHELL_TX_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define USART_SHELL_TX_PIN                      GPIO_PIN_6
#define USART_SHELL_TX_AF                       GPIO_AF8_USART6
			  
#define USART_SHELL_IRQHandler                  USART6_IRQHandler
#define USART_SHELL_IRQ                 		    USART6_IRQn
/************************************************************/
//DMA
//#define SENDBUFF_SIZE                     		10//发送的数据量
//#define DEBUG_USART_SHELL_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	
//#define DEBUG_USART_SHELL_DMA_CHANNEL           		DMA_CHANNEL_4
//#define DEBUG_USART_SHELL_DMA_STREAM            		DMA2_Stream7
#endif
/************************ USART2 配置（PD5 TX / PD6 RX） ************************/
//#define USART_SHELL                             USART2                  // 外设名称改为USART2
//#define USART_SHELL_CLK_ENABLE()                __USART2_CLK_ENABLE();  // USART2 外设时钟使能

//// USART2 RX 引脚：PD6（复用AF7）
//#define USART_SHELL_RX_GPIO_PORT                GPIOD                   // RX引脚端口：GPIOD
//#define USART_SHELL_RX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()    // GPIOD 时钟使能
//#define USART_SHELL_RX_PIN                      GPIO_PIN_6              // RX引脚：PD6
//#define USART_SHELL_RX_AF                       GPIO_AF7_USART2         // RX引脚复用功能：AF7（F429 USART2 固定AF7）

//// USART2 TX 引脚：PD5（复用AF7）
//#define USART_SHELL_TX_GPIO_PORT                GPIOD                   // TX引脚端口：GPIOD
//#define USART_SHELL_TX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()    // GPIOD 时钟使能
//#define USART_SHELL_TX_PIN                      GPIO_PIN_5              // TX引脚：PD5
//#define USART_SHELL_TX_AF                       GPIO_AF7_USART2         // TX引脚复用功能：AF7

//// USART2 中断配置
//#define USART_SHELL_IRQHandler                  USART2_IRQHandler       // 中断服务函数名
//#define USART_SHELL_IRQ                         USART2_IRQn             // 中断向量号
//DMA
//#define SENDBUFF_SIZE                     		10//发送的数据量
//#define DEBUG_USART_SHELL_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	
//#define DEBUG_USART_SHELL_DMA_CHANNEL           		DMA_CHANNEL_4
//#define DEBUG_USART_SHELL_DMA_STREAM            		DMA2_Stream7



extern UART_HandleTypeDef huart_shell_Handle;
//#define  huart_shell  huart_shell_Handle

//#define USE_UART 1
#define TEST_SHELL_UART 0
#define USE_IT_1 0



//void USART_SHELL_Config(void);

//void USART_Config_SHELL(void);
void USART_Shell_ComDrvInit(void);

#if TEST_SHELL_UART
 #if !(USE_LETTER_SHELL)
void HAL_UART_Shell_RxCpltCallback(void);
#endif
#endif

void USART_Shell_SendChar(uint8_t ch);
uint8_t USART_Shell_ReceiveChar(void);
void USART_Shell_UartDeInit(void);

void Usart_SendString(uint8_t *str);

void Test_USART_SHELL_while(void);


#endif /* __BSP_USART_A_H */
