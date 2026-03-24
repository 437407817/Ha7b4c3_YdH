


#include "./communication/SD_485DataFun.h"

#include "./sys/bsp_systime.h"  
#include "./defines.h"
#include "./GV_variable.h"
#include "./usart/bsp_usart_sd.h"
#include "./sys/sysio.h"
#include "./service/EepromFun.h"

#include "./communication/SD_485Dis.h"
#include "./converter/cvt_time.h"
#include "./controller/TaskSingleWork.h"
#include "./controller/TaskFun.h"
#include "./sys/bsp_random.h" 
#include "./usart/bsp_usart_sd.h"
extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 
extern str_ScreenSettingDataState GV_ScreenSettingDataState;


extern str_BatterySetCalibration GV_BatterySetCalibration;

extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

extern str_CommSlaveAskState GV_CommSlaveAskState;

//extern str_CommSendToSlaveBoardState BigD_GV_CommSlaveBoardState;

extern str_BatterySetCalibration BigD_GV_BatterySetCalibration;


extern str_ComuBoardState GV_ComuBoardState;

extern str_SD_485_ITEM_t GV_SD_485_ITEM;

extern str_Screen_SYS_SettingDataState Screen_SYS_SettingDataState;


extern str_Screen_SYS_SHOW_SettingDataState Screen_SYS_SHOW_SettingDataState;

extern str_EEprom_SYS_SettingDataState TMP_EEprom_SYS_SettingState;

//void sd_usart_cmd_SetTitle_bynum(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second,char* workmode)

extern str_ScreenSettingDataState GV_ScreenSettingDataState;
//extern str_Screen_SYS_SHOW_SettingDataState Screen_SYS_SHOW_SettingDataState;
//extern str_ComuBoardState GV_ComuBoardState;





extern  str_SD_data GV_SD_data;


void SD_BeginSave_Fun(void){
#if 1
//	sd_usart_cmd_ReadTime();//------
//			GetSYS_SettingEEpromData();
	//GV_ScreenSetRunBoardState.Set_AllUpperLimitParallelVol
	
//	memcpy(&Screen_SYS_SettingDataState.sys_sd_save_hour,&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour,EEPROM_SYS_SettingSaveDataCount);
	
//	delay_ms(100);
//	pull_data_from_SD_485();
//	pull_data_from_SD_485();
//	delay_ms(10);
//	SYSTEM_DEBUG("  %d-%d-%d,%d-%d-%d\n",GV_SD_485_time_ITEM.sd_year,GV_SD_485_time_ITEM.sd_month,GV_SD_485_time_ITEM.sd_day,GV_SD_485_time_ITEM.sd_hour,GV_SD_485_time_ITEM.sd_minute,GV_SD_485_time_ITEM.sd_second);
//sd_usart_cmd_SetTitle_bynum(GV_SD_485_time_ITEM.sd_year,GV_SD_485_time_ITEM.sd_month,GV_SD_485_time_ITEM.sd_day,GV_SD_485_time_ITEM.sd_hour,GV_SD_485_time_ITEM.sd_minute,GV_SD_485_time_ITEM.sd_second,"开始充电");
//	GV_SD_485_time_ITEM.sd_begin_timestamp=CalculateTimestamp(GV_SD_485_time_ITEM.sd_year,GV_SD_485_time_ITEM.sd_month,GV_SD_485_time_ITEM.sd_day,GV_SD_485_time_ITEM.sd_hour,GV_SD_485_time_ITEM.sd_minute,GV_SD_485_time_ITEM.sd_second);
	sd_usart_cmd_SetTitle_bystr("存储数据");
  delay_ms(10);
	pull_data_from_SD_485();
	sd_usart_cmd_SetItem();
	delay_ms(10);
	pull_data_from_SD_485();
SYSTEM_DEBUG("SD Save_Fun OK/n");
GV_SD_data.SD_launch_flag=1;
//uint32_t 	TMP_seconds;
//TMP_seconds = TMP_EEprom_SYS_SettingState.EEP_DS_save_hour*3600+TMP_EEprom_SYS_SettingState.EEP_DS_save_minute*60+TMP_EEprom_SYS_SettingState.EEP_DS_save_second;	
	
//	if(TMP_seconds==0){
//		SYSTEM_DEBUG("TMP_seconds==0/n");
//	return;
//	}
	
	
	
//	if(TMP_seconds<=10){
//	TMP_seconds=10;
//		
//	}
//SetTaskHandlerTimeRload(&Handle_SaveSdData,(TMP_seconds)*1000);
//ResumeTaskHandler(&Handle_SaveSdData);
//Handle_SaveSdData();
#endif
SD_Save_Fun();
}


