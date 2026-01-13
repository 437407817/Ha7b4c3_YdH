/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  MCD Application Team
  * @version V4.5.1
  * @date    20-September-2021
  * @brief   This file provides a set of functions needed to manage the I2C M24CXX 
  *          EEPROM memory mounted on STM32xx-EVAL board (refer to stm32_eval.h
  *          to know about the boards supporting this memory). 
  *          
  *          ===================================================================      
  *          Note: This driver is intended for STM32F10x families devices only.
  *          ===================================================================
  *            
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (I2C and 
  *          GPIO) are defined in stm32xx_eval.h file, and the initialization is 
  *          performed in sEE_LowLevel_Init() function declared in stm32xx_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          sEE_LowLevel_Init() function. 
  *        
  *          @note In this driver, basic read and write functions (sEE_ReadBuffer() 
  *                and sEE_WritePage()) use the DMA to perform the data transfer 
  *                to/from EEPROM memory (except when number of requested data is
  *                equal to 1). Thus, after calling these two functions, user 
  *                application may perform other tasks while DMA is transferring
  *                data. The application should then monitor the variable holding 
  *                the number of data in order to determine when the transfer is
  *                completed (variable decremented to 0). Stopping transfer tasks
  *                are performed into DMA interrupt handlers (which are integrated
  *                into this driver).
  *            
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |                 
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32 I2C Pins                       |   sEE     |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | .                                     |   E0(GND) |    1  (0V)  |
  *     | .                                     |   E1(GND) |    2  (0V)  |
  *     | .                                     |   E2(GND) |    3  (0V)  |
  *     | .                                     |   E0(VSS) |    4  (0V)  |
  *     | sEE_I2C_SDA_PIN/ SDA                  |   SDA     |    5        |
  *     | sEE_I2C_SCL_PIN/ SCL                  |   SCL     |    6        |
  *     | .                                     |   /WC(VDD)|    7 (3.3V) |
  *     | .                                     |   VDD     |    8 (3.3V) |
  *     +---------------------------------------+-----------+-------------+  
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

/* Includes ------------------------------------------------------------------*/
#include "./i2c/hard_i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"
#include "stm32f10x_dma.h"


/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */
  
/** @addtogroup STM32_I2C_EE
  * @brief      This file includes the I2C EEPROM driver of STM32-EVAL boards.
  * @{
  */ 

/** @defgroup STM32_I2C_EE_Private_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_I2C_EE_Private_Defines
  * @{
  */  
/**
  * @}
  */ 


/** @defgroup STM32_I2C_EE_Private_Macros
  * @{
  */
/**
  * @}
  */ 


/** @defgroup STM32_I2C_EE_Private_Variables
  * @{
  */
__IO uint16_t EEPROM_ADDRESS= 0;

__IO uint8_t   sEEDataNum;
__IO uint8_t*  sEEDataWritePointer;  
__IO uint16_t* sEEDataReadPointer;   



static __IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;   




DMA_InitTypeDef   sEEDMA_InitStructure; 

/**
  * @brief  Initializes peripherals 
  * @param  ?
  * @retval ?
  */
static void I2C_GPIO_Config(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure; 

  
  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
 
  /*!< Configure sEE_I2C pins: SCL */   
   /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  /*!< sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
    
  /*!< GPIO configuration */  
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 

}
#ifdef EEPROM_IIC_DMA_ENABLE 
void sEE_DMA_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;    
	 /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);  
  
  /*!< I2C DMA TX and RX channels configuration */
  /* Enable the DMA clock */
  RCC_AHBPeriphClockCmd(sEE_I2C_DMA_CLK, ENABLE);

  /* I2C TX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  sEEDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)sEE_I2C_DR_Address;
  sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  sEEDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(sEE_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  
  /* I2C RX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
  DMA_Init(sEE_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);  
  
  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_RX, DMA_IT_TC, ENABLE);    

}
#endif	/*!< EEPROM_IIC_DMA_ENABLE */

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		                    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;	                            /* 通信速率 */
  I2C_Init(sEE_I2C, &I2C_InitStructure);	                                      /* I2C1 初始化 */
  I2C_Cmd(sEE_I2C, ENABLE);  	                                                /* 使能 I2C1 */

  I2C_AcknowledgeConfig(sEE_I2C, ENABLE);  
	
	
