#include "./sys/sys_init_all.h" 
#include "./stm32_FH_xxx_hal.h"
#include "stm32_FH_xxx.h"
#include "./StateMechine/TaskFun.h"  
#include "./StateMechine/TaskSema.h"
#include "./sys/sysio.h"
/**
  ******************************************************************************
  * @file    bsp_util_time.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */


#include "sys_dwt.h"
#include "./DataConvert/data_random.h"
#include "./usart/bsp_usart_shell.h"
  #include "./test/bsp_all_test.h"
//#include "./io/bsp_io_input.h"
//#include "./io/bsp_io_output.h"  
//#include "./i2c/hard_i2c/bsp_iic.h"
#include "./usart/bsp_com_uart.h"
#include "./usart/bsp_usart_dma.h"
//#include "./test/bsp_all_test.h"
#include "./sdram/bsp_sdram_dma.h"
#include "./mpu/bsp_mpu.h" 


//#include "./spi/bsp_spi_flash.h"
//#include "./sdram/bsp_sdram.h" 




//#include "./usart/bsp_usart.h"
  
//#include "./adc/bsp_adc.h"
//#include "./i2c/hard_i2c/bsp_i2c_ee.h"
//#include "./i2c/hard_i2c/bsp_iic.h"
//#include "./sys/bsp_systime.h" 

//#include "./test/bsp_test_all.h"  

//#include "./can/bsp_can.h"
//#include "./SEGGER_RTT.h"
//#include "./extfun/oled/bsp_oled.h"
//#include "./io/IN_io/bsp_gpio_in.h"
////#include "./spi/lcd/lcd.h"
//#include "./i2c/soft_i2c/bsp_soft_i2c_gpio.h"
//#include "./io/OUT_io/bsp_gpio_out.h"  
//#include "./adc/ADS7822.h"
//#include "./adc/CC6902.h"
//#include "./time/bsp_TiMbase.h" 
////******************************
//#include "./dma/bsp_dma.h"
//#include "shell_port.h"
////#include "./zns_shell.h"

#include "./sdram/bsp_sdram.h"  
#include "./sys/sys_interrupt.h"  
//#include "./iwdg/bsp_iwdg.h"   
//#include "./pwr/bsp_pwr.h"   
#include <elog.h>
//#include "nr_micro_shell.h"
//#include "nr_shell_cfg.h" 
//#include "user_config.h"
#include "shell_port.h"
//#include "./sys/sys_interrupt.h" 
//#include "./fatfs.h"
#include "./touch/touch_drv.h"
#include "./i2c/factory_iic.h"
#include "./flash/bsp_qspi_flash.h"
//#include "usbd_init.h"
#include "./lvgl_port.h"
//#include "./i2c/factory_iic.h"
//#include "./dma/bsp_usart_dma.h"
//#include "./usart/p_data_queue_outer.h"
//#include "./usart/bsp_usart_shell.h"
//#include "./lvgl_port.h"
////#include "./dma/bsp_dma2d.h"
//#include "./touch/touch_drv.h"





extern void WIFI_PDN_INIT(void);

