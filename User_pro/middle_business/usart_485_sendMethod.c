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
  
#include "./middle_business/usart_485_sendMethod.h"
//#include "./usart/usart485verify.h"
#include "./pro_com/usart485verify.h"
#include "./sys/sysio.h"
//#include "./middle_business/usart_485_address.h"
//#include "./middle_business/usart_485_driver.h"
//#include "./global/GV_enum.h" 
#include "./DataConvert/data_convert.h"
#include "./global/GV_variable.h" 
#include "string.h"
#include "./pro_com/usart_485_232_send.h"

#include "./global/GV_enum.h" 

#define LOG_TAG "COM_INPUT"
#include "elog.h"
#include "log.h"
#define v_printf logInfo





extern STR_SEND_RUN_DATA_t GV_send_run_state_data;
extern STR_SEND_RUN_DATA_t GV_send_run_state_bigdata;

extern STR_SEND_SETTING_DATA_t GV_send_setting_data;
extern STR_SEND_SETTING_DATA_t GV_send_setting_bigdata;

void run_machine(void){
	
	GV_send_run_state_data.Machine_RUN=1;
	
memmove(&GV_send_run_state_bigdata,&GV_send_run_state_data,2);
EndianSwap_VpChange16HL((uint16_t *)&GV_send_run_state_bigdata.Machine_RUN,2);
com_DW_write_cmd_print((uint16_t)EM_SEND_RUNSTOP_ADDRESS,(uint16_t *)&GV_send_run_state_bigdata,sizeof(GV_send_run_state_bigdata)/2,1);
	

}



void stop_machine(void){
	
	GV_send_run_state_data.Machine_RUN=0;
	
memmove(&GV_send_run_state_bigdata,&GV_send_run_state_data,2);
EndianSwap_VpChange16HL((uint16_t *)&GV_send_run_state_bigdata.Machine_RUN,2);
com_DW_write_cmd_print((uint16_t)EM_SEND_RUNSTOP_ADDRESS,(uint16_t *)&GV_send_run_state_bigdata,sizeof(GV_send_run_state_bigdata)/2,1);
	
	
}


void run_stop_mechine(uint8_t run){
	GV_send_run_state_data.Machine_RUN=run;
	
memmove(&GV_send_run_state_bigdata,&GV_send_run_state_data,2);
EndianSwap_VpChange16HL((uint16_t *)&GV_send_run_state_bigdata.Machine_RUN,2);
com_DW_write_cmd_print((uint16_t)EM_SEND_RUNSTOP_ADDRESS,(uint16_t *)&GV_send_run_state_bigdata,sizeof(GV_send_run_state_bigdata)/2,1);
	
v_printf("run stop = %d",run);

}





#include "shell.h"
int shell_runfunc(int i)
{
	if(i==1){
	run_machine();
	}else{
	stop_machine();
	
	}
    v_printf("runfunc input int: %d, \r\n", i);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), rf, shell_runfunc, test01);









void shell_settingfun(int iar,int sv,int sd){
GV_send_setting_data.IsAutoRun=iar;
GV_send_setting_data.StopVoltage=sv;	
GV_send_setting_data.VoltageDifference=sd;
v_printf("shell_settingfun input int: %d, %x, %x, \r\n", iar ,sv,sd);
	memmove(&GV_send_setting_bigdata,&GV_send_setting_data,sizeof(GV_send_setting_data));
EndianSwap_VpChange16HL((uint16_t *)&GV_send_setting_bigdata.IsAutoRun,2);

com_DW_write_cmd_print((uint16_t)EM_SEND_SETTING_ADDRESS,(uint16_t *)&GV_send_setting_bigdata,sizeof(GV_send_setting_bigdata)/2,1);
	
	
	
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), sf, shell_settingfun, test01);


/*********************************************END OF FILE**********************/