#if defined (EEPROM_IIC_DMA_ENABLE)
	  /* Enable the sEE_I2C peripheral DMA requests */
  I2C_DMACmd(sEE_I2C, ENABLE);
#endif   /* EEPROM_IIC_DMA_ENABLE */
}











/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)
{
  I2C_GPIO_Config(); 
 #if defined (EEPROM_IIC_DMA_ENABLE)
	  /* Enable the sEE_I2C peripheral DMA requests */
  sEE_DMA_Init();
#endif   /* EEPROM_IIC_DMA_ENABLE */
  I2C_Mode_Configu();

#if defined (DOUBLE_BYTE_ADDRESS)
  /*!< Select the EEPROM address according to the state of E0, E1, E2 pins */
  EEPROM_ADDRESS = sEE_HW_ADDRESS;  
#elif defined (SINGLE_BYTE_ADDRESS) 
/* 根据头文件i2c_ee.h中的定义来选择EEPROM要写入的地址 */
#ifdef EEPROM_Block0_ADDRESS
  /* 选择 EEPROM Block0 来写入 */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* 选择 EEPROM Block1 来写入 */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* 选择 EEPROM Block2 来写入 */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* 选择 EEPROM Block3 来写入 */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
#endif /*!< DOUBLE_BYTE_ADDRESS */ 
}



/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg ReadAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */


uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{  
	
	  sEEDataReadPointer = NumByteToRead;
	
    sEETimeout = sEE_LONG_TIMEOUT;

  //*((uint8_t *)0x4001080c) |=0x80; 
	
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))   
    {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(39);
    }
	
  /* Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  //*((uint8_t *)0x4001080c) &=~0x80;
  
  sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(10);
   }

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  sEETimeout = sEE_FLAG_TIMEOUT;
 
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(11);
   }
	//	#ifdef SINGLE_BYTE_ADDRESS 
	 
  /* Clear EV6 by setting again the PE bit */// I2C_Cmd(sEE_I2C, ENABLE);

 

#if defined (SINGLE_BYTE_ADDRESS)  
  
  /*!< Send the EEPROM's internal address to read from: Only one byte address */
  I2C_SendData(sEE_I2C, ReadAddr);  

#elif defined (DOUBLE_BYTE_ADDRESS)

  /*!< Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));    

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(112);
  }

  /*!< Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));    
  
#endif /*!< SINGLE_BYTE_ADDRESS */
	   /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(113);
  }
	 
	 #if 0
	 	 /* Send the EEPROM's internal address to write to */
  I2C_SendData(sEE_I2C, ReadAddr);  

		//	#endif /*!< SINGLE_BYTE_ADDRESS */ 
	 
	 
	   /*!< Test on EV8 and clear it */
     sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
	// 
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	// while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
    {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(12);
   }
	 
	 
	 #endif
	 
	 
	 
	 
	 
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(sEE_I2C, ENABLE);
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */

     sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(13);
   }
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(sEE_I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);
 