void ALL_Init_cfg(void){

	

//HAL_NVIC_SetPriorityGrouping(NVIC_PriorityGroup_x);//HAL_Init(void)
//SysTick_Init();//HAL_Init中自动调用--HAL_InitTick
DWT_Init();
//Init_PWR();	
//WIFI_PDN_INIT();
	
//	
	 
	 MX_RNG_Init();  // 初始化RNG外设（关键！必须调用，否则生成失败）

//	MX_DMA_Init();
	

	DWT_Delay_ms(1000);
	SEGGER_RTT_Init();SYSTEM_INFO("SEGGER_RTT_Init over------------- \n");	
	USART_Shell_ComDrvInit();SYSTEM_DEBUG("USART_Shell_ComDrvInit over\n");
#if	1
	#if USE_UART_DMA
	
	USART_TX_RX_DMA_ConfigALL();
	
	#endif
#endif	
	printf("USART_Config=X= init over------------- ");SYSTEM_INFO("USART_Config=X= init over------------- \n");	//如果未初始化串口就printf，会卡死程序

SYSTEM_INFO("VERSION:%X\n",BSP_GetVersion());	

#if !USE_OS 
AppTaskScheduleInit();SYSTEM_INFO("TaskSchedule_Init over------------- \n");	
#endif	
////	
//OUT_Io_GPIO_Config();		SYSTEM_INFO("OUT_Io_GPIO_Init over------------- \n");
//IN_Io_ALL_GPIO_Config();	SYSTEM_INFO("IN_Io_GPIO_Init over------------- \n");
//	

////I2C_EE_Hard_Init();	SYSTEM_INFO("I2C_EE_Hard_Init over------------- \n");
IIC_EE_Factory_Init();SYSTEM_INFO("IIC_EE_Factory_Init over------------- \n");

//	
//	
//SPI_FLASH_Init();SYSTEM_INFO("SPI_FLASH_Init over------------- \n");


mpu_init();


/* 防止不用NAND时对共用脚影响 */
Disable_NAND_CS();
SDRAM_Init();SYSTEM_INFO("SDRAM_Init over------------- \n");
SDRAM_DMA_Init();
QSPI_FLASH_Init();


#if (USE_LVGL)
lv_init_all();
#endif

#if (!USE_OS)&&USE_TOUCH_IIC
//LCD_Init();
TouchDrvInit();//裸机时才用
#endif




//lv_init_all();
//MX_DMA2D_Init();
//MX_FATFS_Init();
//BSP_usbd_init();

//	
	//ADCx_Init();
//I2C_EE_Init();
//	BASIC_TIM_Init();
//IIC_ConfigInit();	
//	i2c_Soft_CfgGpio();
//SPI_A_Init();	
	
//	Delay(200);
//RelayALL_GPIO_Config();	
//	IN_Io_ALL_GPIO_Config();
//ADC_CC6902_Init();
//	LCD_Init();
//IWDG_Config_nms(EM_iwd_4000ms);

//AppTaskScheduleInit();
//	ADS7822_Init();
//	OLED_Init();
//Delay(1000);

easylogger_init();SYSTEM_INFO("easylogger_init over------------- \n");
//USE_FREERTOS 
#if USE_OS&&USE_LETTER_SHELL



//zn_shell_init();
LetterShell_OS_Init();
//nr_os_shell_init();SYSTEM_INFO("nr_os_shell_init over------------- \n");
#else


#endif


#if USE_LETTER_SHELL&&!USE_OS
letterShell_no_os_WhileInit();SYSTEM_INFO("letterShell_no_os_noWhile_Init over------------- \n");
#endif

//SYSTEM_INFO("is_faultmask_active %d \n",is_faultmask_active());
////__enable_irq();
//__set_FAULTMASK(0);
//SYSTEM_INFO("is_faultmask_active %d \n",is_faultmask_active());

//BatteryInit();

//    SysTick_Config(SystemCoreClock / 1000); 

//    // 配置 SysTick 中断优先级，例如设置为最低优先级
//    NVIC_SetPriority(SysTick_IRQn, 0x0F);


//shell_init();SYSTEM_INFO("shell_init over------------- ");
//CAN_Config_Init();	




SYSTEM_INFO("is_faultmask_active %d \n",is_faultmask_active());
__set_FAULTMASK(0);
SYSTEM_INFO("is_faultmask_active %d \n",is_faultmask_active());
}


void NeedToOnceOperate_Pro(void){
	
TEST_ALL_TEST();
		
//ALLperipheral_SingleTest();
//    while(1)
//    {
//        delay_ms(5);
//        lv_timer_handler();
//    }
#if !(LVGL_OS)
//lv_test();//使用裸机调用LVGL
#endif
//IWDG_Feed();


}

//	RelayCHX_OFF_01To08;
//	RelayCHX_ON_01;
//	ADC_VolcalculateData(ADC_GetValue(ADC_CHANNEL));


void NeedToRepeatOperate_Pro(void){

  AppTaskCb();
  //ALLperipheral_RepetitionTest();
	AppTaskHandler();//通过标识位运行函数
	//TaskSemaConsumeHandler();
	//SYSTEM_INFO("-------------xxx /r/n");
	//Delay(1000);
//Delay(1);
//delay_us_soft(10);
//SYSTEM_DEBUG("xxxx is time:%u ---",(uint32_t)GetSysRunTime());
}

//	uint16_t ic=	ADC_VolcalculateData(ADC_GetValue(ADC_CHANNEL));
//SYSTEM_INFO("\n ADC_VolcalculateData=%d \n",ic);

int32_t BSP_GetVersion(void)
{
  return (int32_t)STM32HFXXX_NUCLEO_BSP_VERSION;
}


/*********************************************END OF FILE**********************/
