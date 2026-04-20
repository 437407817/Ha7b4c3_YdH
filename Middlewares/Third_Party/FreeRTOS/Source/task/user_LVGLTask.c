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
#include "./task/user_LVGLTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "freerotspro.h"  
/* 开发板硬件bsp头文件 */

#include "./sys/sysio.h"

#include "./SEGGER_TOOLKIT.h"
#include "./sys/bsp_systime.h"   




void update_lvgl_data(void);






/**
 * @brief       LVGL运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void vLvglTask(void *pvParameters)
{

		lv_init_all();
		lv_pro_run();
    
    while(1)
    {
			
uint32_t sleep_tick = lv_timer_handler(); // 获取下次任务所需的等待时间
        if(sleep_tick > 5) sleep_tick = 5;       // 强制最高 200Hz 刷新检查
        vTaskDelay(pdMS_TO_TICKS(sleep_tick));
    }
}


#include "./touch/touch_drv.h"

// 1. 定义触摸任务句柄
TaskHandle_t TouchTask_Handle = NULL;
TouchInfo_t g_touchInfo;
// 2. 触摸扫描任务函数
void vTouchTask(void *pvParameters)
{
    // 确保硬件已经初始化（通常已经在 main 中初始化，或者在这里初始化一次）
    // Touch_Init(); 
    TouchDrvInit();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 10ms 扫描一次

    for(;;)
    {
        /* 核心逻辑：直接读取硬件 I2C 寄存器 */
        /* 不要在这里加任何 printf，I2C 速度受限时 printf 会拖慢任务 */
        TouchScan(&g_touchInfo); 
        
        // 按照固定频率唤醒，保证采样率稳定
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}




/**
 * @brief      FreeRTOS Tick 钩子函数
 * @note       在系统Tick中断中调用，用于精确计时
 * @retval     无
 */
void vApplicationTickHook(void)
{
    lv_tick_inc(1);  //代替lv_tick_set_cb((lv_tick_get_cb_t)xTaskGetTickCount);
}









/********************************END OF FILE****************************/
