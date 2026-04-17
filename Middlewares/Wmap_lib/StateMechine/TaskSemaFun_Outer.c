/**
  ******************************************************************************
  * @file    TaskSemaFun_Outer.c 
  * @author  wmap Team
  * @version V1.0.1
  * @date    09-03-2025
  * @brief   任务调度
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 0 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */









#include "./TaskSemaFun_Outer.h"
#include "./TaskFun.h"
#include "./TaskSema.h"
#include "./sys/sysio.h"
#include "./TaskWorkHandle.h"







//typedef struct
//{
//	uint8_t run;                // 调度标志，1：开启，0：关闭

//	char Sema_name[10];     //任务名

//	uint32_t Sema_count;      // 任务信号的当前计数值，0：，1：，
//	uint32_t Sema_total;      // 任务信号的总计数值，0：，1：，
//	uint8_t  countdown_run;   	
//	uint32_t Sema_consume_timCount;      // 任务信号消耗间隔时间：，	
//	uint32_t Sema_consume_timRload;      // 任务信号消耗间隔时间：，
//	//uint32_t Sema_takeOrgive; // 任务信号的释放或发送标值，0：take，1：give，
//	void (*pTaskSemaGetFuncCb)(void);  // 函数指针变量，用来保存业务功能模块函数地址
//} TaskSemaphore_t;

TaskSemaphore_t g_taskSemaphore[] = 
{
# if 1
	{0, "test1",  0,3,0,5000,5000,HandleSema_test},
	{1, "test2",  0,1,1,2000,2000,HandleSema_test2},
#endif	
	/* 添加业务功能模块 */
};

//extern uint8_t calculate_TASKSema_NUM_MAX( void );

 uint8_t calculate_TASKSema_NUM_MAX(void)
{
	//Usb2ComAppInit();
	return (sizeof(g_taskSemaphore) / sizeof(g_taskSemaphore[0]));

}


/**
* @brief  50ms
  * @param  
  * @retval 无
  */
 void HandleSema_test(void){

	SYSTEM_DEBUG("HandleSema_test consume success /r/n");

}

 void HandleSema_test2(void){
static uint16_t i=0;
	SYSTEM_DEBUG("HandleSema_test2 consume success i=%d /r/n",i++);

}


