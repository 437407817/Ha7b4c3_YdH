#ifndef __TASKFUN_H
#define	__TASKFUN_H

//#include "stm32fxxx.h"


#include <stdlib.h>
#include <stdint.h>
#include "./sys/systick.h"
#include "./sys/sysio.h"



enum GateType{
Disable=0,
Enable=1,
};




typedef struct
{
	uint8_t run;                // 调度标志，1：调度，0：挂起
	uint32_t timCount;          // 时间片计数值(如果初始化时为0.则不启动)
	uint32_t timRload;          // 时间片重载值
	uint8_t StartCountGate;                // 启动次数开启，1：开启，0：不开启
	uint32_t startupTimes;            // 启动次数，1：一次后挂起，10：10次后挂起
	void (*pTaskFuncCb)(void);  // 函数指针变量，用来保存业务功能模块函数地址
} TaskComps_t;






 void TaskHandler(void);


 void AppTaskScheduleInit(void);

void SuspendTaskHandler(void (*pc)(void));
void ResumeTaskHandler(void (*pc)(void));

void SetTaskHandlerTimeRload(void (*pc)(void),uint32_t rlotime);
 void SetTaskHandlerTimeRloadAndChangeState(void (*pc)(void),uint32_t rlotime,uint8_t changestate);

void AppTaskCb(void);

 void AppTaskHandler(void);
















#endif /* __TASKFUN_H */