#ifdef EEPROM_IIC_DMA_ENABLE  
	 
	  /* If number of data to be read is 1, then DMA couldn't be used */
  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
  if ((uint16_t)(*NumByteToRead) < 2)
  {
    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(14);
    }     
    
    /*!< Disable Acknowledgement */
    I2C_AcknowledgeConfig(sEE_I2C, DISABLE);   

    /* Call User callback for critical section start (should typically disable interrupts) */
    sEE_EnterCriticalSection_UserCallback();
    
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)sEE_I2C->SR2;
    
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
   
    /* Call User callback for critical section end (should typically re-enable interrupts) */
    sEE_ExitCriticalSection_UserCallback();
    
    /* Wait for the byte to be received */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(15);
    }
    
    /*!< Read the byte received from the EEPROM */
    *pBuffer = I2C_ReceiveData(sEE_I2C);
    
    /*!< Decrement the read bytes counter */
    (uint16_t)(*NumByteToRead)--;        
    
    /* Wait to make sure that STOP control bit has been cleared */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(sEE_I2C->CR1 & I2C_CR1_STOP)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(16);
    }  
    
    /*!< Re-Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE);    
  }
  else/* More than one Byte Master Reception procedure (DMA) -----------------*/
 {
    /*!< Test on EV6 and clear it */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(17);
    }  
    
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), sEE_DIRECTION_RX);
    
    /* Inform the DMA that the next End Of Transfer Signal will be the last one */
    I2C_DMALastTransferCmd(sEE_I2C, ENABLE); 
    
    /* Enable the DMA Rx Channel */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, ENABLE);  
  }
	 
	 
	 
#else	 
	
     sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(14);
   }
  /* While there is data to be read */
  while(*NumByteToRead)  
  {
    if((*NumByteToRead) == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(sEE_I2C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
    }

		
		sEETimeout = sEE_LONG_TIMEOUT;
		while(I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(3);
		} 	
		{
		  /* Read a byte from the device */
      *pBuffer = I2C_ReceiveData(sEE_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      (*NumByteToRead)--;
		}			

  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(sEE_I2C, ENABLE);
 #endif 
  return 0;
}


/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{
	  /* Set the pointer to the Number of data to be written. This pointer will be used 
      by the DMA Transfer Completer interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been complete or not. */
  sEEDataWritePointer = NumByteToWrite;  
	/*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))  
   {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(140);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
	
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(5);
  } 
  
  /* Send EEPROM address for write */
	sEETimeout = sEE_FLAG_TIMEOUT;
  I2C_Send7bitAddress(sEE_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  

  /* Test on EV6 and clear it */
	sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(6);
  } 
  /* Send the EEPROM's internal address to write to */  

	
#if defined (SINGLE_BYTE_ADDRESS) 
  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(DOUBLE_BYTE_ADDRESS)
  
  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;  
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(78);
  }  
  
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< SINGLE_BYTE_ADDRESS */  

 /*!< Test on EV8 and clear it */
sEETimeout = sEE_FLAG_TIMEOUT;

  while(! I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(7);
  } 



	#if 0
	 I2C_SendData(sEE_I2C, WriteAddr);  

  sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(7);
  } 
  /* While there is data to be written */
	
	#endif
#ifdef EEPROM_IIC_DMA_ENABLE 

  /* Configure the DMA Tx Channel with the buffer address and the buffer size */
 
	 sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);
    
  /* Enable the DMA Tx Channel */
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, ENABLE);
#else

  while((*NumByteToWrite)--)  
  {
    /* Send the current byte */
    I2C_SendData(sEE_I2C, *pBuffer); 
		
  //printf("+=0x%02X ", *pBuffer);//写入数据展示，如果在调试过程中查看写入哪些数据，在这里展开
    /* Point to the next byte to be written */
    pBuffer++; 

    sEETimeout = sEE_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(8);
    } 
  }
	  /* Send STOP condition */
  I2C_GenerateSTOP(sEE_I2C, ENABLE);

#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 
	
  return 1;
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, u16 NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0,  count = 0;
  uint16_t Addr = 0;
	
  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;   //本页剩余空间
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
  /* If WriteAddr is sEE_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage == 0) 
    {
			/* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
    #ifdef EEPROM_IIC_DMA_ENABLE     
			/* Wait transfer through DMA to be complete */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(10); return 1;};
      }      
			#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 
			sEE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > sEE_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
			 /* Store the number of data to be written */
      sEEDataNum = sEE_PAGESIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