//time_t CalculateTimestamp(uint16_t t_year,uint16_t t_month,uint16_t t_day,uint16_t t_hour,uint16_t t_minute,uint16_t t_second);
//time_t CalculateTimestampDifference(time_t T_seconds,uint16_t t_year,uint16_t t_month,uint16_t t_day,uint16_t t_hour,uint16_t t_minute,uint16_t t_second);
//void CalculateTimestampToTime(time_t seconds,uint16_t* t_year,uint16_t* t_month,uint16_t* t_day,uint16_t* t_hour,uint16_t* t_minute,uint16_t* t_second);



#if 1

void SD_Save_Fun(void){




	if(GV_ScreenSetRunBoardState.Set_NowRunState==0){//没运行，测试
//		delay_ms(10);
sd_usart_cmd_ReadTime();
//		delay_ms(80);
//pull_data_from_SD_485();pull_data_from_SD_485();
		
}
	
	
	
//GV_SD_485_time_ITEM.sd_TimestampDifference=CalculateTimestampDifference(GV_SD_485_time_ITEM.sd_begin_timestamp,
//	GV_SD_485_time_ITEM.sd_year,GV_SD_485_time_ITEM.sd_month,GV_SD_485_time_ITEM.sd_day,GV_SD_485_time_ITEM.sd_hour,GV_SD_485_time_ITEM.sd_minute,GV_SD_485_time_ITEM.sd_second);
//SYSTEM_DEBUG(" CD %d-%d-%d,%d-%d-%d\n",GV_SD_485_time_ITEM.sd_year,GV_SD_485_time_ITEM.sd_month,GV_SD_485_time_ITEM.sd_day,GV_SD_485_time_ITEM.sd_hour,GV_SD_485_time_ITEM.sd_minute,GV_SD_485_time_ITEM.sd_second);
	


//CalculateTimestampToTime(GV_SD_485_time_ITEM.sd_TimestampDifference,
//	&GV_SD_485_time_ITEM.sd_cal_year,&GV_SD_485_time_ITEM.sd_cal_month,&GV_SD_485_time_ITEM.sd_cal_day,&GV_SD_485_time_ITEM.sd_cal_hour,&GV_SD_485_time_ITEM.sd_cal_minute,&GV_SD_485_time_ITEM.sd_cal_second);
  

//SYSTEM_DEBUG(" CAL %d %d-%d-%d,%d-%d-%d\n",GV_SD_485_time_ITEM.sd_TimestampDifference,GV_SD_485_time_ITEM.sd_cal_year,GV_SD_485_time_ITEM.sd_cal_month,GV_SD_485_time_ITEM.sd_cal_day,
//GV_SD_485_time_ITEM.sd_cal_hour,GV_SD_485_time_ITEM.sd_cal_minute,GV_SD_485_time_ITEM.sd_cal_second);

	//formatTitleDateTime2(GV_SD_485_time_ITEM.sd_cal_hour,GV_SD_485_time_ITEM.sd_cal_minute,GV_SD_485_time_ITEM.sd_cal_second);
	char state[]="停止运行";
//SYSTEM_DEBUG(" ------------CD %d\n",sizeof(state));


for(uint16_t ii=0;ii<BOARD_COUNT;ii++){
	switch(GV_CommSendToSlaveBoardState.S_Run[ii]){
		case 0:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "停止  ");break;  //停止
		case 1:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "并充");break;	 //恒流充电
		case 2:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "并放");break;  //放电
		case 3:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "维护");break;  //	维护	
//		case 4:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "恒压并放");break;		//	恒压充电			
//		case 5:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "恒流均衡");break;	 //恒流充电
//		case 6:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "恒压均衡");break;  //放电
//		case 0x07:strcpy(GV_SD_485_ITEM.sd_work_state, "恒流并放");break;  //	维护	
//		case 0x08:strcpy(GV_SD_485_ITEM.sd_work_state, "恒压并放");break;		//	恒压充电				
		
	
	}
}
	//void sd_usart_cmd_SetData(char* runtime,char* workmode,uint8_t batgroup,uint16_t* workdata,uint8_t showmode)
	//GV_ComuBoardState
_485_B_RX_EN();delay_ms(80);
sd_usart_cmd_ReadData();delay_ms(80);	sd_usart_cmd_ReadData();delay_ms(80);	sd_usart_cmd_ReadData();delay_ms(80);	sd_usart_cmd_ReadData();delay_ms(80);	

	sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);

	delay_ms(100);			
	sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);

	delay_ms(100);		
	sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);

	delay_ms(200);		
//sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);
////	_485_B_RX_EN();
//	delay_ms(70);

SD_SAVEALLDATA_Fun();
	
	//sd_usart_cmd_SetData();
	//formatSD_Date();
	
	
	
	
	
	

}
#endif

void SD_STOP_FunOuter(void){

	
		if(GV_SD_data.SD_launch_flag==1){
	GV_SD_data.SD_launch_flag=0;
		SD_STOP_Fun();
	}
	


}






