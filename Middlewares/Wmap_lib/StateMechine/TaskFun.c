
/**
  ******************************************************************************
  * @file    TaskFun.c 
  * @author  wmap
  * @version V0.0.0
  * @date    27-12-2024
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
#include "./TaskFun.h"
#include "./TaskSema.h"

//typedef struct
//{
//	uint8_t run;                // 调度标志，1：调度，0：挂起
//	uint32_t timCount;          // 时间片计数值(如果初始化时为0.则不启动)
//	uint32_t timRload;          // 时间片重载值
//	uint8_t startup;                // 启动次数开启，1：开启，0：不开启
//	uint32_t startupTimes;            // 启动次数，1：一次后挂起，10：10次后挂起
//	void (*pTaskFuncCb)(void);  // 函数指针变量，用来保存业务功能模块函数地址
//} TaskComps_t;





extern  TaskComps_t g_taskComps[] ;

static uint8_t  TASK_NUM_MAX;

//#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))





/**
***********************************************************
* @brief 在main-while中调用（2），执行pTaskFuncCb指针函数所指向的功能
* @param
* @return 
***********************************************************
*/
 void TaskHandler(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].run)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].pTaskFuncCb();        // 执行调度业务功能模块
			
				if(g_taskComps[i].StartCountGate){//判断启动次数标识位是否打开
				    g_taskComps[i].startupTimes--;//运行次数减1
					if(g_taskComps[i].startupTimes==0){//判断是否是最后一次运行
					   g_taskComps[i].timCount =0;//停止启动
					   }
				  }
		}
	}
}


 void SuspendTaskHandler(void (*pc)(void))
{
	if(pc==NULL){
		
	return;
	}
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (*(g_taskComps[i].pTaskFuncCb)==*pc)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].timCount=0;        // 执行调度业务功能模块
			return;
		}
	}
}

 void ResumeTaskHandler(void (*pc)(void))
{
	if(pc==NULL){
		
	return;
	}
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (*(g_taskComps[i].pTaskFuncCb)==*pc)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].timCount=g_taskComps[i].timRload;        // 执行调度业务功能模块
			return;
		}
	}
}

//不改变原来的启动运行状态去改变加载时间
 void SetTaskHandlerTimeRload(void (*pc)(void),uint32_t rlotime)
{
	if(pc==NULL){
		
	return;
	}
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (*(g_taskComps[i].pTaskFuncCb)==*pc)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].timRload=rlotime;        // 执行调度业务功能模块
			return;
		}
	}
}

//改变启动运行状态去改变加载时间
//changestate=0:挂起/暂停，changestate=1:恢复，changestate=2，3，4:保持原来状态，
 void SetTaskHandlerTimeRloadAndChangeState(void (*pc)(void),uint32_t rlotime,uint8_t changestate)
{
	if(pc==NULL){
		
	return;
	}
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (*(g_taskComps[i].pTaskFuncCb)==*pc)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].timRload=rlotime;        // 执行调度业务功能模块
			
			if(changestate==0){
			g_taskComps[i].timCount=0;
			}else if(changestate==1){
			g_taskComps[i].timCount=g_taskComps[i].timRload;
			}else{
			
			
			}
			//return;
		}
	}
}
//改变启动次数状态开关，同时去改变改变启动次数//待验证
 void SetTaskHandlerStartupTimesRload(void (*pc)(void),uint8_t rlostartup,uint32_t rlostartuptimes)
{
	if(pc==NULL){
		
	return;
	}
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (*(g_taskComps[i].pTaskFuncCb)==*pc)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].StartCountGate=rlostartup;        // 执行调度业务功能模块
			g_taskComps[i].startupTimes=rlostartuptimes;        // 执行调度业务功能模块			
			return;
		}
	}
}



/**
***********************************************************
* @brief 在定时器中断服务函数中被间接调用，设置时间片标记，
         需要定时器1ms产生1次中断（所有程序一起计数减1，减完后，再判断谁去运行）
* @param
* @return 
***********************************************************
*/
static void TaskScheduleCb(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].timCount)
		{
			g_taskComps[i].timCount--;
			
			
			if (g_taskComps[i].timCount == 0)
			{
				g_taskComps[i].run = 1;
				g_taskComps[i].timCount = g_taskComps[i].timRload;

				
			}
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
 void AppTaskScheduleInit(void)
{

extern uint8_t calculate_TASK_NUM_MAX(void);
	
	 TASK_NUM_MAX = calculate_TASK_NUM_MAX();
#if 0
	TaskScheduleCbReg(TaskScheduleCb);
#endif
	AppSemaTaskScheduleInit();//信号量初始化
	//SYSTEM_INFO("-------------%d /r/n",TASK_NUM_MAX);
}



volatile uint32_t g_sysRunCount=0 ;
/**
***********************************************************
* @brief 在main-while中调用（1），代替TaskScheduleCbReg(TaskScheduleCb);
* @param
* @return 
***********************************************************
*/
void AppTaskCb(void){

	//uint32_t GetSysRunCount(void)
	
	if(g_sysRunCount>0){
//			printf("-");
	g_sysRunCount--;
		TaskScheduleCb();//每秒判断使能运行的函数
		TaskSemaConsumeCb();
	}

}



 void AppTaskHandler(void){
 
 TaskHandler();
 
 TaskSemaConsumeHandler();
 
 }




