
#include "./service/DisposeDataFun.h"
#include "./GV_variable.h" 


#include "./sys/bsp_systime.h" 
#include "./service/EepromFun.h"

#include "./service/DisposeDataFun.h"

#include "./controller/TaskFun.h"
#include "./controller/TaskSingleWork.h"
#include "./communication/SD_485Dis.h"
#include "./usart/bsp_usart_sd.h"
#include "./communication/Comu485.h"
#include "./screen/ScreenSendFun.h"
#include "./communication/Comu485Dis.h"
#include "./service/PowerDownFun.h"
#include "./screen/ScreenDataDis.h"
#include "./crypt/bsp_encode.h"
#include "./controller/DisStateFun.h"

str_ScreenSetRunState GV_ScreenSetRunBoardState;
//str_ScreenSetRunState OldGV_ScreenSetRunBoardState;
str_ScreenSettingDataState GV_ScreenSettingDataState;
str_ScreenSettingDataState OldGV_ScreenSettingDataState;


str_EEpromSettingState TMP_EEpromSettingState;

str_EEprom_SYS_SettingDataState TMP_EEprom_SYS_SettingState;

str_Screen_SYS_SHOW_SettingDataState Screen_SYS_SHOW_SettingDataState;

str_Screen_SYS_SettingDataState Screen_SYS_SettingDataState;
str_SD_data GV_SD_data;

ScreenCommu_Item_t  GV_ScreenCommu;
str_HardwareState GV_HardwareState; 
str_BatterySetCalibration GV_BatterySetCalibration;
str_BatterySetCalibration BigD_GV_BatterySetCalibration;

str_ComuBoardState GV_ComuBoardState;
str_GetSlaveUpdloadState BigD_GV_getSlaveUpdloadState[BOARD_COUNT];
str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;
str_CommSendToSlaveBoardState BigD_GV_CommSlaveBoardState;

//str_CommSendToSlaveBoardState__packed BigD_GV_CommSlaveBoardState__packed;



str_CommSlaveAskState GV_CommSlaveAskState;

str_ScreenAllCalibration GV_ScreenAllCalibration;

//SD_485_time_ITEM_t GV_SD_485_time_ITEM;


str_PowerDownState GV_PowerDownState;

str_SD_485_ITEM_t GV_SD_485_ITEM;


str_ChipEncode GV_EEpromChipEncode;
str_ChipEncode GV_SaveEEpChipEncode;
//str_ChipEncode GV_NeedSaveEEpChipEncode;


str_SrceenChipEncode_data	GV_ScreenChipEncode;





extern uint16_t Array_WorkType_Stop[4];//待机中
extern uint16_t Array_WorkType_Stop_English[ETE_Size];//待机中


extern uint16_t Array_WorkType_ParrelChargeFinish[4];//并充完成
extern uint16_t Array_WorkType_ParrelDisChargeFinish[4];//并放完成
extern uint16_t Array_WorkType_MaintainFinish[4];//维护完成
extern uint16_t Array_WorkType_NowMessage[4];
extern uint16_t Array_WorkType_NowMessage_English[ETE_Size];
extern uint16_t Array_ErrorType_Nomal[4];
extern uint16_t Array_ErrorType_Nomal_English[4];





void initGV_variable(void){
	 StingMessageConvert();
GV_ScreenSetRunBoardState.Set_NowRunState=0;
GV_HardwareState.ReadyRunState=1;
GV_PowerDownState.powerdown_first_enter=0;//第一次进入电压页面
	GV_HardwareState.hard_board_count=BOARD_COUNT_CHANGE;
	
GV_CommSendToSlaveBoardState.CommS_Addr = 0xFE;	
GV_CommSlaveAskState.A_Addr =	0xFE;
GV_CommSlaveAskState.A_DataType=	0x00;
	GV_ScreenSetRunBoardState.Set_NowShowLanguage=0;
//	GV_ComuBoardState.check_overtime_askdata=OVERTIME_EACH_DATA_ASK;
	
	
//	GV_ComuBoardState.check_isOverTimeStop=0;
	memset(GV_ComuBoardState.check_isOverTimeStop,0,BOARD_COUNT*2);
memcpy(&Array_WorkType_NowMessage,&Array_WorkType_Stop,4*2);
memcpy(&Array_WorkType_NowMessage_English,&Array_WorkType_Stop_English,ETE_Size*2);
	
			for(uint8_t ica=0;ica<BOARD_COUNT;ica++){
				
memcpy(&GV_ComuBoardState.screenBatVolComuState[ica].SC_ErrorType,&Array_ErrorType_Nomal,4*2);
memcpy(&GV_ComuBoardState.screenBatVolComuState[ica].SC_ErrorType_English,&Array_ErrorType_Nomal_English,ETE_Size*2);				
				
			GV_ComuBoardState.commu_overtime[ica]=0;
//				GV_ComuBoardState.check_overtime_askdata_timesflag[ica]=0;
GV_ComuBoardState.screenBatVolComuState[ica].SC_ErrorBattNo=0;
			GV_CommSendToSlaveBoardState.S_Run[ica]=EM_HW_STOP;
		}

	//然后再发送跳转页面数据
	GetBoardSettingEEpromData();
	CopyEEpromDatatoSettingData();
		Setboadnumfun();//设置板数量
//		DisposeSaveSettingData();
		GV_CommSendToSlaveBoardState.Effective=0X0F;
	CopySettingDataToSlaveBuff(EM_HW_LOSEPOWER_PARA);
//	Setboadnumfun();
		GV_SD_data.SD_launch_flag=0;
		
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8,"----////=====h99");
///======================
		Delay(100);
		SYSTEM_DEBUG("\n FirstSaveChipData *********** ");  
		
		 init_encodechipid();
		  
