#ifndef __BSP_COM_UART_H
#define	__BSP_COM_UART_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>


#define USE_COM_LOG                         0U
#define USE_BSP_COM_FEATURE                 1U




/* Common Error codes */
#define BSP_ERROR_NONE                    0
#define BSP_ERROR_NO_INIT                -1
#define BSP_ERROR_WRONG_PARAM            -2
#define BSP_ERROR_BUSY                   -3
#define BSP_ERROR_PERIPH_FAILURE         -4
#define BSP_ERROR_COMPONENT_FAILURE      -5
#define BSP_ERROR_UNKNOWN_FAILURE        -6
#define BSP_ERROR_UNKNOWN_COMPONENT      -7 
#define BSP_ERROR_BUS_FAILURE            -8 
#define BSP_ERROR_CLOCK_FAILURE          -9  
#define BSP_ERROR_MSP_FAILURE            -10  
#define BSP_ERROR_FEATURE_NOT_SUPPORTED  -11



#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COMn
}COM_TypeDef;

typedef enum
{
 COM_STOPBITS_1     =   UART_STOPBITS_1,
 COM_STOPBITS_2     =   UART_STOPBITS_2,
}COM_StopBitsTypeDef;

typedef enum
{
 COM_PARITY_NONE     =  UART_PARITY_NONE,
 COM_PARITY_EVEN     =  UART_PARITY_EVEN,
 COM_PARITY_ODD      =  UART_PARITY_ODD,
}COM_ParityTypeDef;

typedef enum
{
 COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
 COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
 COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
 COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
}COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
}COM_WordLengthTypeDef;

typedef struct
{
  uint32_t               BaudRate;
  COM_WordLengthTypeDef  WordLength;
  COM_StopBitsTypeDef    StopBits;
  COM_ParityTypeDef      Parity;
  COM_HwFlowCtlTypeDef   HwFlowCtl;
}COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
}BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */

#define MX_UART_InitTypeDef COM_InitTypeDef
#endif


/**
 * @brief Definition for COM port1, connected to USART3
 */
#if (USE_BSP_COM_FEATURE > 0)

#define COMn                          1U
#define COM1_UART                     USART1
#define COM1_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()

#define COM1_TX_PIN                   GPIO_PIN_9
#define COM1_TX_GPIO_PORT             GPIOA
#define COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_TX_AF                    GPIO_AF7_USART1

#define COM1_RX_PIN                   GPIO_PIN_10
#define COM1_RX_GPIO_PORT             GPIOA
#define COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_RX_AF                    GPIO_AF7_USART1
#define COM_POLL_TIMEOUT             1000
#endif

#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
extern USART_TypeDef* COM_USART[];



__weak HAL_StatusTypeDef MX_USARTx_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);

void BSP_COM_UART_INIT(void);
#endif














#endif 



