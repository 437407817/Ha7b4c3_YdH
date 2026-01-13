#ifndef __SDRAM_H
#define	__SDRAM_H

#include "stm32_FH_xxx.h"
#include <stdio.h>
#include "./sys/sysio.h"



#define SDRAM_SIZE 0x4000000  //64M字节,两片W9825G6KH-6,32MB@16bit组成64M@32bit


/**
  * @brief  SDRAM status structure definition
  */
#define   STATE_SDRAM_OK         ((uint8_t)0x00)
#define   STATE_SDRAM_ERROR      ((uint8_t)0x01)


/*SDRAM 的bank选择*/  
#define FMC_BANK_SDRAM            FMC_Bank2_SDRAM  
#define FMC_COMMAND_TARGET_BANK   FMC_SDRAM_CMD_TARGET_BANK2

/**
  * @brief  FMC SDRAM 数据基地址
  */   
#define SDRAM_BANK_ADDR     ((uint32_t)0xD0000000)
  
/**
  * @brief  FMC SDRAM 数据宽度
  */  
/* #define SDRAM_MEMORY_WIDTH   FMC_SDRAM_MEM_BUS_WIDTH_8  */
/* #define SDRAM_MEMORY_WIDTH   FMC_SDRAM_MEM_BUS_WIDTH_16  */
#define SDRAM_MEMORY_WIDTH    FMC_SDRAM_MEM_BUS_WIDTH_32 

/**
  * @brief  FMC SDRAM CAS Latency
  */  
/* #define SDRAM_CAS_LATENCY   FMC_CAS_Latency_2  */
#define SDRAM_CAS_LATENCY    FMC_SDRAM_CAS_LATENCY_3

/**
  * @brief  FMC SDRAM SDCLK时钟分频因子
  */  
#define SDCLOCK_PERIOD    FMC_SDRAM_CLOCK_PERIOD_2   
/* #define SDCLOCK_PERIOD    FMC_SDClock_Period_3 */

/**
  * @brief  FMC SDRAM 突发读取特性
  */  
/* #define SDRAM_READBURST    FMC_SDRAM_RBURST_DISABLE*/ 
#define SDRAM_READBURST    FMC_Read_Burst_Enable  
  

#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)

/* 需根据你的SDRAM芯片手册调整以下宏 */

#define SDRAM_FMC_CLOCK_MHZ   120                         // SDRAM最终时钟频率(120MHz)



/* DMA definitions for SDRAM DMA transfer */
#define __DMAx_CLK_ENABLE       __HAL_RCC_DMA2_CLK_ENABLE
#define SDRAM_DMAx_CHANNEL      DMA_CHANNEL_0
#define SDRAM_DMAx_STREAM       DMA2_Stream0
#define SDRAM_DMAx_IRQn         DMA2_Stream0_IRQn
#define SDRAM_DMAx_IRQHandler   DMA2_Stream0_IRQHandler


/* -------------------------- 1. 宏定义（根据硬件配置调整） -------------------------- */
// H743 MDMA配置（示例：使用MDMA_Channel0，需匹配FMC/SDRAM的MDMA请求线）
#define SDRAM_MDMAx_CHANNEL    MDMA_Channel0
#define SDRAM_MDMAx_INSTANCE   MDMA_Channel0  // H743 MDMA无Stream概念，直接用Channel
#define SDRAM_MDMAx_IRQn       MDMA_Channel0_IRQn  // MDMA通道0中断号
#define SDRAM_MDMAx_CLK_ENABLE()  __HAL_RCC_MDMA_CLK_ENABLE()  // MDMA时钟使能宏


/**
  * @brief  FMC SDRAM 模式配置的寄存器相关定义
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)      


/*信息输出*/
#define SDRAM_DEBUG_ON         1

#define SDRAM_INFO						SYSTEM_DEBUG
#define SDRAM_ERROR						SYSTEM_DEBUG
#define SDRAM_DEBUG          SYSTEM_DEBUG
/**
  * @}
  */  
 
/*地址信号线*/  
#define FMC_A0_GPIO_PORT         GPIOF
#define FMC_A0_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A0_GPIO_PIN          GPIO_PIN_0

#define FMC_A1_GPIO_PORT         GPIOF
#define FMC_A1_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A1_GPIO_PIN          GPIO_PIN_1

#define FMC_A2_GPIO_PORT         GPIOF
#define FMC_A2_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A2_GPIO_PIN          GPIO_PIN_2

#define FMC_A3_GPIO_PORT         GPIOF
#define FMC_A3_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A3_GPIO_PIN          GPIO_PIN_3

