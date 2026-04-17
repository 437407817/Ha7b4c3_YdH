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
#include "./task/rtos_ProBtnTask.h"


#include "task.h"
#include "queue.h"
#include "freerotspro.h"  
/* 开发板硬件bsp头文件 */

#include "./sys/sysio.h"

#include "./SEGGER_TOOLKIT.h"
#include "./sys/bsp_systime.h"   


#include "./middle_business/usart_485_sendMethod.h"
#include "./rtosprintf/frtos_printf.h"
#include "./global/GV_variable.h" 

 #define LOG_TAG "TASK"
#include "elog.h"
//#include <elog.h>

#include "log.h"



#define v_printf logInfo


//#include "./TaskTest/Task_check.h"

//#include "stdarg.h"


// 全局信号量
SemaphoreHandle_t g_btn_task_sem = NULL;


void btn_data_task(void *pvParameters)
{
	// 创建二进制信号量
g_btn_task_sem = xSemaphoreCreateBinary();
	
    while(1)
    {
        xSemaphoreTake(g_btn_task_sem, portMAX_DELAY);

//        if(GV_send_run_state_data.Machine_RUN == 1)
//        {
//					
//            // 开启运行
//        }
//        else
//        {
//            // 停止运行
//        }
			
			run_stop_mechine(GV_send_run_state_data.Machine_RUN);
			
    }
}

QueueHandle_t g_setting_data_queue = NULL; 

extern STR_SEND_SETTING_DATA_t GV_send_setting_data;


void btn_setting_data_task(void *pvParameters)
{
g_setting_data_queue = xQueueCreate(5, sizeof(STR_SEND_SETTING_DATA_t));


	
    while(1)
    {
        if(g_setting_data_queue != NULL) {
            // 1. 尝试从队列接收数据
            // 参数1: 队列句柄
            // 参数2: 存放接收数据的缓冲区指针
            // 参数3: 等待时间。portMAX_DELAY 表示一直死等，直到有新数据，不占用 CPU
            if(xQueueReceive(g_setting_data_queue, &GV_send_setting_data, portMAX_DELAY) == pdPASS) {
                
                // 2. 接收成功，在这里执行你的逻辑
                v_printf("Task B Received: Auto=%d, StopVol=%d, Dif=%d ,AlaHVol=%d\n", GV_send_setting_data.IsAutoRun,
                        GV_send_setting_data.StopVoltage, 
                        GV_send_setting_data.VoltageDifference ,GV_send_setting_data.AlarmHighVoltage
                        );
							
                shell_settingfun(GV_send_setting_data.IsAutoRun,GV_send_setting_data.StopVoltage,GV_send_setting_data.VoltageDifference,GV_send_setting_data.AlarmHighVoltage);
                // 根据收到的数据修改 STM32 的硬件运行参数...
            }
        }else{
				        // 处理创建失败的情况（通常是内存不足）
        v_printf("Queue Create Failed!\n");
					vTaskDelay(pdMS_TO_TICKS(1000));
				
				}
			
    }
}















/********************************END OF FILE****************************/
