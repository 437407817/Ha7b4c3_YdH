
/**
  ******************************************************************************
  * @file    TaskSema.c 
  * @author  wmap
  * @version V1.0.1
  * @date    09-03-2025
  * @brief   任务调度.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "./TaskSema.h"
#include <string.h>
#include "./sys/sysio.h"
#include <stdio.h>

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




extern  TaskSemaphore_t g_taskSemaphore[] ;

static uint8_t  TASK_SEMA_NUM_MAX;

//#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))





/**
***********************************************************
* @brief 在main-while中调用（2），执行pTaskFuncCb指针函数所指向的功能
* @param（所有程序分别计数减1，谁先减完谁去运行）
* @return 
***********************************************************
*/
 void TaskSemaConsumeHandler(void)
{
	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if ((g_taskSemaphore[i].run==1)&&(g_taskSemaphore[i].countdown_run==1))                  // 判断时间片标志
		{

			if(g_taskSemaphore[i].Sema_count!=0){
			g_taskSemaphore[i].Sema_count--;
			g_taskSemaphore[i].pTaskSemaGetFuncCb();        // 执行调度业务功能模块
				g_taskSemaphore[i].countdown_run=0;
				SYSTEM_INFO("Take Semaphore : %s, rest count is %d/r/n",g_taskSemaphore[i].Sema_name,g_taskSemaphore[i].Sema_count);	
				if(g_taskSemaphore[i].Sema_count==0){
				g_taskSemaphore[i].run=0;
				
				}
				
				
			}
		}
	}
}



void TaskSemaConsumeCb(void)
{
	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		//SYSTEM_INFO("-------------%d /r/n",i);
		if ((g_taskSemaphore[i].run==1)&&(g_taskSemaphore[i].countdown_run==0)) {
			
				if (g_taskSemaphore[i].Sema_consume_timCount)
		     {
			   g_taskSemaphore[i].Sema_consume_timCount--;

			if (g_taskSemaphore[i].Sema_consume_timCount == 0)
			{
				g_taskSemaphore[i].countdown_run = 1;
				g_taskSemaphore[i].Sema_consume_timCount = g_taskSemaphore[i].Sema_consume_timRload;
			}
		}
		
		
		}

	}
}












//释放信号
//Sema_num_t:信号的号标识，0则以字符串名标识为主，name_str:信号的字符串名标识，Sema_open_t=1，为开启,直接消耗，0，则不开启
 void GiveSemaTaskHandler2(char* name_str,uint8_t Sema_open_t)
{

	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if (strcmp(g_taskSemaphore[i].Sema_name, name_str)==0)                  // 判断时间片标志
		{
			if( g_taskSemaphore[i].Sema_count < g_taskSemaphore[i].Sema_total){
			    g_taskSemaphore[i].Sema_count ++; 
				
				
				SYSTEM_INFO("Give Semaphore : %s /r/n",g_taskSemaphore[i].Sema_name);				
				if(Sema_open_t==1){
				  g_taskSemaphore[i].run=1;				// 			
				}else if(Sema_open_t==0){
				 g_taskSemaphore[i].run=0;
				}

			}else{
			
			SYSTEM_INFO(": %s give Semaphore to max is %d, Give FAILED /r/n",g_taskSemaphore[i].Sema_name,g_taskSemaphore[i].Sema_count);		
			}


			//return;
		}
	}
}

 void GiveSemaTaskHandler(void (*pTask)(void),uint8_t Sema_open_t)
{

	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if (g_taskSemaphore[i].pTaskSemaGetFuncCb==pTask)                  // 判断时间片标志
		{
			if( g_taskSemaphore[i].Sema_count < g_taskSemaphore[i].Sema_total){
			    g_taskSemaphore[i].Sema_count ++; 
				
				
				SYSTEM_INFO("Give Semaphore : %s /r/n",g_taskSemaphore[i].Sema_name);				
				if(Sema_open_t==1){
				  g_taskSemaphore[i].run=1;				// 			
				}else if(Sema_open_t==0){
				 g_taskSemaphore[i].run=0;
				}

			}else{
			
			SYSTEM_INFO(": %s give Semaphore to max is %d, Give FAILED /r/n",g_taskSemaphore[i].Sema_name,g_taskSemaphore[i].Sema_count);	
			}


			//return;
		}
	}
}




//使能信号
//Sema_num_t:信号的号标识，0则以字符串名标识为主，name_str:信号的字符串名标识，
 void EnableSemaTaskHandler2(char* name_str)
{
	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if (strcmp(g_taskSemaphore[i].Sema_name, name_str)==0)                  // 判断时间片标志
		{
			g_taskSemaphore[i].run=1;           // 
     SYSTEM_INFO("EnableSemaTaskHandler = %s /r/n",g_taskSemaphore[i].Sema_name);
			//return;
		}
	}
}

 void EnableSemaTaskHandler(void (*pTask)(void))
{
	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if (g_taskSemaphore[i].pTaskSemaGetFuncCb==pTask)                  // 判断时间片标志
		{
			g_taskSemaphore[i].run=1;           // 
     SYSTEM_INFO("EnableSemaTaskHandler = %s /r/n",g_taskSemaphore[i].Sema_name);
			//return;
		}
	}
}



//关闭信号
//Sema_num_t:信号的号标识，0则以字符串名标识为主，name_str:信号的字符串名标识，
 void DisableSemaTaskHandler2(char* name_str)
{
	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if (strcmp(g_taskSemaphore[i].Sema_name, name_str)==0)                  // 判断时间片标志
		{
			g_taskSemaphore[i].run=0;           // 
     SYSTEM_INFO("DisableSemaTaskHandler = %s /r/n",g_taskSemaphore[i].Sema_name);
			//return;
		}
	}
}

 void DisableSemaTaskHandler(void (*pTask)(void))
{
	for (uint8_t i = 0; i < TASK_SEMA_NUM_MAX; i++)
	{
		if (g_taskSemaphore[i].pTaskSemaGetFuncCb==pTask)                  // 判断时间片标志
		{
			g_taskSemaphore[i].run=0;           // 
     SYSTEM_INFO("DisableSemaTaskHandler = %s /r/n",g_taskSemaphore[i].Sema_name);
			//return;
		}
	}
}



/**
***********************************************************
* @brief 在main-while-before调用（0），初始化
* @param
* @return 
***********************************************************
*/
 void AppSemaTaskScheduleInit(void)
{

extern uint8_t calculate_TASKSema_NUM_MAX( void );
	
	 TASK_SEMA_NUM_MAX = calculate_TASKSema_NUM_MAX();

	
	//SYSTEM_INFO("-------------%d /r/n",TASK_NUM_MAX);
}




































