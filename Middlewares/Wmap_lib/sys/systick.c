#include <stdint.h>
//#include "stm32f10x.h"
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h" 
#include "./stm32_FH_xxx_hal.h"
#include "./Sys/systick.h"
#include "./user_test_config.h"



static volatile uint64_t g_sysRunTime = 0;
//uint64_t g_sysRunTime = 0;
extern volatile uint32_t g_sysRunCount ;

#if 0
static void (*g_pTaskScheduleFunc)(void);          // 函数指针变量，保存任务调度的函数地址

/**
***********************************************************
* @brief 注册任务调度回调函数
* @param pFunc, 传入回调函数地址
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void))
{
	g_pTaskScheduleFunc = pFunc;
}
#endif 
/**
***********************************************************
* @brief systick初始化
* @param
* @return 
***********************************************************
//*/
void SysTick_Init(void)
{

	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
  if (HAL_SYSTICK_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
//	printf("xoxoxo\r\n");
//NVIC_SetPriority(SysTick_IRQn, 0x00U);
}



#include "FreeRTOSConfig.h" 
#include "./sys/sysio.h"
#include "./sys/bsp_systime.h"   
/**
***********************************************************
* @brief 定时中断服务函数，1ms产生一次中断
* @param
* @return 
***********************************************************
*/

#if USE_FREERTOS 
extern void xPortSysTickHandler(void);
#endif


//systick中断服务函数
void SysTick_Handler(void)
{	
	
	#if USE_OS
	HAL_IncTick();//延迟函数HAL_DELAY必备
	#else
//	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	#endif
	

	#if USE_FREERTOS
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
	#else
	
	g_sysRunCount++;
	//g_pTaskScheduleFunc();
	//TimingDelay_Decrement();
	
	//HAL_IncTick();
	
	
	
//	SYSTEM_INFO("-------------%d /r/n",g_sysRunTime);
	//SYSTEM_DEBUG("Han time=%llu  =%llu",g_sysRunTime++,GetSysRunTime());
	//SYSTEM_DEBUG("Han1 time=%llu  ", GetSysRunTime());
	//SYSTEM_DEBUG("Han2 time=%llu  ", g_sysRunTime);
	//printf("Han1 time=%llu  ", GetSysRunTime());
	#endif

	g_sysRunTime++;
	
	
}



void Calculate_StartRunTime(uint64_t* nowStarttime)
{
	*nowStarttime=g_sysRunTime;

	return ;
}

uint64_t Calculate_diffRunTime(uint64_t* nowStarttime,uint64_t* nowOvertime)
{
//	uint32_t printtime;
	*nowOvertime=g_sysRunTime;
//	printtime=*nowOvertime-(*nowStarttime);

	return *nowOvertime-(*nowStarttime);
}

/**
***********************************************************
* @brief 获取系统运行时间
* @param
* @return 以1ms为单位
***********************************************************
*/
volatile uint64_t GetSysRunTime(void)
{
//	SYSTEM_INFO("%lld \r\n",g_sysRunTime);
	return g_sysRunTime;
}
