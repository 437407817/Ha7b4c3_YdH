/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS V9.0.0  + STM32 任务管理
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "./task/user_TestTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "freerotspro.h"  
/* 开发板硬件bsp头文件 */

#include "./sys/sysio.h"

#include "./SEGGER_TOOLKIT.h"
#include "./sys/bsp_systime.h"   


#include "./DataConvert/data_random.h"
#include "./DataConvert/data_set.h"
#include "./global/GV_variable.h" 


#include "shell.h"

#define LOG_TAG "USER_TEST"

#include "elog.h"

#include "log.h"
#define v_printf logInfo


//Bat_Vol
extern STR_GET_VOL_Data_t GV_get_vol_real_data;
extern STR_GET_VOL_Data_t GV_get_vol_485_232_Bigdata;
 STR_GET_VOL_Data_t GV_get_vol_485_232_Bigdata3;
 int8_t RNG_SelectOpByWeight2(void){
 
 
 return 10;
 }
	uint16_t aa[BAT_MAX_NUM];
 
extern uint8_t update_flag;
void rdatasettask(void *pvParameters){

	MX_RNG_Init(); 
	Weight_RANDOM_INIT();

	uint16_t i=0;
   while(1)
    {
log_d("start RANDOM sample task");

			setInt16Array((int16_t *)GV_get_vol_real_data.Bat_Vol,BAT_MAX_NUM,RNG_SelectOpByWeight2);
//			memset(GV_get_vol_485_232_Bigdata3.Bat_Vol, ++i, BAT_MAX_NUM * sizeof(int16_t));
//			for(int i=0; i<BAT_MAX_NUM; i++)
//{
//    GV_get_vol_485_232_Bigdata3.Bat_Vol[i] = 100;
//}
//				memset(aa, ++i, BAT_MAX_NUM );
			GV_get_vol_real_data.Bat_Vol[2]--;
			GV_get_vol_real_data.Bat_Vol[3]++;
			GV_get_vol_real_data.Bat_WorkStatus =++i;
			GV_get_vol_real_data.Bat_Vol[59]+=3;
			GV_get_vol_real_data.Bat_Vol[60]-=3;
			v_printf(" = %d",GV_get_vol_real_data.Bat_Vol[1]);
			
			 update_flag=1;
//			setInt16Array((int16_t *)&GV_get_vol_real_data.Bat_Vol,BAT_MAX_NUM,1);
			SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)&GV_get_vol_real_data.Bat_WorkStatus,sizeof(GV_get_vol_real_data),"xxxx");
//			SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)aa,sizeof(aa)*2,"xxxx");
			 vTaskDelay(pdMS_TO_TICKS(3000));
		}

}




#include "./hook/hook_mem.h"


void rds(int argc,char **argv)
{
	TaskHandle_t RAN_handle;
	BaseType_t xReturn = pdPASS;
 log_d("start RANDOM sample task");
	
 xTaskCreate(rdatasettask,"rdatasettask",2048,(void *)0,2,&RAN_handle);
							
			vPrintStack_TaskCreationResult("rdatasettask", xReturn, 2048);			
 
}
//ZNS_CMD_EXPORT(smst,start mock sample task)
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), rds, rds, test random task);







/********************************END OF FILE****************************/
