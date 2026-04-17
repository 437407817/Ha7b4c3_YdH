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

extern STR_GET_VOL_Data_t_unpacked GV_get_vol_real_data;

extern STR_SEND_SETTING_DATA_t GV_send_setting_return_Bigdata;
extern STR_SEND_SETTING_DATA_t GV_send_setting_return_data;

extern uint8_t update_lvgl_02_flag;


//CM7 内核不支持对非对齐地址做 64 位读写
void EndianSwap_BatWorkStatus(STR_GET_VOL_Data_t* pData)
{
    if (pData == NULL) return;

    // 用 uint8_t* 指向 Bat_WorkStatus，逐字节操作（CM7 允许任意地址）
    uint8_t* p = (uint8_t*)&pData->Bat_WorkStatus;

    // 字节 0 <-> 7
    uint8_t temp0 = p[0]; p[0] = p[7]; p[7] = temp0;

    // 字节 1 <-> 6
    uint8_t temp1 = p[1]; p[1] = p[6]; p[6] = temp1;

    // 字节 2 <-> 5
    uint8_t temp2 = p[2]; p[2] = p[5]; p[5] = temp2;

    // 字节 3 <-> 4
    uint8_t temp3 = p[3]; p[3] = p[4]; p[4] = temp3;
}


uint64_t Bat_temp_WorkStatus;

void Getdata485_voldata_process(uint8_t num,uint8_t** p_data){

//VpChange16HL((uint16_t*)*p_data,num);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)*p_data,num,"485 GET REAL DATA 0 %d= ",num);
	
	
//	SYSTEM_DEBUG("---------------%x---",*(*p_data+1));
	memmove(&GV_get_vol_485_232_Bigdata,*p_data,num);
	

	
	
EndianSwap_BatWorkStatus(&GV_get_vol_485_232_Bigdata);
//	EndianSwap_VpChange64HL((uint64_t *)&GV_get_vol_485_232_Bigdata.Bat_WorkStatus);

EndianSwap_VpChange16HL((uint16_t *)&GV_get_vol_485_232_Bigdata.Bat_Vol,BATNUM*2);

	memmove(&GV_get_vol_real_data.Bat_WorkStatus,&GV_get_vol_485_232_Bigdata.Bat_WorkStatus,sizeof(GV_get_vol_485_232_Bigdata)-2);
	GV_get_vol_real_data.ErrorStatus=GV_get_vol_485_232_Bigdata.ErrorStatus;
	
//	memcpy(&Bat_temp_WorkStatus, &GV_get_vol_real_data.Bat_WorkStatus,8);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&Bat_temp_WorkStatus,8,"   lvgl----");
	
//	display_64_values(scroll_view, GV_get_vol_real_data.Bat_Vol, GV_get_vol_real_data.Bat_WorkStatus, 64);
	//释放信号量
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(PRINT_HEX,(uint8_t *)&GV_get_vol_real_data,num,"485 GET REAL DATA = ");
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&GV_get_vol_real_data,num,"485 GET REAL DATA 1= ");
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,(uint8_t *)&GV_get_vol_real_data,num,"485 GET REAL DATA = ");
//	SYSTEM_DEBUG("---------------%x---",*(*p_data+1));
}

void Getdata485_settingdata_process(uint8_t num,uint8_t** p_data){

memmove(&GV_send_setting_return_Bigdata,*p_data,num);

	EndianSwap_VpChange16HL((uint16_t *)&GV_send_setting_return_Bigdata.StopVoltage,6);
	
		memmove(&GV_send_setting_return_data,&GV_send_setting_return_Bigdata,sizeof(GV_send_setting_return_data));
	

	
	
	update_lvgl_02_flag=1;
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&GV_send_setting_return_data,num,"485 GET REAL DATA 2= ");
}


/*********************************************END OF FILE**********************/