#ifdef EEPROM_IIC_DMA_ENABLE 
				/* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
				 while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(11); return;};
        }   
#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
				/* Store the number of data to be written */
        sEEDataNum = NumOfSingle;    
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#ifdef EEPROM_IIC_DMA_ENABLE 				
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
				 while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(12); return;};
        }  				
#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 			
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not sEE_PAGESIZE aligned  */
  else 
  {
    /* If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage== 0) 
    {
			      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
#if 	1		//xxxxxxxxxxxxxxxxxxx官方代码出差异点，可屏蔽xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx			
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
      sEEDataNum = count;        
        /*!< Write the data conained in same page */ 			
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#ifdef EEPROM_IIC_DMA_ENABLE 
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(13); return;};
        }          
#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 						
        sEE_WaitEepromStandbyState();
				//从不满页中写一个不满页数据，但是超了本页
        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);          
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
#ifdef EEPROM_IIC_DMA_ENABLE        
				/* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(14); return;};
        } 
#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 				
        sEE_WaitEepromStandbyState();   	
       }	
			else
#endif
			{
				//从不满页中写一个不满页数据，但是没超了本页
      /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
				
#ifdef EEPROM_IIC_DMA_ENABLE        
				/* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(15); return;};
        }      
#endif	/*!< EEPROM_IIC_DMA_ENABLE */ 	
				
        sEE_WaitEepromStandbyState();        
			 
			 }					
    }
    /* If NumByteToWrite > sEE_PAGESIZE */
    else
    {
			
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;	
      
      if(count != 0)
      {  
				/* Store the number of data to be written */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#ifdef EEPROM_IIC_DMA_ENABLE 
				/* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(16); return;};
        } 
#endif	/*!< EEPROM_IIC_DMA_ENABLE */				
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
       /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#ifdef EEPROM_IIC_DMA_ENABLE 
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(17); return;};
        }        
#endif	/*!< EEPROM_IIC_DMA_ENABLE */	  
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
     /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;           
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
#ifdef EEPROM_IIC_DMA_ENABLE 
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(18); return;};
        }         
#endif	/*!< EEPROM_IIC_DMA_ENABLE */	
        sEE_WaitEepromStandbyState();
      }
    }
  }  
	return 0;
}

/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */














 uint32_t sEE_WaitEepromStandbyState(void)      
{

	__IO uint16_t tmpSR1 = 0;
__IO uint32_t sEETrials = 0;
	  /*!< While the bus is busy */
	 sEETimeout = sEE_LONG_TIMEOUT;

   /*!< While the bus is busy */

  
#ifdef EEPROM_IIC_DMA_ENABLE 
	while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
#else
while(! I2C_CheckEvent(sEE_I2C, I2C_FLAG_BUSY)) 
#endif	
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(19);
  } 
	 
	 /* Keep looping till the slave acknowledge his address or maximum number 
     of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
     in stm32_eval_i2c_ee.h file) */
	 while (1)
  {
		/* Send START condition */
    I2C_GenerateSTART(sEE_I2C, ENABLE);
		 
	 sEETimeout = sEE_FLAG_TIMEOUT;

    /*!< Test on EV5 and clear it */
  while(! I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT)) 
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(200);
  } 
	
	/* Send EEPROM address for write */
    I2C_Send7bitAddress(sEE_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* Wait for ADDR flag to be set (Slave acknowledged his address) */
 	 sEETimeout = sEE_LONG_TIMEOUT;
	 do
    {     
      /* Get the current value of the SR1 register */
      tmpSR1 = sEE_I2C->SR1;
      
      /* Update the timeout value and exit if it reach 0 */
    //  if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(29);
    }
    /* Keep looping till the Address is acknowledged or the AF flag is 
       set (address not acknowledged at time) */
    while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
	
	 /* Check if the ADDR flag has been set */
	 


		if (tmpSR1 & I2C_SR1_ADDR)
    {
      /* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already 
         been read) */
      (void)sEE_I2C->SR2;
      
      /*!< STOP condition */    
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
        
      /* Exit the function */
      return 1;
    }
    else
    {
      /*!< Clear AF flag */
      I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);                  
    }

	    
    /* Check if the maximum allowed numbe of trials has bee reached */
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return sEE_TIMEOUT_UserCallback(12);
    }

	}
