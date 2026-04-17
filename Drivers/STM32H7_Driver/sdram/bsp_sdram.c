/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdram应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F767 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./sdram/bsp_sdram.h"  

static SDRAM_HandleTypeDef SdramHandle;
//SDRAM_HandleTypeDef SdramHandle;
//#define SdramHandle SdramHandle
static FMC_SDRAM_CommandTypeDef Command;
// DMA_HandleTypeDef dmaHandle;




//// H743 MDMA句柄（替代F429的DMA_HandleTypeDef）
//MDMA_HandleTypeDef mdmaHandle;
// H743 SDRAM核心句柄（需与HAL库的SDRAM_HandleTypeDef一致）
//SDRAM_HandleTypeDef SdramHandle;

/**
  * @brief  延迟一段时间
  * @param  延迟的时间长度
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
/**
  * @brief  初始化控制SDRAM的IO
  * @param  无
  * @retval 无
  */
/**
  * @brief  初始化控制SDRAM的IO
  * @param  无
  * @retval 无
  */
static void SDRAM_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
  
	/* 使能SDRAM相关的IO时钟 */
	/*地址信号线*/
	FMC_A0_GPIO_CLK();FMC_A1_GPIO_CLK(); FMC_A2_GPIO_CLK();
	FMC_A3_GPIO_CLK();FMC_A4_GPIO_CLK(); FMC_A5_GPIO_CLK();
	FMC_A6_GPIO_CLK();FMC_A7_GPIO_CLK(); FMC_A8_GPIO_CLK();
	FMC_A9_GPIO_CLK();FMC_A10_GPIO_CLK();FMC_A11_GPIO_CLK();
	FMC_A12_GPIO_CLK();
	/*数据信号线*/
	FMC_D0_GPIO_CLK(); FMC_D1_GPIO_CLK() ; FMC_D2_GPIO_CLK() ; 
	FMC_D3_GPIO_CLK(); FMC_D4_GPIO_CLK() ; FMC_D5_GPIO_CLK() ;
	FMC_D6_GPIO_CLK(); FMC_D7_GPIO_CLK() ; FMC_D8_GPIO_CLK() ;
	FMC_D9_GPIO_CLK(); FMC_D10_GPIO_CLK(); FMC_D11_GPIO_CLK();
	FMC_D12_GPIO_CLK();FMC_D13_GPIO_CLK(); FMC_D14_GPIO_CLK();
	FMC_D15_GPIO_CLK();FMC_D16_GPIO_CLK(); FMC_D17_GPIO_CLK(); 
	FMC_D18_GPIO_CLK();FMC_D19_GPIO_CLK(); FMC_D20_GPIO_CLK();
	FMC_D21_GPIO_CLK();FMC_D22_GPIO_CLK(); FMC_D23_GPIO_CLK();
	FMC_D24_GPIO_CLK();FMC_D25_GPIO_CLK(); FMC_D26_GPIO_CLK();
	FMC_D27_GPIO_CLK();FMC_D28_GPIO_CLK(); FMC_D29_GPIO_CLK();
	FMC_D30_GPIO_CLK();FMC_D31_GPIO_CLK();	
	/*控制信号线*/
	FMC_CS_GPIO_CLK() ; FMC_BA0_GPIO_CLK(); FMC_BA1_GPIO_CLK() ;
	FMC_WE_GPIO_CLK() ; FMC_RAS_GPIO_CLK(); FMC_CAS_GPIO_CLK();
	FMC_CLK_GPIO_CLK(); FMC_CKE_GPIO_CLK(); FMC_UDQM_GPIO_CLK();
	FMC_LDQM_GPIO_CLK();FMC_UDQM2_GPIO_CLK();FMC_LDQM2_GPIO_CLK();
  
	/*-- SDRAM IO 配置 -----------------------------------------------------*/     
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//配置为复用功能
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF12_FMC;

	/*地址信号线 针对引脚配置*/
	GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN; 
	HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);

	/*数据信号线 针对引脚配置*/
	GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D16_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D16_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D17_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D17_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D18_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D18_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D19_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D19_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D20_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D20_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D21_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D21_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D22_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D22_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D23_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D23_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D24_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D24_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D25_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D25_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D26_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D26_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D27_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D27_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D28_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D28_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D29_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D29_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D30_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D30_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D31_GPIO_PIN; 
	HAL_GPIO_Init(FMC_D31_GPIO_PORT, &GPIO_InitStructure);
	/*控制信号线*/
	GPIO_InitStructure.Pin = FMC_CS_GPIO_PIN; 
	HAL_GPIO_Init(FMC_CS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_BA0_GPIO_PIN; 
	HAL_GPIO_Init(FMC_BA0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_BA1_GPIO_PIN;
	HAL_GPIO_Init(FMC_BA1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
	HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_RAS_GPIO_PIN; 
	HAL_GPIO_Init(FMC_RAS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_CAS_GPIO_PIN; 
	HAL_GPIO_Init(FMC_CAS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_CLK_GPIO_PIN; 
	HAL_GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_CKE_GPIO_PIN; 
	HAL_GPIO_Init(FMC_CKE_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN; 
	HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN; 
	HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_UDQM2_GPIO_PIN; 
	HAL_GPIO_Init(FMC_UDQM2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_LDQM2_GPIO_PIN; 
	HAL_GPIO_Init(FMC_LDQM2_GPIO_PORT, &GPIO_InitStructure);
}







/**
  * @brief  对SDRAM芯片进行初始化配置
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
	uint32_t tmpr = 0;

	/* Step 1 ----------------------------------------------------------------*/
	/* 配置命令：开启提供给SDRAM的时钟 */
	Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&SdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 2: 延时100us */ 
	SDRAM_delay(1);

	/* Step 3 ----------------------------------------------------------------*/
	/* 配置命令：对所有的bank预充电 */ 
	Command.CommandMode = FMC_SDRAM_CMD_PALL;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&SdramHandle, &Command, SDRAM_TIMEOUT);   

	/* Step 4 ----------------------------------------------------------------*/
	/* 配置命令：自动刷新 */   
	Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 8;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&SdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 5 ----------------------------------------------------------------*/
	/* 设置sdram寄存器配置 */
	tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
				   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
				   SDRAM_MODEREG_CAS_LATENCY_3           |
				   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
				   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* 配置命令：设置SDRAM寄存器 */
	Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = tmpr;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&SdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 6 ----------------------------------------------------------------*/

	/* 设置刷新计数器 */
	/* 刷新周期=64ms/8192行=7.8125us */
	/* COUNT=(7.8125us x Freq) - 20 */
	/* 设置自刷新速率 */
	HAL_SDRAM_ProgramRefreshRate(&SdramHandle, 824); 
}





/**
 * @brief       向SDRAM发送命令
 * @param        bankx:0,向BANK5上面的SDRAM发送指令
 * @param              1,向BANK6上面的SDRAM发送指令
 * @param       cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
 * @param       refresh:自刷新次数
 * @param       返回值:0,正常;1,失败.
 * @retval      模式寄存器的定义
 */
uint8_t sdram_send_cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval)
{
    uint32_t target_bank = 0;
    FMC_SDRAM_CommandTypeDef command;
    
    if (bankx == 0)
    {
        target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
    }
    else if (bankx == 1)
    {
        target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
    }

    command.CommandMode = cmd;                /* 命令 */
    command.CommandTarget = target_bank;      /* 目标SDRAM存储区域 */
    command.AutoRefreshNumber = refresh;      /* 自刷新次数 */
    command.ModeRegisterDefinition = regval;  /* 要写入模式寄存器的值 */

    if (HAL_SDRAM_SendCommand(&SdramHandle, &command, 0X1000) == HAL_OK) /* 向SDRAM发送命令 */
    {
        return 0;
    }

    else return 1;
}















/**
 * @brief  配置FMC的时钟源（PLL2），生成SDRAM所需的120MHz时钟
 * @note   基于HSE=25MHz计算，若晶振不同需调整PLL2参数
 * @retval HAL_StatusTypeDef：HAL_OK=成功，其他=失败
 */
static HAL_StatusTypeDef SDRAM_Clock_Config(void)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};

  /* 配置FMC时钟源为PLL2 */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FMC;
  RCC_PeriphClkInit.PLL2.PLL2M = 5;
  RCC_PeriphClkInit.PLL2.PLL2N = 144;
  RCC_PeriphClkInit.PLL2.PLL2P = 2;
  RCC_PeriphClkInit.PLL2.PLL2Q = 2;
  RCC_PeriphClkInit.PLL2.PLL2R = 3;
  RCC_PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  RCC_PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  RCC_PeriphClkInit.PLL2.PLL2FRACN = 0;
  RCC_PeriphClkInit.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;

  /* 应用时钟配置并返回结果 */
  return HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
}


/**
 * @brief  配置FMC SDRAM控制器的核心参数（匹配SDRAM芯片规格）
 * @note   参数需与SDRAM芯片手册一致（行/列位宽、CAS延迟等）
 * @retval 无
 */
static void SDRAM_Controller_Config(void)
{
  /* 初始化SDRAM控制器句柄 */
  SdramHandle.Instance = FMC_SDRAM_DEVICE;

  /* 核心参数配置 */
  SdramHandle.Init.SDBank = FMC_SDRAM_BANK2;                  // 挂载在Bank2
  SdramHandle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9; // 9位列地址
  SdramHandle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;     // 13位行地址
  SdramHandle.Init.MemoryDataWidth = SDRAM_MEMORY_WIDTH;          // 32位数据宽度
  SdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4; // 4个内部Bank
  SdramHandle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;          // CAS延迟3个时钟
  SdramHandle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE; // 禁用写保护
  SdramHandle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;     // SDRAM时钟=FMC时钟/2
  SdramHandle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;          // 使能突发传输
  SdramHandle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;      // 读管道延迟1个时钟
}



/**
 * @brief  配置SDRAM的时序参数（匹配SDRAM芯片手册）
 * @param  SdramTiming: 时序配置结构体指针（输出参数）
 * @retval 无
 */
static void SDRAM_Timing_Config(FMC_SDRAM_TimingTypeDef *SdramTiming)
{
  /* 时序参数配置（单位：SDRAM时钟周期） */
  SdramTiming->LoadToActiveDelay = 2;      // 加载模式寄存器到行有效延迟
  SdramTiming->ExitSelfRefreshDelay = 8;   // 退出自刷新延迟
  SdramTiming->SelfRefreshTime = 5;        // 自刷新时间
  SdramTiming->RowCycleDelay = 8;          // 行循环延迟
  SdramTiming->WriteRecoveryTime = 2;      // 写恢复延迟
  SdramTiming->RPDelay = 2;                // 预充电延迟
  SdramTiming->RCDDelay = 2;               // 行到列延迟
}





/**
 * @brief  SDRAM初始化主入口（整合所有子步骤）
 * @note   按「GPIO→时钟→控制器→时序→硬件初始化→芯片序列」顺序执行
 * @retval 无
 */
void SDRAM_Init(void)
{
  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* 步骤1：配置SDRAM GPIO引脚 */
  SDRAM_GPIO_Config();

  /* 步骤2：配置FMC时钟源（失败则进入错误处理） */
  if (SDRAM_Clock_Config() != HAL_OK)
  {
    Error_Handler();
  }

  /* 步骤3：使能FMC时钟 */
  __FMC_CLK_ENABLE();

  /* 步骤4：配置FMC SDRAM控制器核心参数 */
  SDRAM_Controller_Config();

  /* 步骤5：配置SDRAM时序参数 */
  SDRAM_Timing_Config(&SdramTiming);

  /* 步骤6：初始化FMC SDRAM硬件（失败则错误处理） */
  if (HAL_SDRAM_Init(&SdramHandle, &SdramTiming) != HAL_OK)
  {
    Error_Handler();
  }

  /* 步骤7：执行SDRAM芯片初始化序列 */
  SDRAM_InitSequence();
}








#if 0
/**
  * @brief  以“字”为单位向sdram写入数据 
  * @param  pBuffer: 指向数据的指针 
  * @param  uwWriteAddress: 要写入的SDRAM内部地址
  * @param  uwBufferSize: 要写入数据大小
  * @retval None.
  */
void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

  /* 禁止写保护 */
  HAL_SDRAM_WriteProtection_Disable(&SdramHandle);
  /* 检查SDRAM标志，等待至SDRAM空闲 */ 
  while(HAL_SDRAM_GetState(&SdramHandle) != RESET)
  {
  }

  /* 循环写入数据 */
  for (; uwBufferSize != 0; uwBufferSize--) 
  {
    /* 发送数据到SDRAM */
    *(uint32_t *) (SDRAM_DEVICE_ADDR + write_pointer) = *pBuffer++;

    /* 地址自增*/
    write_pointer += 4;
  }
    
}




