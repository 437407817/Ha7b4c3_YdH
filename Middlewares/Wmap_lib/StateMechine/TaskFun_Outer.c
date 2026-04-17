#include "./TaskFun_Outer.h"
#include "./TaskFun.h"
#include "./sys/sysio.h"
#include "./TaskWorkHandle.h"
//#include "./systick.h"

//#include "./usart/bsp_usart.h"
#include "./SEGGER_TOOLKIT.h"
// #include "./lvgl_port.h"
#include "./touch/touch_drv.h"
#include "./user_config.h"
#include "./usart/bsp_usart_dma.h"
#include <elog.h>
 #include "./lvgl_port.h"
 #include "lvgl.h"
 #include "./DataCache/data_heap.h"
 #include "./DataCache/data_stack.h"
 #include "./Sys/systick.h"
 
 
static void Handle_test(void);
static void Handle_test2(void);

TaskComps_t g_taskComps[] = 
{
	
# if 1
	{0, 1000,   1000, Disable,0 , SEGGER_HostDataToUsart},

	
#endif		
# if 0
	{0, 1000,   1000,   Handle_CheckStop_CommuOverTimeFalg},
	{0, 1000, 1000, Handle_SendVolDataToScreenAndAskSlave},
	{0, 100,   100,   pull_data_from_screen},
	{0, 0,   4000,   Handle_SendCapacityDataToScreen},
  {0, 0,   900,   Handle_SendSDTimeDataToScreen},
	{0, 0,   150,   Handle_usart},
	
	{0, 1000,   1000,   SEGGER_HostDataToUsart},
	{0, 0,   99999999,   Handle_SaveSdData},	
	{0, 0,   150,   Handle_ImmediateSaveSd},	

  {0, 1,   1,   pull_data_from_485},	
	{0, 1000,   1000,   Handle_FeedIWatchDog},	
#endif	
//	 {0, 1000,   5000, Disable,0 , Handle_DWT},
   {0, 1000,   3000, Disable,0 , Handle_usart},
	 {0, 20000,   20000,  Enable,2,Handle_test },	
	 {0, 2000,   8000,  Disable,10,Handle_test2},	
	 	 {0, 1000,   1000,  Disable,10,Handle_led},	
//		 {0, 2000,   2000,  Disable,10,Handle_DMA_USART_Save},	
//		  {0, 2000,   200,  Disable,10,Handle_DMA_USART_Send},
#if !(USE_LVGL_OS)&&(USE_LVGL)
		{0, 30,   LV_DEF_REFR_PERIOD, Disable,0 , Handle_lv},//使用裸机调用LVGL
#endif
//	{0, 1000,   1000, Disable,0 , touch_drv_while_test},//使用裸机调用触摸屏测试
	 //{0, 2000,   3000,  EM_SC_Disable,10,Handle_watchdog_feed},	
	/* 添加业务功能模块 */
		 #if TESTUsartDMASendSaveAndSend
		 {0, 2000,   2000,  Disable,10,Handle_DMA_USART_Save},	//多任务测试效果最佳
		  {0, 2000,   200,  Disable,10,Handle_DMA_USART_Send},  //多任务测试效果最佳
		#endif
		 
		 #if USE_LETTER_SHELL&&(!USE_OS)
		 {0, 10,   10,  Disable,10,Handle_letter_Shell_NoOsWhile},	
		 #endif
		 
		 #if (!USE_OS)&&USE_TOUCH_IIC
		  {0, 200,   500,  Disable,10,Handle_IIC_TOUCH},	
		#endif
		 #if TEST_SHELL_UART
		 {0, 1000,   1000, Disable,0 , Test_USART_SHELL_while},
		 #endif
//		 	{0, 5000,   10000,  Disable,10,PrintHeapUsageRate},	
//		 	{0, 5000,   10000,  Disable,10,stack_check_usage},	
};


//extern uint8_t calculate_TASK_NUM_MAX( void );

 uint8_t calculate_TASK_NUM_MAX(void)
{
	//Usb2ComAppInit();
	return (sizeof(g_taskComps) / sizeof(g_taskComps[0]));

}

//static void Handle_test3(unsigned int num);
/**
* @brief  50ms
  * @param  
  * @retval 无
  */
static void Handle_test(void){
    log_a("Hello EasyLogger!");
    log_e("Hello EasyLogger!");
    log_w("Hello EasyLogger!");
    log_i("Hello EasyLogger!");
    log_d("Hello EasyLogger!");
    log_v("Hello EasyLogger!");
	uint32_t i;
	i=(uint32_t)GetSysRunTime();
	
	SYSTEM_DEBUG_BINARY(i,"BINARY");
//stack_check_usage();

	SYSTEM_DEBUG("Handle_test is time:%u ---",i);

}

 void Handle_test2(void){
static uint16_t i=0;
	uint64_t ii=0;
	ii=(uint64_t)GetSysRunTime();
//	SYSTEM_DEBUG("Handle_test2 is i=%d---%llu",i++,ii);
//	 SYSTEM_DEBUG("Handle_test2 is i=%d---0x%08X%08X", i++, (uint32_t)(ii >> 32), (uint32_t)ii);
//	
//	SYSTEM_BIGNUM_HEX(ii,"");
	SYSTEM_DEBUG("");
  SYSTEM_BIGNUM_DEC(1,ii,"Handle_test2 is i=%d  ",i++);
//	printf("Handle_test2 is i=%d---%llu",i++,ii);
//printf("xxxxxxxxxxxxxxxxx------------- ");
}





