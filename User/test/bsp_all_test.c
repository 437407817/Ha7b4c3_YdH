





#include "./test/bsp_all_test.h"

#include "./user_test_config.h"
#include "./usart/bsp_usart.h"

#include "./test/bsp_usart_test.h"
#include "./user_test_config.h"
#include "./test/bsp_iic_test.h"
//#include "./test/bsp_segger_test.h"
//#include "./test/bsp_spi_test.h"
//#include "./test/bsp_sdram_test.h"
//#include "./test/bsp_lcd_test.h"
//#include "./test/bsp_usart_test.h"
#include "./usart/bsp_usart_shell.h"
#include "./usart/bsp_usart_dma.h"
#include "./test/bsp_sdram_test.h"

#include "./sdram/bsp_sdram_dma.h"
#include "./test/bsp_lcd_test.h"
#include "./test/bsp_qspi_flash_test.h"
#include "./lvgl_port.h"

//#include "usbd_init.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void TEST_ALL_Init(void)
{

	
	
	

}

void TEST_ALL_TEST(void)
{
printf("TEST_ALL_TEST\n");
#if (TEST_SHELL_UART==1)
	Test_USART_SHELL_while();
#endif
	#if TEST_RingBuffer_SAVEANDREAD
	Test_Usart_QueueSaveFun();
	#endif
	
#if TEST_DMA_RB_READ
	TEST_USART_RX_DMA_ALL_while();
#endif

#if 0	
#if TESTUsartDMASendSaveAndSend
	TEST_Usart_SendFUN_ALL();//单独测试，最好多任务测试效果最佳
#endif	
#endif	


#if (TEST_I2C==1)	
	I2C_Test();
#endif
#if (TEST_SDRAM==1)
SDRAM_Basic_Test();
#endif

#if TEST_SDRAM_DMA
SDRAM_DMA_Test();
#endif


#if TEST_LCD	
	LCD_main_Test();
	#endif
	
#if TEST_QSPI_FLASH
	Flash_Test();
#endif

#if (USE_LVGL)
lv_test();//使用裸机调用LVGL
#endif
//	Segger_Test();
	
//	SPI_FLASH_Test();
//	SDRAM_Test();

#ifdef TEST_LCD_MODULE_ENABLED	
	
#endif
// USART_DMA_Test();
//	BSP_usbd_test();
	
}













































//----------------------------------------------------------------------------

#if 0
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
static __IO uint32_t  IIC1Timeout = I2CT_LONG_TIMEOUT;   
/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*!< EEPROM_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(IIC1_CLK, ENABLE);
  
  /*!< EEPROM_I2C_SCL_GPIO_CLK and EEPROM_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(IIC1_SCL_GPIO_CLK | IIC1_SDA_GPIO_CLK, ENABLE);

  /*!< GPIO configuration */
 GPIO_InitStructure.GPIO_Pin = IIC1_SCL_PIN;
	
#ifdef IIC_STM32F10X
RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 	
	
#else
	
	//STM32F4配置
 /* Connect PXx to I2C_SCL*/
	
  GPIO_PinAFConfig(IIC1_SCL_GPIO_PORT, IIC1_SCL_SOURCE, IIC1_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(IIC1_SDA_GPIO_PORT, IIC1_SDA_SOURCE, IIC1_SDA_AF);
	
 GPIO_InitStructure.GPIO_Mode = IIC1_SCL_AF;
#endif	
	
  /*!< Configure EEPROM_I2C pins: SCL */   
	
	  
	
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(IIC1_SCL_GPIO_PORT, &GPIO_InitStructure);	
	
	
  /*!< Configure EEPROM_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = IIC1_SDA_PIN;
	
  GPIO_Init(IIC1_SDA_GPIO_PORT, &GPIO_InitStructure);	
 
 	GPIO_SetBits(IIC1_SCL_GPIO_PORT,IIC1_SCL_PIN);
 	GPIO_SetBits(IIC1_SDA_GPIO_PORT,IIC1_SDA_PIN);	
 
 
 
 



}


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
  I2C_InitStructure.I2C_OwnAddress1 =IIC1_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_ClockSpeed = IIC1_Speed;	                            /* 通信速率 */
  I2C_Init(IIC1, &I2C_InitStructure);	                                      /* I2C1 初始化 */
  I2C_Cmd(IIC1, ENABLE);  	                                                /* 使能 I2C1 */

  I2C_AcknowledgeConfig(IIC1, ENABLE);  
}