Delay(1500);//等待屏幕亮起延迟
GV_ComuBoardState.commu_finishwork_Old_FLAG=0;
clearAllErrorBoardNoFlag();

//				for(uint16_t i=0;i<0;i++){
//	comu_Sendslave_Rundata();
//		delay_ms(100);
//	}
SendCommu485data();

				SCREEN_write_cmd(EM_Icon_Mode_Select_1,GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8);
			
	//SuspendTaskHandler(&Handle_SendCapacityDataToScreen);
		initPowerDownData();
	_485_B_RX_EN();	sd_usart_cmd_ReadData();Delay(70);sd_usart_cmd_ReadData();Delay(70);sd_usart_cmd_ReadData();Delay(70);sd_usart_cmd_ReadData();Delay(70);	
//	sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);
//	pull_data_from_SD_485();
//	pull_data_from_SD_485();
//	pull_data_from_SD_485();
	
}



//GV_HardwareState.HW_NeedWorkingState=0;
uint8_t checkNeedStateNotEquRealState(void){

	
	
	if(GV_HardwareState.HW_NowRealWorkingState!=GV_HardwareState.HW_NeedWorkingState){
	return 1;
	}else{
	return 0;
	}

}

#include "./Converter/data_find.h"

void Setboadnumfun(void){

//	uint16_t tmp_hard_board_count;
	GV_HardwareState.hard_board_whichIsActiveBit=0;
	
	
for(uint16_t i=0;i<BOARD_COUNT;i++)
{
	if(GV_ScreenSettingDataState.Set_Bat_BattNum[i]!=0){
//	tmp_hard_board_count=i+1;
		
	GV_HardwareState.hard_board_whichIsActiveBit |= (1 << i);
	}
	
	if((GV_ScreenSettingDataState.Set_Bat_SingleBattRun[i]&0x7fff)==0){
		GV_CommSendToSlaveBoardState.S_Run[i]=0;	
	};
}


GV_HardwareState.hard_board_HightActiveBitNum=findHighestSetBit(GV_HardwareState.hard_board_whichIsActiveBit);
SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"");

}




//如果进入该函数，代表从机规定时间没有返回数据
void AskUploadDataovertimefun(void){//如果发送1号板询问数据，超时时间达到，但没有接收1号板数据，则触发该函数

//GV_HardwareState.hard_board_nowActive
uint8_t i;
//	uint8_t  top_board_count;
//	
//	top_board_count=GV_HardwareState.hard_1board_count+1
	
    for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {

        if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0 && i > (GV_HardwareState.hard_board_nowActive-1)) {//这里GV_HardwareState.hard_board_nowActive是从2开始，所以i必定从2开始
					
//          GV_ComuBoardState.check_overtime_askdata_timesflag[GV_HardwareState.hard_board_nowActive-1]+=1;
					
					GV_HardwareState.hard_board_nowActive = i+1;//如果2没有，则跳到3
					
					break;
        }

				if(i==GV_HardwareState.hard_board_count-1){//最后1次没有置位激活
				GV_HardwareState.hard_board_nowActive = GV_HardwareState.hard_board_count+1;
				
				}
    }
	
	
		
		GV_CommSlaveAskState.A_DataType=0x00;
		
		
		
	if( GV_HardwareState.hard_board_nowActive > GV_HardwareState.hard_board_count ){
	
	GV_HardwareState.hard_board_nowActive =0;
		GV_CommSlaveAskState.A_Addr=0xfd;
		
		c485_write_cmd(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1);
		
		
	SuspendTaskHandler(&AskUploadDataovertimefun);
		
	}else{
		GV_CommSlaveAskState.A_Addr=GV_HardwareState.hard_board_nowActive;
		
		c485_write_cmd(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1);
	ResumeTaskHandler(&AskUploadDataovertimefun);//刷新计时
	}
	

	
}



void clearScreenDataVol(void){
	uint8_t i;
for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {
if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) == 0){


memset(&GV_ComuBoardState.getSlaveUpdloadState[i].BS_Vol,0,106);
}


}



}





/*********************************************END OF FILE**********************/
