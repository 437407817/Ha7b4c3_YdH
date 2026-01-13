 /**
  ******************************************************************************
  * @file    bsp_spi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./spi/bsp_spi.h"

SPI_HandleTypeDef spi_a;





uint8_t MX_SPI_A_Init(void)
{
   /*##-1- Configure the SPI peripheral #######################################*/
  /* Set the SPI parameters */
  spi_a.Instance               = SPI_a;
  spi_a.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  spi_a.Init.Direction         = SPI_DIRECTION_2LINES;
  spi_a.Init.CLKPhase          = SPI_PHASE_2EDGE;
  spi_a.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  spi_a.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  spi_a.Init.CRCPolynomial     = 7;
  spi_a.Init.DataSize          = SPI_DATASIZE_8BIT;
  spi_a.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  spi_a.Init.NSS               = SPI_NSS_SOFT;
  spi_a.Init.TIMode            = SPI_TIMODE_DISABLE;
  
  spi_a.Init.Mode = SPI_MODE_MASTER;

  if(HAL_SPI_Init(&spi_a)!=HAL_OK){
	return STATE_SPI_ERROR;
	
	}; 
  
	
	
  __HAL_SPI_ENABLE(&spi_a); 

//  /* 使 SPI_FLASH 进入 4 字节地址模式 */
//	SPI_a_FLASH_Mode_Init();  
 return STATE_SPI_OK;
}



/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
    if(spiHandle->Instance==SPI_a)
  {
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SPI_a_SCK_GPIO_CLK_ENABLE();
  SPI_a_MISO_GPIO_CLK_ENABLE();
  SPI_a_MOSI_GPIO_CLK_ENABLE();
  SPI_a_CS_GPIO_CLK_ENABLE();
  /* Enable SPI clock */
  SPI_a_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPI_a_SCK_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = SPI_a_SCK_AF;
  
  HAL_GPIO_Init(SPI_a_SCK_GPIO_PORT, &GPIO_InitStruct);
    
  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPI_a_MISO_PIN;
  GPIO_InitStruct.Alternate = SPI_a_MISO_AF;
  
  HAL_GPIO_Init(SPI_a_MISO_GPIO_PORT, &GPIO_InitStruct);
  
  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPI_a_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPI_a_MOSI_AF;  
  HAL_GPIO_Init(SPI_a_MOSI_GPIO_PORT, &GPIO_InitStruct);   

  GPIO_InitStruct.Pin = FLASH_CS_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStruct); 
	}
}




/**
 * @brief   发送一个字节
 * @param   无
 * @retval  无
 */
void SPI_FLASH_SendByte(uint8_t byte)
{
    HAL_SPI_Transmit(&spi_a, &byte, 1, 500);
}
 
/**
 * @brief   发送n个字节
 * @param   pData：发送数据首地址
 * @param   data_number：发送数据个数（以字节为单位）
 * @retval  无
 */
void SPI_FLASH_SendnByte(uint8_t *pData, uint32_t data_number)
{
    HAL_SPI_Transmit(&spi_a, pData, data_number, 500);
}
 

/**
 * @brief   读取一个字节
 * @param   无
 * @retval  接收的数据
 */
uint8_t SPI_FLASH_ReadByte(void)
{
    uint8_t rxData = 0;
    HAL_SPI_Receive(&spi_a, &rxData, 1, 500);
    return rxData;
}

/**
 * @brief   接收n个字节
 * @param   pData：接收数据首地址
 * @param   data_number：接收数据个数（以字节为单位）
 * @retval  无
 */
void SPI_FLASH_ReadnByte(uint8_t *pData, uint32_t data_number)
{
    HAL_SPI_Receive(&spi_a, pData, data_number, 500);
}
 








   
/*********************************************END OF FILE**********************/
