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
#include "./spi/bsp_qspi_a.h"





QSPI_HandleTypeDef QSPIHandle;


/*
*********************************************************************************************************
*    函 数 名: HAL_QSPI_MspInit
*    功能说明: QSPI底层初始化，被HAL_QSPI_Init调用的底层函数
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)//下面的引脚初始化，建议直接使用CubeMX生成的代码！！！
{

 GPIO_InitTypeDef GPIO_InitStruct;

  /* QUADSPI clock enable  ，默认120MHz*/
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
	PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  __HAL_RCC_QSPI_CLK_ENABLE();
  /* 复位时钟接口 */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();    
  /* 使能GPIO时钟 */

  QSPI_CS_GPIO_CLK_ENABLE();
  QSPI_CLK_GPIO_CLK_ENABLE();
  QSPI_BK1_D0_GPIO_CLK_ENABLE();
  QSPI_BK1_D1_GPIO_CLK_ENABLE();
  QSPI_BK1_D2_GPIO_CLK_ENABLE();
  QSPI_BK1_D3_GPIO_CLK_ENABLE();


  /* QSPI CS GPIO 引脚配置 */
  GPIO_InitStruct.Pin = QSPI_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//这句代码不能少
  GPIO_InitStruct.Alternate = QSPI_CS_GPIO_AF;//有坑
  HAL_GPIO_Init(QSPI_CS_GPIO_PORT, &GPIO_InitStruct);
  
  /* QSPI CLK GPIO 引脚配置 */
  GPIO_InitStruct.Pin = QSPI_CLK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//这句代码不能少
  GPIO_InitStruct.Alternate = QSPI_CLK_GPIO_AF;//有坑
  HAL_GPIO_Init(QSPI_CLK_GPIO_PORT, &GPIO_InitStruct);
  
  /* QSPI D0 GPIO pin 引脚配置 */
  GPIO_InitStruct.Pin = QSPI_BK1_D0_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//这句代码不能少
  GPIO_InitStruct.Alternate = QSPI_BK1_D0_GPIO_AF;//有坑
  HAL_GPIO_Init(QSPI_BK1_D0_GPIO_PORT, &GPIO_InitStruct);
  
  /* QSPI D1 GPIO 引脚配置 */
  GPIO_InitStruct.Pin = QSPI_BK1_D1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//这句代码不能少
  GPIO_InitStruct.Alternate = QSPI_BK1_D1_GPIO_AF;//有坑
  HAL_GPIO_Init(QSPI_BK1_D1_GPIO_PORT, &GPIO_InitStruct);
  
  /* QSPI D2 GPIO 引脚配置 */
  GPIO_InitStruct.Pin = QSPI_BK1_D2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//这句代码不能少
  GPIO_InitStruct.Alternate = QSPI_BK1_D2_GPIO_AF;//有坑
  HAL_GPIO_Init(QSPI_BK1_D2_GPIO_PORT, &GPIO_InitStruct);
  
  /* QSPI D3 GPIO 引脚配置 */
  GPIO_InitStruct.Pin = QSPI_BK1_D3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//这句代码不能少
  GPIO_InitStruct.Alternate = QSPI_BK1_D3_GPIO_AF;//有坑
  HAL_GPIO_Init(QSPI_BK1_D3_GPIO_PORT, &GPIO_InitStruct);
	
	
}

/*
*********************************************************************************************************
*    函 数 名: HAL_QSPI_MspDeInit
*    功能说明: QSPI底层复位，被HAL_QSPI_Init调用的底层函数
*    形    参: hqspi QSPI_HandleTypeDef类型句柄
*    返 回 值: 无
*********************************************************************************************************
*/
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
  /* 复位QSPI引脚 */
  HAL_GPIO_DeInit(QSPI_CS_GPIO_PORT, QSPI_CS_PIN);
  HAL_GPIO_DeInit(QSPI_CLK_GPIO_PORT, QSPI_CLK_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D0_GPIO_PORT, QSPI_BK1_D0_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D1_GPIO_PORT, QSPI_BK1_D1_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D2_GPIO_PORT, QSPI_BK1_D2_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D3_GPIO_PORT, QSPI_BK1_D3_PIN);

  /* 复位QSPI */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();

  /* 关闭QSPI时钟 */
  __HAL_RCC_QSPI_CLK_DISABLE();
}







#endif 








   
/*********************************************END OF FILE**********************/
