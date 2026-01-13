#ifndef __TASKSEMA_H
#define	__TASKSEMA_H

//#include "stm32fxxx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "./sys/systick.h"
#include "./sys/sysio.h"



typedef struct
{
	uint8_t run;                // 调度标志，1：开启，0：关闭

	char Sema_name[10];     //任务名

	uint32_t Sema_count;      // 任务信号的当前计数值，0：，1：，
	uint32_t Sema_total;      // 任务信号的总计数值，0：，1：，
	uint8_t  countdown_run;   	//开启后的运行过程中的标识，如果为1，有信号后立刻运行一次，如果为0，需要等待信号消耗时间到才能运行一次
	uint32_t Sema_consume_timCount;      // 任务信号消耗间隔时间：，	
	uint32_t Sema_consume_timRload;      // 任务信号消耗间隔时间：，
	//uint32_t Sema_takeOrgive; // 任务信号的释放或发送标值，0：take，1：give，
	void (*pTaskSemaGetFuncCb)(void);  // 函数指针变量，用来保存业务功能模块函数地址
} TaskSemaphore_t;

 void GiveSemaTaskHandler(void (*pTask)(void),uint8_t Sema_open_t);
 void GiveSemaTaskHandler2(char* name_str,uint8_t Sema_open_t);
 void TaskSemaConsumeHandler(void);

 void EnableSemaTaskHandler2(char* name_str);
 void DisableSemaTaskHandler2(char* name_str);

 void EnableSemaTaskHandler(void (*pTask)(void));
 void DisableSemaTaskHandler(void (*pTask)(void));


 void AppSemaTaskScheduleInit(void);

void TaskSemaConsumeCb(void);
















#endif /* __TASKFUN_H */

