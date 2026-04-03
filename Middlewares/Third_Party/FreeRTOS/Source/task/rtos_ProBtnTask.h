


#ifndef __RTOS_PROBTNTASK_H
#define	__RTOS_PROBTNTASK_H




#include "./stm32_FH_xxx_hal.h"                // Device header
#include "FreeRTOS.h"
#include "semphr.h"









void btn_data_task(void *pvParameters);
void btn_setting_data_task(void *pvParameters);

extern SemaphoreHandle_t g_btn_task_sem;
extern QueueHandle_t g_setting_data_queue; // 引用外部定义的句柄








#endif