void SD_STOP_Fun(void){
#if 1
for(uint16_t ii=0;ii<BOARD_COUNT;ii++){

//		strcpy(GV_SD_485_ITEM.sd_work_state[ii], "运行完成");//恒流充电
	switch(GV_ScreenSettingDataState.Set_Bat_Runworkingmode[ii]){
		case 0:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "未启动  ");break;  //停止
		default:strcpy(GV_SD_485_ITEM.sd_work_state[ii], "运行停止");break;	 //恒流充电

			
		
	
	}

//strcpy(GV_SD_485_time_ITEM.sd_work_state, "停止运行");  //停止
//sd_usart_cmd_SetData(char* runtime,char* workmode,uint8_t batgroup,uint16_t* workdata,uint8_t showmode)

}
	
	
SD_SAVEALLDATA_Fun();
//SuspendTaskHandler(&Handle_SaveSdData);
#endif



}

void SD_SAVEALLDATA_Fun(void){


//	_485_B_RX_EN();
//	delay_ms(70);	
//	sd_usart_cmd_ReadTime();

//	_485_B_TX_EN();
//	Usart_SendString( USART_485_B,formatSD_Date( runtime,workmode,batgroup, workdata, showmode));
//	_485_B_RX_EN();
	#if 1

	SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"SD_SAVEALLDATA_Fun++++%d  \r\n",GV_HardwareState.hard_board_count);
	
	for(uint16_t i=0;i<GV_HardwareState.hard_board_count;i++){
		
//		Simple_Random_setArray(GV_ComuBoardState.screenBatVolComuState[i].SC_Vol,12,3000,3499);
		
if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0 ){

		sd_usart_cmd_SetData(
	formatDateTimeToString(Screen_SYS_SHOW_SettingDataState.sys_NOW_hour,Screen_SYS_SHOW_SettingDataState.sys_NOW_minute,Screen_SYS_SHOW_SettingDataState.sys_NOW_second),GV_SD_485_ITEM.sd_work_state[i],
	i+1,GV_ComuBoardState.screenBatVolComuState[i].SC_Vol,EM_SHOW_VOL);
		
//			SYSTEM_DEBUG("+++-----**868686");
}
		
delay_ms(100);
		
		
	}
		
//    for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {

//        if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0 && i > (GV_HardwareState.hard_board_nowActive-1)) {//GV_HardwareState.hard_board_nowActive这里必定从1开始，不可能是0，0代表空闲，所以i必定从1开始
//            break;//
//        }

//    }
	

//		for(uint16_t i=0;i<GV_HardwareState.hard_board_count;i++){
//			Simple_Random_setArray((uint16_t*)GV_ComuBoardState.screenBatVolComuState[i].SC_Cur,12,3000,3499);
			
//		sd_usart_cmd_SetData(GV_SD_485_time_ITEM.sd_work_state,formatDateTimeToString(GV_SD_485_time_ITEM.sd_cal_hour,GV_SD_485_time_ITEM.sd_cal_minute,GV_SD_485_time_ITEM.sd_cal_second),
//	i+1,(uint16_t*)GV_ComuBoardState.screenBatVolComuState[i].SC_Cur,EM_SHOW_CURRENT);
	#if 0		
				sd_usart_cmd_SetData(
	formatDateTimeToString(Screen_SYS_SHOW_SettingDataState.sys_NOW_hour,Screen_SYS_SHOW_SettingDataState.sys_NOW_minute,Screen_SYS_SHOW_SettingDataState.sys_NOW_second),GV_SD_485_ITEM.sd_work_state[i],
	i+1,(uint16_t*)GV_ComuBoardState.screenBatVolComuState[i].SC_Cur,EM_SHOW_CURRENT);	
			
	delay_ms(70);
	}
		
	#endif
			for(uint16_t i=0;i<GV_HardwareState.hard_board_count;i++){
				
//				Simple_Random_setArray(GV_ComuBoardState.screenBatCapComuState[i].SC_Capacity,12,3000,3499);
				if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0 ){

				
	sd_usart_cmd_SetData(
	formatDateTimeToString(Screen_SYS_SHOW_SettingDataState.sys_NOW_hour,Screen_SYS_SHOW_SettingDataState.sys_NOW_minute,Screen_SYS_SHOW_SettingDataState.sys_NOW_second),GV_SD_485_ITEM.sd_work_state[i],
	i+1,GV_ComuBoardState.screenBatVolComuState[i].SC_Cap,EM_SHOW_CAPICITY);		
				
				}
	delay_ms(100);
	};
				delay_ms(100);		
	sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);
	delay_ms(100);	
	SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"SD_SAVEALLDATA_Fun++++%d  \r\n",GV_HardwareState.hard_board_count);
#endif

}













