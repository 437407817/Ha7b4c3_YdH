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
#include "./task/rtos_comTask.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "freerotspro.h" 

/* 开发板硬件bsp头文件 */

#include "./sys/sysio.h"

#include "./SEGGER_TOOLKIT.h"
#include "./sys/bsp_systime.h"   



#include "./rtosprintf/frtos_printf.h"
#include "./usart/bsp_usart_dma.h"

 #define LOG_TAG "TASK"
#include "elog.h"
//#include <elog.h>

#include "log.h"


#include "./TaskTest/Task_check.h"




#define v_printf logInfo


//SemaphoreHandle_t g_com_uart_send_sem = NULL;
//SemaphoreHandle_t g_com_nextdata_send_sem = NULL;
SemaphoreHandle_t uart_send_res_sem =  NULL;
extern str_DMA_usart_send GV_usartdmaSend;


void com_send_task(void *pvParameters)
{
	// 创建二进制信号量
//g_com_uart_send_sem = xSemaphoreCreateBinary();
	
uart_send_res_sem =	xSemaphoreCreateCounting(4, 4);
	
//	g_com_nextdata_send_sem = xSemaphoreCreateBinary();
    while(1)
    {
			
			
			
//			if(GV_usartdmaSend.uart_tx_alldatas_sendfinish == 1){
//			
//				xSemaphoreTake(g_com_uart_send_sem, portMAX_DELAY);
//			
//			}else{
//			
//			 Usart_SendFUN_ALL();
//				
////				xSemaphoreTake(g_com_nextdata_send_sem, portMAX_DELAY);
//			vTaskDelay(pdMS_TO_TICKS(TX_WAITTING_TIMEOUT));
//			}
       
			
			        // 申请1个资源，拿不到就阻塞
       xSemaphoreTake(uart_send_res_sem, portMAX_DELAY);
			
			Usart_SendFUN_ALL();
			
			


			
    }
}














/********************************END OF FILE****************************/
