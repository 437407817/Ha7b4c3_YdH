/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.h
  * @brief   This file contains all the function prototypes for
  *          the dma.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_SDRAM_DMA_H__
#define __BSP_SDRAM_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./sdram/bsp_sdram.h" 
#include "./user_test_config.h" 



void SDRAM_DMA_Init(void);


#if TEST_SDRAM_DMA
void SDRAM_DMA_Test(void);
#endif





#ifdef __cplusplus
}
#endif




#endif /* __DMA_H__ */

