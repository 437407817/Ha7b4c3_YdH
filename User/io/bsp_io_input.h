#ifndef __BSP_IO_INPUT_H
#define	__BSP_IO_INPUT_H

#include "./stm32_FH_xxx_hal.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOA
#define KEY1_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE();
#define KEY1_INT_GPIO_PIN                 GPIO_PIN_0
#define KEY1_INT_EXTI_IRQ                 EXTI0_IRQn
#define KEY1_IRQHandler                   EXTI0_IRQHandler

#define KEY2_INT_GPIO_PORT                GPIOC
#define KEY2_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE();
#define KEY2_INT_GPIO_PIN                 GPIO_PIN_13
#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define KEY2_IRQHandler                   EXTI15_10_IRQHandler

/*******************************************************/


void IO_EXTI_Key_Config(void);





void IN_Io_ALL_GPIO_Config(void);


#endif /* __BSP_IO_INPUT_H */
