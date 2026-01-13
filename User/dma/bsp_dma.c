/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "./dma/bsp_dma.h"
 #include "./sys/sysio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
DMA_HandleTypeDef hdma_memtomem_dma2_stream7;
/**
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma2_stream0
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
//  __HAL_RCC_DMA2_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
//  hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
//  hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
//  hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
//  hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
//  hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_ENABLE;
//  hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//  hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//  hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
//  hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_LOW;
//  hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//  hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//  hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_SINGLE;
//  hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_SINGLE;
//  if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)
//  {
//    Error_Handler();
//  }

//	  /* Configure DMA request hdma_memtomem_dma2_stream7 on DMA2_Stream7 */
//  hdma_memtomem_dma2_stream7.Instance = DMA2_Stream7;
//  hdma_memtomem_dma2_stream7.Init.Channel = DMA_CHANNEL_0;
//  hdma_memtomem_dma2_stream7.Init.Direction = DMA_MEMORY_TO_MEMORY;
//  hdma_memtomem_dma2_stream7.Init.PeriphInc = DMA_PINC_ENABLE;
//  hdma_memtomem_dma2_stream7.Init.MemInc = DMA_MINC_ENABLE;
//  hdma_memtomem_dma2_stream7.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//  hdma_memtomem_dma2_stream7.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//  hdma_memtomem_dma2_stream7.Init.Mode = DMA_NORMAL;
//  hdma_memtomem_dma2_stream7.Init.Priority = DMA_PRIORITY_LOW;
//  hdma_memtomem_dma2_stream7.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//  hdma_memtomem_dma2_stream7.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//  hdma_memtomem_dma2_stream7.Init.MemBurst = DMA_MBURST_SINGLE;
//  hdma_memtomem_dma2_stream7.Init.PeriphBurst = DMA_PBURST_SINGLE;
//  if (HAL_DMA_Init(&hdma_memtomem_dma2_stream7) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//  /* DMA2_Stream7_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
/*
*注册的是传输完成中断
*具体注册的中断服务函数在lv_port_disp.c中
*/
//HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream7, HAL_DMA_XFER_CPLT_CB_ID, LVGL_LCD_FSMC_DMA_pCallback);
}


//void DMA2_Stream7_IRQHandler(void)
//{
//  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

//  /* USER CODE END DMA2_Stream7_IRQn 0 */
//  HAL_DMA_IRQHandler(&hdma_memtomem_dma2_stream7);
//  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

//  /* USER CODE END DMA2_Stream7_IRQn 1 */
//}
/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

