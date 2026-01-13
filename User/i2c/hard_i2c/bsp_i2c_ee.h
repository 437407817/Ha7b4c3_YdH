/**
  ******************************************************************************
  * @file    bsp_i2c_ee.h
  * @author  MCD Application Team
  * @version V4.5.1
  * @date    20-September-2021
  * @brief   This file contains all the functions prototypes for the stm32_eval_i2c_ee
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __bsp_I2C_EE_H
#define __bsp_I2C_EE_H

/* Includes ------------------------------------------------------------------*/


#include "stm32_FH_xxx.h"
#include "./i2c/hard_i2c/bsp_iic.h"
#include "./sys/sysio.h"


#if  0  //open or close DMA RX ?开启或关闭
#define EEPROM_IIC_DMA_ENABLE 
#endif


/* AT24C01/02每页有8个字节 */
//#define EEPROM_PAGESIZE    8
#define EEPROM_PAGESIZE 	   8
/* AT24C04/08A/16A每页有16个字节 */
//#define EEPROM_PAGESIZE           16			
/* AT24C32/64A/256A每页有32个字节 */
//#define EEPROM_PAGESIZE           32	


/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#define I2Cx_TIMEOUT_MAX                300
/* Maximum number of trials for HAL_I2C_IsDeviceReady() function */
#define EEPROM_MAX_TRIALS               300

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

#define EEPROM_ADDRESS        0xA0

void I2C_EE_Init(void);
										  
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr);
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);





#define I2C_E_Handle I2C_A_Handle












//-------------------------------
#if 0

//#define sEE_M24C02
//#define  sEE_M24C08
//24lc256
//#define sEE_M24C64_32
#define sEE_M24C256



#if defined (sEE_M24C08)  || defined (sEE_M24C02)

#define SINGLE_BYTE_ADDRESS

#elif   defined (sEE_M24C64_32)  || defined (sEE_M24C256)
#define DOUBLE_BYTE_ADDRESS
#endif


#if !defined (sEE_M24C08) && !defined (sEE_M24C64_32)  && !defined (sEE_M24C02) && !defined (sEE_M24C256)
/* Use the defines below the choose the EEPROM type */
/* #define sEE_M24C08*/  /* Support the device: M24C08. */
/* note: Could support: M24C01, M24C02, M24C04 and M24C16 if the blocks and 
   HW address are correctly defined*/
#define sEE_M24C02  /* Support the devices: M24C32 and M24C64 */
#endif


#if defined (sEE_M24C64_32)  || defined (sEE_M24C256)
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
  address selection (ne need for additional address lines). According to the 
  Harware connection on the board (on STM3210C-EVAL board E0 = E1 = E2 = 0) */

 #define sEE_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 

#elif defined (sEE_M24C08) || defined (sEE_M24C02)
/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 
   EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */




#endif /* sEE_M24C64_32 */

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */


/* AT24C01/02每页有8个字节 */
/* AT24C04/08A/16A每页有16个字节 */

#if defined (sEE_M24C02)
 #define sEE_PAGESIZE           8
#elif defined (sEE_M24C08)
 #define sEE_PAGESIZE           16
#elif defined (sEE_M24C64_32)
 #define sEE_PAGESIZE           32
#elif defined (sEE_M24C256)
 #define sEE_PAGESIZE           32 
#endif

/* STM32 I2C 快速模式 */
#define I2C_Speed              IIC1_Speed

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS7      IIC1_OWN_ADDRESS7   

///*I2C接口*/
//#define sEE_I2C                          I2C1
//#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
//#define sEE_I2C_CLK_INIT								RCC_APB2PeriphClockCmd

//#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                 
//#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       
//#define sEE_I2C_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
//#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
//#define sEE_I2C_SCL_AF                   GPIO_AF_I2C1

//#define sEE_I2C_SDA_PIN                  GPIO_Pin_7                  
//#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       
//#define sEE_I2C_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
//#define sEE_I2C_SDA_SOURCE               GPIO_PinSource7
//#define sEE_I2C_SDA_AF                   GPIO_AF_I2C1


/*I2C接口*/
#define sEE_I2C                          IIC1
#define sEE_I2C_CLK                      IIC1_CLK
#define sEE_I2C_CLK_INIT								IIC1_CLK_INIT

#define sEE_I2C_SCL_PIN                  IIC1_SCL_PIN                 
#define sEE_I2C_SCL_GPIO_PORT            IIC1_SCL_GPIO_PORT                       
#define sEE_I2C_SCL_GPIO_CLK             IIC1_SCL_GPIO_CLK
#define sEE_I2C_SCL_SOURCE               IIC1_SCL_SOURCE
#define sEE_I2C_SCL_AF                   IIC1_SCL_AF

#define sEE_I2C_SDA_PIN                  IIC1_SDA_PIN                  
#define sEE_I2C_SDA_GPIO_PORT            IIC1_SDA_GPIO_PORT                       
#define sEE_I2C_SDA_GPIO_CLK             IIC1_SDA_GPIO_CLK
#define sEE_I2C_SDA_SOURCE               IIC1_SDA_SOURCE
#define sEE_I2C_SDA_AF                   IIC1_SDA_AF

#if defined (EEPROM_IIC_DMA_ENABLE) 

#define sEE_I2C_DMA                      DMA1   
#define sEE_I2C_DMA_CHANNEL_TX           DMA1_Channel6
#define sEE_I2C_DMA_CHANNEL_RX           DMA1_Channel7 
#define sEE_I2C_DMA_FLAG_TX_TC           DMA1_IT_TC6   
#define sEE_I2C_DMA_FLAG_TX_GL           DMA1_IT_GL6 
#define sEE_I2C_DMA_FLAG_RX_TC           DMA1_IT_TC7 
#define sEE_I2C_DMA_FLAG_RX_GL           DMA1_IT_GL7    
#define sEE_I2C_DMA_CLK                  RCC_AHBPeriph_DMA1
#define sEE_I2C_DR_Address               ((uint32_t)0x40005410)
#define sEE_USE_DMA


   
#define sEE_I2C_DMA_TX_IRQn              DMA1_Channel6_IRQn
#define sEE_I2C_DMA_RX_IRQn              DMA1_Channel7_IRQn
#define sEE_I2C_DMA_TX_IRQHandler        DMA1_Channel6_IRQHandler
#define sEE_I2C_DMA_RX_IRQHandler        DMA1_Channel7_IRQHandler   
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0   
 
 
#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1   


#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 






/*等待超时时间*/
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))

/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     150
   







void I2C_EE_Init(void);

uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);

uint32_t sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, u16 NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction);

uint32_t sEE_TIMEOUT_UserCallback(uint8_t errorCode);
//uint32_t sEE_ReadBuffer(u8* pBuffer, u8 ReadAddr, uint16_t NumByteToRead);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);


#ifdef USE_DEFAULT_CRITICAL_CALLBACK
void sEE_EnterCriticalSection_UserCallback(void);
void sEE_ExitCriticalSection_UserCallback(void);



#endif /* USE_DEFAULT_CRITICAL_CALLBACK */







#endif /* __I2C_EE_H */
#endif 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

