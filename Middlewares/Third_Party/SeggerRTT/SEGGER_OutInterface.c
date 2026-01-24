#include "./SEGGER_OutInterface.h"
#include "./SEGGER_RTT.h"

#include "./sys/bsp_systime.h"   
#include "string.h"
#include <stdlib.h>
#include <stdlib.h>


//#include "stm32_FH_xxx.h"
#include "stm32_FH_xxx_hal.h"

 #include "./sys/sysio.h"

// #include "./io/OUT_io/bsp_gpio_out.h" 

// #include "./converter/data_convert.h"

// #include "./TaskSema.h"
// #include "./TaskSemaFun_Outer.h"
// //#include "./iwdg/bsp_iwdg.h"   
// /**
//   * @brief  切换采集通道channel for collection
//   */

// #include "sys_dwt.h"
// #include "./pwr/bsp_pwr.h"   

// #include "./WindowCmd/Fatfs_cmd.h"   
// #include "./test/bsp_test_spi.h" 
 
// #include "./WindowCmd/IIC_cmd.h"   
// #include "./test/bsp_test_iic.h"  
// #include "nr_shell_cfg.h" 
// #include "shell_port.h"

//#include "./crypt/bsp_encode.h"


//#include "./WindowCmd/IIC_cmd.h"   

//#include "./WindowCmd/Fatfs_cmd.h"   
//#include "./test/bsp_fatfs_sdram_test.h"

#include "./TaskTest/Task_check.h"
/**
  * @brief  切换采集通道channel for collection
  * @param  *i 当前通道第几个的状态地址  *openstate 1开启轮训转换采集 0 关闭所有通道
  * @retval 无
  */





 void segger_outfun(uint8_t * tmp,uint16_t retlen){

SYSTEM_INFO("implement this function.\r\n");

	 
	 		switch(tmp[1]){

       case 01:uart_unlock_task();SYSTEM_DEBUG("uart_unlock_task ");break;
 			case 02:uart_reableIRQ_task();SYSTEM_DEBUG("uart_reableIRQ_task  start");break;
 			case 03: USART_Shell_UartDeInit();SYSTEM_DEBUG("void USART_Shell_UartDeInit(void)");break;
 			case 04:uart_reable_task();SYSTEM_DEBUG("uart_reable_task");break;

       case 05:uart_debug_error();SYSTEM_DEBUG("uart_debug_error");break;
 			case 06:uart_debug_fixerror();SYSTEM_DEBUG("uart_debug_fixerror");break;
// 			case 07:EnableSemaTaskHandler2(HandleSema_test);SYSTEM_DEBUG("EnableSemaTaskHandler");break;
// 			case 0x08:DisableSemaTaskHandler2(HandleSema_test);SYSTEM_DEBUG("DisableSemaTaskHandler");break;
// 			case 0x09:SYSTEM_DEBUG("dwt_delay_ms");dwt_delay_ms(3000);break;
// 			case 0x0A:SYSTEM_DEBUG("EnterSleepMode");EnterSleepMode();break;				
// 			case 0x0B:SYSTEM_DEBUG("EnterStopMode");EnterStopMode();break;					
// 			case 0x0C:SYSTEM_DEBUG("EnterStandbyMode");EnterStandbyMode();break;					
//			case 0x0D:SYSTEM_DEBUG("fatfs_window_cmd");SDIO_SD_Test2();break;	
// 			case 0x0E:SYSTEM_DEBUG("SDIO_SD_Test");SDIO_SD_Test();break;						
// 			case 0x0F:SYSTEM_DEBUG("SPI_Test2");SPI_FLASH_Test();break;		
// 			case 0x10:SYSTEM_DEBUG("I2C_Test");I2C_Test();break;		
// 			case 0x11:SYSTEM_DEBUG("IIC_window_cmd");IIC_window_cmd();break;		
// 			case 0x12:SYSTEM_DEBUG("SDRAM_RNG_generate");SDRAM_Test2();break;		
// 			case 0x13:SYSTEM_DEBUG("SPI_Test6");FATFS_SDRAM_Test();break;	
//			case 0x14:SYSTEM_DEBUG("IIC_window_cmd");fatfs_window_cmd();break;	
//			case 0x15:SYSTEM_DEBUG("saveEEpromChip");saveEEpromChip();break;
// //			case 0x16:SYSTEM_DEBUG("");nr_shell_NoOsWhileTask();break;	
// 			case 0x17:SYSTEM_DEBUG("letter_Shell_NoOsWhileTask");letter_Shell_NoOsWhileTask();break;				
			
//			case 02:IWDG_Config_nms(EM_iwd_2000ms);SYSTEM_DEBUG("EM_iwd_2000ms");break;
//			
//			case 03:IWDG_Config_nms(EM_iwd_4000ms);SYSTEM_DEBUG("EM_iwd_4000ms");break;
//			case 04:SuspendTaskHandler(&Handle_FeedIWatchDog);SYSTEM_DEBUG("SuspendTaskHandler_Handle_FeedIWatchDog");break;
//			
//			case 05:IWDG_Config_nms(EM_iwd_4000ms);SYSTEM_DEBUG("EM_iwd_4000ms");break;				
//			case 06:ResumeTaskHandler(&Handle_FeedIWatchDog);SYSTEM_DEBUG("ResumeTaskHandler_Handle_FeedIWatchDog");break;	
//				
//				
//			case 07:SD_BeginSave_Fun();break;
//			//case 0x08:sd_usart_cmd_SetData( "串充","03:59:55",1234, 34567, 1);break;
// 
//case 0x08: IIC_window_cmd();break;
//			case 0x09: saveEEpromChip();break;
//			case 0x0a: SaveSYS_SettingEEpromData();break;
			
			
			default:break;
		}
	 
	 

}




















/*********************************************END OF FILE**********************/