return 0;
}


/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(0);
  }    

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  
  sEETimeout = sEE_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(1);
  }    
 


#if 0
	
	  /* Send the EEPROM's internal address to write to */
  I2C_SendData(sEE_I2C, WriteAddr);
  
  sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(2);
  } 
	
	#endif
	
	
	#if defined (SINGLE_BYTE_ADDRESS)
  
  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(DOUBLE_BYTE_ADDRESS)
  
  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;  
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(78);
  }  
  
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< SINGLE_BYTE_ADDRESS */  
	
	  sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(2);
  } 
	
	
	
	
	
	
	
	
	
  /* Send the byte to be written */
  I2C_SendData(sEE_I2C, *pBuffer); 
   
  sEETimeout = sEE_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(sEE_I2C, ENABLE);
  
  return 0;
}



#ifdef EEPROM_IIC_DMA_ENABLE 
/**
  * @brief  Initializes DMA channel used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{ 
  /* Initialize the DMA with the new parameters */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;  
    DMA_Init(sEE_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  }
  else
  { 
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;      
    DMA_Init(sEE_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);    
  }
}
#endif	/*!< EEPROM_IIC_DMA_ENABLE */
/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
  uint32_t sEE_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  SYSTEM_ERROR("I2C timeout!errorCode = %d ",errorCode);
  
  return 1;
}




#if defined (EEPROM_IIC_DMA_ENABLE)

/**
  * @brief  This function handles the DMA Tx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void sEE_I2C_DMA_TX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */ 
  if(DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_TX_TC) != RESET)
  {  
    /* Disable the DMA Tx Channel and Clear all its Flags */  
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_TX_GL);

    /*!< Wait till all data have been physically transferred on the bus */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) sEE_TIMEOUT_UserCallback(19);
    }
    
    /*!< Send STOP condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    /* Perform a read on SR1 and SR2 register to clear eventualaly pending flags */
    (void)sEE_I2C->SR1;
    (void)sEE_I2C->SR2;
    
    /* Reset the variable holding the number of data to be written */
    *sEEDataWritePointer = 0; 
  }
}

/**
  * @brief  This function handles the DMA Rx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void sEE_I2C_DMA_RX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_RX_TC) != RESET)
  {      
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);    
    
    /* Disable the DMA Rx Channel and Clear all its Flags */  
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_RX_GL);
    
    /* Reset the variable holding the number of data to be read */
    *sEEDataReadPointer = 0;
  }
}
#endif   /* EEPROM_IIC_DMA_ENABLE */










#ifdef USE_DEFAULT_CRITICAL_CALLBACK
/**
  * @brief  Start critical section: these callbacks should be typically used
  *         to disable interrupts when entering a critical section of I2C communication
  *         You may use default callbacks provided into this driver by uncommenting the 
  *         define USE_DEFAULT_CRITICAL_CALLBACK.
  *         Or you can comment that line and implement these callbacks into your 
  *         application.
  * @param  None.
  * @retval None.
  */
void sEE_EnterCriticalSection_UserCallback(void)
{
  __disable_irq();  
}

/**
  * @brief  Start and End of critical section: these callbacks should be typically used
  *         to re-enable interrupts when exiting a critical section of I2C communication
  *         You may use default callbacks provided into this driver by uncommenting the 
  *         define USE_DEFAULT_CRITICAL_CALLBACK.
  *         Or you can comment that line and implement these callbacks into your 
  *         application.
  * @param  None.
  * @retval None.
  */
void sEE_ExitCriticalSection_UserCallback(void)
{
  __enable_irq();
}

#endif /* USE_DEFAULT_CRITICAL_CALLBACK */

/*********************************************END OF FILE**********************/


