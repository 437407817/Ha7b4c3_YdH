#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>






#define creat_ticktime(tick_name)	\
    uint32_t start_tick_##tick_name = 0; \
    uint32_t diff_tick_##tick_name = 0; 


#define Cal_start_ticktime(tick_name)	Calculate_StartRunTime( &start_tick_##tick_name)

#define Cal_diff_ticktime(tick_name) SYSTEM_INFO(" %s - %dms\n", __func__,Calculate_diffRunTime(&start_tick_##tick_name,&diff_tick_##tick_name))






/**
***********************************************************
* @brief systick初始化
* @param
* @return 
***********************************************************
*/
void SysTick_Init(void);

/**
***********************************************************
* @brief 获取系统运行时间
* @param
* @return 以1ms为单位
***********************************************************
*/
volatile uint64_t GetSysRunTime(void);

void Calculate_StartRunTime(uint64_t* nowStarttime);
uint64_t Calculate_diffRunTime(uint64_t* nowStarttime,uint64_t* nowOvertime);



/**
***********************************************************
* @brief 注册任务调度回调函数
* @param pFunc, 传入回调函数地址
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void));

#endif