#define FMC_A4_GPIO_PORT         GPIOF
#define FMC_A4_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A4_GPIO_PIN          GPIO_PIN_4

#define FMC_A5_GPIO_PORT         GPIOF
#define FMC_A5_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A5_GPIO_PIN          GPIO_PIN_5

#define FMC_A6_GPIO_PORT         GPIOF
#define FMC_A6_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A6_GPIO_PIN          GPIO_PIN_12

#define FMC_A7_GPIO_PORT         GPIOF
#define FMC_A7_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A7_GPIO_PIN          GPIO_PIN_13

#define FMC_A8_GPIO_PORT         GPIOF
#define FMC_A8_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A8_GPIO_PIN          GPIO_PIN_14

#define FMC_A9_GPIO_PORT         GPIOF
#define FMC_A9_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A9_GPIO_PIN          GPIO_PIN_15

#define FMC_A10_GPIO_PORT        GPIOG
#define FMC_A10_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_A10_GPIO_PIN         GPIO_PIN_0

#define FMC_A11_GPIO_PORT        GPIOG
#define FMC_A11_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_A11_GPIO_PIN         GPIO_PIN_1

#define FMC_A12_GPIO_PORT        GPIOG
#define FMC_A12_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_A12_GPIO_PIN         GPIO_PIN_2
/*数据信号线*/
#define FMC_D0_GPIO_PORT         GPIOD
#define FMC_D0_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D0_GPIO_PIN          GPIO_PIN_14

#define FMC_D1_GPIO_PORT         GPIOD
#define FMC_D1_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D1_GPIO_PIN          GPIO_PIN_15

#define FMC_D2_GPIO_PORT         GPIOD
#define FMC_D2_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D2_GPIO_PIN          GPIO_PIN_0

#define FMC_D3_GPIO_PORT         GPIOD
#define FMC_D3_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D3_GPIO_PIN          GPIO_PIN_1

#define FMC_D4_GPIO_PORT         GPIOE
#define FMC_D4_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D4_GPIO_PIN          GPIO_PIN_7

#define FMC_D5_GPIO_PORT         GPIOE
#define FMC_D5_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D5_GPIO_PIN          GPIO_PIN_8

#define FMC_D6_GPIO_PORT         GPIOE
#define FMC_D6_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D6_GPIO_PIN          GPIO_PIN_9

#define FMC_D7_GPIO_PORT         GPIOE
#define FMC_D7_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D7_GPIO_PIN          GPIO_PIN_10

#define FMC_D8_GPIO_PORT         GPIOE
#define FMC_D8_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D8_GPIO_PIN          GPIO_PIN_11

#define FMC_D9_GPIO_PORT         GPIOE
#define FMC_D9_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D9_GPIO_PIN          GPIO_PIN_12

#define FMC_D10_GPIO_PORT        GPIOE
#define FMC_D10_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_D10_GPIO_PIN         GPIO_PIN_13

#define FMC_D11_GPIO_PORT        GPIOE
#define FMC_D11_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_D11_GPIO_PIN         GPIO_PIN_14

#define FMC_D12_GPIO_PORT        GPIOE
#define FMC_D12_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_D12_GPIO_PIN         GPIO_PIN_15

#define FMC_D13_GPIO_PORT        GPIOD
#define FMC_D13_GPIO_CLK()       __GPIOD_CLK_ENABLE()
#define FMC_D13_GPIO_PIN         GPIO_PIN_8

#define FMC_D14_GPIO_PORT        GPIOD
#define FMC_D14_GPIO_CLK()       __GPIOD_CLK_ENABLE()
#define FMC_D14_GPIO_PIN         GPIO_PIN_9

#define FMC_D15_GPIO_PORT        GPIOD
#define FMC_D15_GPIO_CLK()       __GPIOD_CLK_ENABLE()
#define FMC_D15_GPIO_PIN         GPIO_PIN_10

#define FMC_D16_GPIO_PORT         GPIOH
#define FMC_D16_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D16_GPIO_PIN          GPIO_PIN_8

#define FMC_D17_GPIO_PORT         GPIOH
#define FMC_D17_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D17_GPIO_PIN          GPIO_PIN_9

#define FMC_D18_GPIO_PORT         GPIOH
#define FMC_D18_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D18_GPIO_PIN          GPIO_PIN_10

#define FMC_D19_GPIO_PORT         GPIOH
#define FMC_D19_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D19_GPIO_PIN          GPIO_PIN_11

#define FMC_D20_GPIO_PORT         GPIOH
#define FMC_D20_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D20_GPIO_PIN          GPIO_PIN_12

#define FMC_D21_GPIO_PORT         GPIOH
#define FMC_D21_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D21_GPIO_PIN          GPIO_PIN_13

