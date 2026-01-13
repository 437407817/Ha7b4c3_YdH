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
#include "./sdram/bsp_sdram_dma.h"

#include "string.h"

 #include "./sys/sysio.h"








// DMA句柄（内存→SDRAM：发送）
DMA_HandleTypeDef hdma_mem2sdram;
// DMA句柄（SDRAM→内存：接收）
DMA_HandleTypeDef hdma_sdram2mem;



/************************* DMA初始化（内存→SDRAM） *************************/
static void MX_DMA_Mem2SDRAM_Init(void)
{
  // 1. 使能DMA2时钟
  __HAL_RCC_DMA2_CLK_ENABLE();

  // 2. 配置DMA句柄
  hdma_mem2sdram.Instance = DMA2_Stream0;
  hdma_mem2sdram.Init.Request = DMA_REQUEST_MEM2MEM; // 内存到内存（SDRAM映射为内存地址）
  hdma_mem2sdram.Init.Direction = DMA_MEMORY_TO_MEMORY; // 方向：片内RAM→SDRAM
  hdma_mem2sdram.Init.PeriphInc = DMA_PINC_ENABLE;      // 外设地址（SDRAM）自增
  hdma_mem2sdram.Init.MemInc = DMA_MINC_ENABLE;         // 内存地址（片内RAM）自增
  hdma_mem2sdram.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // 16位（匹配SDRAM宽度）
  hdma_mem2sdram.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    // 16位
  hdma_mem2sdram.Init.Mode = DMA_NORMAL;                // 正常模式（非循环）
  hdma_mem2sdram.Init.Priority = DMA_PRIORITY_HIGH;     // 高优先级
  hdma_mem2sdram.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  // 3. 初始化DMA
  if (HAL_DMA_Init(&hdma_mem2sdram) != HAL_OK)
  {
    Error_Handler();
  }

  // 4. 绑定DMA句柄（可选，用于中断回调）
//  __HAL_LINKDMA(NULL, hdma_mem2sdram, hdma_mem2sdram);

  // 5. 配置DMA中断优先级
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/************************* DMA初始化（SDRAM→内存） *************************/
static void MX_DMA_SDRAM2Mem_Init(void)
{
  // 1. 配置DMA句柄
  hdma_sdram2mem.Instance = DMA2_Stream1;
  hdma_sdram2mem.Init.Request = DMA_REQUEST_MEM2MEM;
  hdma_sdram2mem.Init.Direction = DMA_MEMORY_TO_MEMORY; // 方向：SDRAM→片内RAM
  hdma_sdram2mem.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_sdram2mem.Init.MemInc = DMA_MINC_ENABLE;
  hdma_sdram2mem.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_sdram2mem.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_sdram2mem.Init.Mode = DMA_NORMAL;
  hdma_sdram2mem.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_sdram2mem.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  // 2. 初始化DMA
  if (HAL_DMA_Init(&hdma_sdram2mem) != HAL_OK)
  {
    Error_Handler();
  }

  // 3. 绑定+中断配置
//  __HAL_LINKDMA(NULL, hdma_sdram2mem, hdma_sdram2mem);
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 1);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}

void SDRAM_DMA_Init(void){


  MX_DMA_Mem2SDRAM_Init();
  MX_DMA_SDRAM2Mem_Init();

}
#if 0
/************************* DMA中断服务函数（可选） *************************/
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_mem2sdram);
}

void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_sdram2mem);
}
#endif




#if TEST_SDRAM_DMA

#define SDRAM_BASE_ADDR     SDRAM_BANK_ADDR
// DMA传输数据长度（16KB，可根据需求调整）
#define DMA_TRANSFER_LEN    4096U
// 片内RAM测试缓冲区（用于对比验证）
uint16_t tx_buffer[DMA_TRANSFER_LEN] = {0};
uint16_t rx_buffer[DMA_TRANSFER_LEN] = {0};

/************************* SDRAM DMA传输测试函数 *************************/
void SDRAM_DMA_Test(void)
{
  // 1. 初始化SDRAM和DMA
//  MX_FMC_SDRAM_Init();


  // 2. 填充发送缓冲区（生成测试数据）
  for (uint32_t i = 0; i < DMA_TRANSFER_LEN; i++)
  {
    tx_buffer[i] = i + 0x1234; // 自定义测试数据
  }
  memset(rx_buffer, 0, sizeof(rx_buffer)); // 清空接收缓冲区

  // 3. DMA传输：片内RAM → SDRAM
  if (HAL_DMA_Start(&hdma_mem2sdram, (uint32_t)tx_buffer, (uint32_t)SDRAM_BASE_ADDR, DMA_TRANSFER_LEN) != HAL_OK)
  {
     SDRAM_INFO("fail 1"); // 传输启动失败
		return;
  }
  HAL_DMA_PollForTransfer(&hdma_mem2sdram, HAL_DMA_FULL_TRANSFER, 100); // 等待传输完成（超时100ms）
  if (hdma_mem2sdram.State != HAL_DMA_STATE_READY)
  {
		SDRAM_INFO("fail 2"); 
    return ; // 传输超时
  }

  // 4. DMA传输：SDRAM → 片内RAM
  if (HAL_DMA_Start(&hdma_sdram2mem, (uint32_t)SDRAM_BASE_ADDR, (uint32_t)rx_buffer, DMA_TRANSFER_LEN) != HAL_OK)
  {
		SDRAM_INFO("fail 3"); 
    return ; // 传输启动失败
  }
  HAL_DMA_PollForTransfer(&hdma_sdram2mem, HAL_DMA_FULL_TRANSFER, 100);
  if (hdma_sdram2mem.State != HAL_DMA_STATE_READY)
  {
		SDRAM_INFO("fail 4"); 
    return ; // 传输超时
  }

  // 5. 验证数据一致性（对比tx_buffer和rx_buffer）
  for (uint32_t i = 0; i < DMA_TRANSFER_LEN; i++)
  {
    if (tx_buffer[i] != rx_buffer[i])
    {
			SDRAM_INFO("fail 5"); 
      return ; // 数据不一致
    }
  }
SDRAM_INFO("success 0"); 
  return ; // 测试成功
}
#endif