/**
  * @brief  从SDRAM中读取数据 
  * @param  pBuffer: 指向存储数据的buffer
  * @param  ReadAddress: 要读取数据的地十
  * @param  uwBufferSize: 要读取的数据大小
  * @retval None.
  */
void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
  
   
  /* 检查SDRAM标志，等待至SDRAM空闲 */  
  while ( HAL_SDRAM_GetState(&SdramHandle) != RESET)
  {
  }
  
  /*读取数据 */
  for(; uwBufferSize != 0x00; uwBufferSize--)
  {
   *pBuffer++ = *(__IO uint32_t *)(SDRAM_DEVICE_ADDR + write_pointer );
    
   /* 地址自增*/
    write_pointer += 4;
  } 
}

#endif



void Disable_NAND_CS(void)
{		
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*开启GPIO外设时钟*/
    __GPIOG_CLK_ENABLE();

    /*选择要控制的GPIO引脚*/															   
    GPIO_InitStruct.Pin = GPIO_PIN_9;	

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*设置引脚速率为高速 */   
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);	

		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);	
}







/**
  * @brief  Reads an mount of data from the SDRAM memory in polling mode.
  * @param  uwStartAddress : Read start address
  * @param  pData : Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory
  */
uint8_t BSP_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
  if (HAL_SDRAM_Read_32b(&SdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return STATE_SDRAM_ERROR;
  }
  else
  {
    return STATE_SDRAM_OK;
  }
}

