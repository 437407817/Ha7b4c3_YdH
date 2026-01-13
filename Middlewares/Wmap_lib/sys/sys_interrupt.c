/**
  ******************************************************************************
  * @file    bsp_util_time.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  



#include "./sys/sys_interrupt.h"   
//#include "cmsis_armcc.h"
//#include "core_cm4.h"  // 对于 Cortex - M4 内核，其他内核类似

//******************************


int is_interrupt_disabled(void) {
    uint32_t primask = __get_PRIMASK();
    return (primask != 0);
}


int is_faultmask_active(void) {
    uint32_t faultmask = __get_FAULTMASK();
    return (faultmask != 0);
}






// 软件复位函数
void Software_Reset(void) {
	
//	    // 关闭所有中断，防止复位过程被中断干扰
//    __set_FAULTMASK(1); 
//    // 触发系统复位
//    NVIC_SystemReset(); 
	
    // 使能对 AIRCR 的写操作
    __HAL_RCC_AHB1_FORCE_RESET();
    __HAL_RCC_AHB1_RELEASE_RESET();

    // 触发软件复位
    __DSB();
    SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                  (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                   SCB_AIRCR_SYSRESETREQ_Msk);
    __DSB();
    while (1) {
        ;
    }
}









/*********************************************END OF FILE**********************/
