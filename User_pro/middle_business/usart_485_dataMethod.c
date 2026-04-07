/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./middle_business/usart_485_dataMethod.h"
//#include "./usart/usart485verify.h"
#include "./pro_com/usart485verify.h"
#include "./sys/sysio.h"
//#include "./middle_business/usart_485_address.h"
//#include "./middle_business/usart_485_driver.h"
//#include "./global/GV_enum.h" 
#include "./DataConvert/data_convert.h"
#include "./global/GV_variable.h" 
#include "string.h"




extern STR_GET_VOL_Data_t GV_get_vol_485_232_Bigdata;

extern STR_GET_VOL_Data_t GV_get_vol_real_data;



void Getdata485_voldata_process(uint8_t num,uint8_t** p_data){

//VpChange16HL((uint16_t*)*p_data,num);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)*p_data,num,"485 GET REAL DATA 0 %d= ",num);
	
	
//	SYSTEM_DEBUG("---------------%x---",*(*p_data+1));
	memmove(&GV_get_vol_485_232_Bigdata,*p_data,num);
	

	
	
EndianSwap_VpChange64HL_CM7((uint64_t *)&GV_get_vol_485_232_Bigdata);

EndianSwap_VpChange16HL((uint16_t *)&GV_get_vol_485_232_Bigdata.Bat_Vol,BAT_MAX_NUM*2);
	
	memmove(&GV_get_vol_real_data,&GV_get_vol_485_232_Bigdata,sizeof(GV_get_vol_real_data));
	
	
//	display_64_values(scroll_view, GV_get_vol_real_data.Bat_Vol, GV_get_vol_real_data.Bat_WorkStatus, 64);
	//释放信号量
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(PRINT_HEX,(uint8_t *)&GV_get_vol_real_data,num,"485 GET REAL DATA = ");
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&GV_get_vol_real_data,num,"485 GET REAL DATA = ");
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,(uint8_t *)&GV_get_vol_real_data,num,"485 GET REAL DATA = ");
//	SYSTEM_DEBUG("---------------%x---",*(*p_data+1));
}









/*********************************************END OF FILE**********************/