/**
  * @brief  Reads an mount of data from the SDRAM memory in DMA mode.
  * @param  uwStartAddress : Read start address
  * @param  pData : Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory
  */
uint8_t BSP_SDRAM_ReadData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
  if (HAL_SDRAM_Read_DMA(&SdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return STATE_SDRAM_ERROR;
  }
  else
  {
    return STATE_SDRAM_OK;
  }
}

/**
  * @brief  Writes an mount of data to the SDRAM memory in polling mode.
  * @param  uwStartAddress : Write start address
  * @param  pData : Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory
  */
uint8_t BSP_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
  /* Disable write protection */
  HAL_SDRAM_WriteProtection_Disable(&SdramHandle);

  /*Write 32-bit data buffer to SDRAM memory*/
  if (HAL_SDRAM_Write_32b(&SdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return STATE_SDRAM_ERROR;
  }
  else
  {
    return STATE_SDRAM_OK;
  }
}

/**
  * @brief  Writes an mount of data to the SDRAM memory in DMA mode.
  * @param  uwStartAddress : Write start address
  * @param  pData : Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory
  */
uint8_t BSP_SDRAM_WriteData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
  if (HAL_SDRAM_Write_DMA(&SdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return STATE_SDRAM_ERROR;
  }
  else
  {
    return STATE_SDRAM_OK;
  }
}

/**
  * @brief  Sends command to the SDRAM bank.
  * @param  SdramCmd: Pointer to SDRAM command structure
  * @retval HAL status
  */
uint8_t BSP_SDRAM_Sendcmd(FMC_SDRAM_CommandTypeDef *SdramCmd)
{
  if (HAL_SDRAM_SendCommand(&SdramHandle, SdramCmd, SDRAM_TIMEOUT) != HAL_OK)
  {
    return STATE_SDRAM_ERROR;
  }
  else
  {
    return STATE_SDRAM_OK;
  }
}

/**
  * @brief  Handles SDRAM DMA transfer interrupt request.
  */
//void BSP_SDRAM_DMA_IRQHandler(void)
//{
////	printf("SDRAM DMA interrupt triggered.\n");
//  HAL_DMA_IRQHandler(&dmaHandle);
//}

//// DMA 传输完成中断回调函数
//void HAL_DMA_TC_Callback(DMA_HandleTypeDef *hdma)
//{
//    if (hdma == &dmaHandle)
//    {
//        // 处理 DMA 传输完成后的操作
//        // 例如：点亮一个 LED 表示传输完成
////			printf("xxxxxxxxxxxx111");
//    }
//}















/*********************************************END OF FILE**********************/