void IIC_ConfigInit()
{
  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();
}


uint32_t I2C_AddressToWriteInsert(u8 SlaverAddress){


  /* Send EEPROM address for write */
  I2C_Send7bitAddress(IIC1, SlaverAddress, I2C_Direction_Transmitter);//模块地址
  
  
  IIC1Timeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }    
      
return 1;
}




uint32_t I2C_ByteToWriteInsert(u8 WriteAddr){


  I2C_SendData(IIC1, WriteAddr);
  
  IIC1Timeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 

  return 1;

}


uint32_t I2C_START(){

  /* Send STRAT condition */
  I2C_GenerateSTART(IIC1, ENABLE);//开启

  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }    

  return 1;
}

void I2C_STOP(){
 I2C_GenerateSTOP(IIC1, ENABLE);//停止
}





uint32_t I2C_ByteToWriteOver(u8 SlaverAddress, u16 InsideAddrOrCMD,u8 Data)
{
  /* 1 Send STRAT condition */
  I2C_GenerateSTART(IIC1, ENABLE);//开启

  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }    

  /* 2 Send EEPROM address for write */
  I2C_Send7bitAddress(IIC1, SlaverAddress, I2C_Direction_Transmitter);//模块地址
  
  
  IIC1Timeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }    
    /* 3 Send the EEPROM's internal address to write to */
	#if (8==IIC1_WORD_ADDR_SIZE)  
 
  I2C_SendData(IIC1, InsideAddrOrCMD&0x00ff);//模块内部地址cmd
  
  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
	#else
	
	I2C_SendData(IIC1, (u8)(SlaInsideAddr>>8));//模块内部地址cmd
	  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }
	  I2C_SendData(IIC1, (u8)(SlaInsideAddr&0x00ff));//模块内部地址cmd
	  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }
	#endif
  /* Send the byte to be written */
  I2C_SendData(IIC1, Data); //模块内部数据data
   
  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(IIC1, ENABLE);//停止
  
  return 1;
}

uint32_t I2C_BytesToWriteOver(u8 SlaverAddress, u16 InsideAddrOrCMD,u8 *pData)
{
  uint16_t i;
	/* 1 Send STRAT condition */
  I2C_GenerateSTART(IIC1, ENABLE);//开启

  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }    

  /* 2 Send EEPROM address for write */
  I2C_Send7bitAddress(IIC1, SlaverAddress, I2C_Direction_Transmitter);//模块地址
  
  
  IIC1Timeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }    
    /* 3 Send the EEPROM's internal address to write to */
	#if (8==IIC1_WORD_ADDR_SIZE)  
 
  I2C_SendData(IIC1, InsideAddrOrCMD&0x00ff);//模块内部地址cmd
  
  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
	#else
	
	I2C_SendData(IIC1, (u8)(SlaInsideAddr>>8));//模块内部地址cmd
	  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }
	  I2C_SendData(IIC1, (u8)(SlaInsideAddr&0x00ff));//模块内部地址cmd
	  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }
	#endif
  /* Send the byte to be written */
	for(i=0;i<sizeof(pData);i++){
	
	  I2C_SendData(IIC1, pData[i]); //模块内部数据data
   
  IIC1Timeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(IIC1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((IIC1Timeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
	
	}

  
  /* Send STOP condition */
  I2C_GenerateSTOP(IIC1, ENABLE);//停止
  
  return 1;
}





/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  IIC_ERROR("I2C wait overtime! errorCode = %d",errorCode);
  
  return 0;
}
#endif