#define FMC_D22_GPIO_PORT         GPIOH
#define FMC_D22_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D22_GPIO_PIN          GPIO_PIN_14

#define FMC_D23_GPIO_PORT         GPIOH
#define FMC_D23_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_D23_GPIO_PIN          GPIO_PIN_15

#define FMC_D24_GPIO_PORT         GPIOI
#define FMC_D24_GPIO_CLK()        __GPIOI_CLK_ENABLE()
#define FMC_D24_GPIO_PIN          GPIO_PIN_0

#define FMC_D25_GPIO_PORT         GPIOI
#define FMC_D25_GPIO_CLK()        __GPIOI_CLK_ENABLE()
#define FMC_D25_GPIO_PIN          GPIO_PIN_1

#define FMC_D26_GPIO_PORT        GPIOI
#define FMC_D26_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_D26_GPIO_PIN         GPIO_PIN_2

#define FMC_D27_GPIO_PORT        GPIOI
#define FMC_D27_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_D27_GPIO_PIN         GPIO_PIN_3

#define FMC_D28_GPIO_PORT        GPIOI
#define FMC_D28_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_D28_GPIO_PIN         GPIO_PIN_6

#define FMC_D29_GPIO_PORT        GPIOI
#define FMC_D29_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_D29_GPIO_PIN         GPIO_PIN_7

#define FMC_D30_GPIO_PORT        GPIOI
#define FMC_D30_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_D30_GPIO_PIN         GPIO_PIN_9

#define FMC_D31_GPIO_PORT        GPIOI
#define FMC_D31_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_D31_GPIO_PIN         GPIO_PIN_10

/*控制信号线*/  
#define FMC_CS_GPIO_PORT         GPIOH
#define FMC_CS_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_CS_GPIO_PIN          GPIO_PIN_6

#define FMC_BA0_GPIO_PORT        GPIOG
#define FMC_BA0_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_BA0_GPIO_PIN         GPIO_PIN_4

#define FMC_BA1_GPIO_PORT        GPIOG
#define FMC_BA1_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_BA1_GPIO_PIN         GPIO_PIN_5

#define FMC_WE_GPIO_PORT         GPIOC
#define FMC_WE_GPIO_CLK()        __GPIOC_CLK_ENABLE()
#define FMC_WE_GPIO_PIN          GPIO_PIN_0

#define FMC_RAS_GPIO_PORT        GPIOF
#define FMC_RAS_GPIO_CLK()       __GPIOF_CLK_ENABLE()
#define FMC_RAS_GPIO_PIN         GPIO_PIN_11

#define FMC_CAS_GPIO_PORT        GPIOG
#define FMC_CAS_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_CAS_GPIO_PIN         GPIO_PIN_15

#define FMC_CLK_GPIO_PORT        GPIOG
#define FMC_CLK_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_CLK_GPIO_PIN         GPIO_PIN_8

#define FMC_CKE_GPIO_PORT        GPIOH
#define FMC_CKE_GPIO_CLK()       __GPIOH_CLK_ENABLE()
#define FMC_CKE_GPIO_PIN         GPIO_PIN_7

/*UDQM LDQM*/
#define FMC_UDQM_GPIO_PORT        GPIOE
#define FMC_UDQM_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_UDQM_GPIO_PIN         GPIO_PIN_1

#define FMC_LDQM_GPIO_PORT        GPIOE
#define FMC_LDQM_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_LDQM_GPIO_PIN         GPIO_PIN_0

/*UDQM2 LDQM2*/
#define FMC_UDQM2_GPIO_PORT        GPIOI
#define FMC_UDQM2_GPIO_CLK()       __GPIOI_CLK_ENABLE()
#define FMC_UDQM2_GPIO_PIN         GPIO_PIN_5

#define FMC_LDQM2_GPIO_PORT       GPIOI
#define FMC_LDQM2_GPIO_CLK()      __GPIOI_CLK_ENABLE()
#define FMC_LDQM2_GPIO_PIN        GPIO_PIN_4


/** @defgroup STM32429 SDRAM函数
  * @{
  */ 
void  SDRAM_Init(void);
//void  SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
//void  SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

void Disable_NAND_CS(void);



uint8_t           BSP_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t           BSP_SDRAM_ReadData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t           BSP_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t           BSP_SDRAM_WriteData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t           BSP_SDRAM_Sendcmd(FMC_SDRAM_CommandTypeDef *SdramCmd);
//void              BSP_SDRAM_DMA_IRQHandler(void);



uint8_t sdram_send_cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval);




#endif /* __SDRAM_H */
