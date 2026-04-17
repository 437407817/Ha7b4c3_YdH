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
  #if 0
#include "./spi/bsp_spi.h"

SPI_HandleTypeDef spi_a;









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
    if(spiHandle->Instance==SPI1)
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





 


#endif





   
/*********************************************END OF FILE**********************/
