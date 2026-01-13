#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32fxxx.h"
#include <stdio.h>




#define SPI_a                             SPI5
#define SPI_a_CLK_ENABLE()                __HAL_RCC_SPI5_CLK_ENABLE()
#define SPI_a_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define SPI_a_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE() 
#define SPI_a_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE() 
#define SPI_a_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE() 

#define SPI_a_FORCE_RESET()               __HAL_RCC_SPI5_FORCE_RESET()
#define SPI_a_RELEASE_RESET()             __HAL_RCC_SPI5_RELEASE_RESET()

/* Definition for SPI_a Pins */
#define SPI_a_SCK_PIN                     GPIO_PIN_7
#define SPI_a_SCK_GPIO_PORT               GPIOF
#define SPI_a_SCK_AF                      GPIO_AF5_SPI5
#define SPI_a_MISO_PIN                    GPIO_PIN_8
#define SPI_a_MISO_GPIO_PORT              GPIOF
#define SPI_a_MISO_AF                     GPIO_AF5_SPI5
#define SPI_a_MOSI_PIN                    GPIO_PIN_9
#define SPI_a_MOSI_GPIO_PORT              GPIOF
#define SPI_a_MOSI_AF                     GPIO_AF5_SPI5

#define FLASH_CS_PIN                     GPIO_PIN_6               
#define FLASH_CS_GPIO_PORT               GPIOF                     



#define SPI_a_FLASH_CS_LOW()      HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_RESET)
#define SPI_a_FLASH_CS_HIGH()     HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_SET)
/*SPI接口定义-结尾****************************/

///*等待超时时间*/
//#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
//#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define   STATE_SPI_OK         ((uint8_t)0x00)
#define   STATE_SPI_ERROR      ((uint8_t)0x01)
/*信息输出*/
extern SPI_HandleTypeDef spi_a;


uint8_t MX_SPI_A_Init(void);



#define Flash_FATFS_Init     MX_SPI_A_Init







void SPI_FLASH_SendByte(uint8_t byte);
void SPI_FLASH_SendnByte(uint8_t *pData, uint32_t data_number);
uint8_t SPI_FLASH_ReadByte(void);
void SPI_FLASH_ReadnByte(uint8_t *pData, uint32_t data_number);







#endif /* __SPI_FLASH_H */

