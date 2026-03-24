#include "./communication/Comu485DataFun.h"

#include "./communication/Comu485.h"

#include "./converter/data_convert.h"
#include "./GV_variable.h" 
#include "./controller/TaskFun.h"
#include "./defines.h"
#include "./sys/sysio.h"
#include "./communication/Comu485Dis.h"
#include "./service/DisposeDataFun.h"
#include "./Converter/data_find.h"
#include "./defines.h"




//extern str_CommSlaveUpdloadState GV_CommSlaveUpdloadState[BOARD_COUNT];
extern str_ComuBoardState GV_ComuBoardState;

extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

extern str_GetSlaveUpdloadState BigD_GV_getSlaveUpdloadState[BOARD_COUNT];

extern str_CommSlaveAskState GV_CommSlaveAskState;

extern str_HardwareState GV_HardwareState; 


void s485GetData_Dispose(uint8_t** p_data,uint8_t num){

if((**p_data)-1>=12){
	SYSTEM_DEBUG(" ==**p_data-1: %d",**p_data-1);
return;
}
memmove(&BigD_GV_getSlaveUpdloadState[(**p_data)-1].BS_Addr,*p_data,num);


SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)&BigD_GV_getSlaveUpdloadState[(**p_data)-1].BS_Addr,sizeof(GV_ComuBoardState.getSlaveUpdloadState[0]),"GV_ComuBoardState -0 ---%d \n",num);




StructCopyAndVpReserve((uint8_t *)&GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_Addr,(uint8_t *)&BigD_GV_getSlaveUpdloadState[(**p_data)-1].BS_Addr,sizeof(GV_ComuBoardState.getSlaveUpdloadState[0]),
GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_Vol,102);

//SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)&GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_Addr,sizeof(GV_ComuBoardState.getSlaveUpdloadState[0]),"GV_ComuBoardState ---\n");


if((GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_ErrorBattNo!=0)||(GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_ErrorType!=0)){

SYSTEM_ERROR("\n  error BS_ErrorBattNo= %d, BS_ErrorType=%d \n",GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_ErrorBattNo,BigD_GV_getSlaveUpdloadState[(**p_data)-1].BS_ErrorType);
	
}


GV_ComuBoardState.commu_overtime[(**p_data)-1]=0;
//SYSTEM_DEBUG_ARRAY_MESSAGE(&GV_ComuBoardState.getSlaveUpdloadState[(**p_data)-1].BS_Addr,num,"   cc_data ");


int i;
//	SYSTEM_DEBUG("\n ----ppp  ,%d ,%d \n",GV_HardwareState.hard_board_nowActive,GV_HardwareState.hard_board_whichIsActiveBit);
    for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {

        if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0 && i > (GV_HardwareState.hard_board_nowActive-1)) {//找到下一个被激活的板，GV_HardwareState.hard_board_nowActive这里必定从1开始，不可能是0，0代表空闲，所以i必定从1开始
            break;//找到下一个被激活的板
        }

    }


//	SYSTEM_DEBUG_BINARY(GV_ComuBoardState.commu_needfinishwork_FLAG,"PPPPk ");


//GV_CommSlaveAskState.A_Addr=(**p_data)+1;
GV_CommSlaveAskState.A_Addr=(**p_data);

GV_CommSlaveAskState.A_DataType=0x00;
//	SYSTEM_DEBUG("\n ----xxx  ,%d  \n",GV_CommSlaveAskState.A_Addr);
if(GV_CommSlaveAskState.A_Addr==GV_HardwareState.hard_board_nowActive){
	
GV_HardwareState.hard_board_nowActive = i+1;//找到下一个被激活的板，给i,重要
	GV_CommSlaveAskState.A_Addr=GV_HardwareState.hard_board_nowActive;//发送下一个地址
	
//	SYSTEM_DEBUG("\n hard_board_nowActive  ,%d \n",GV_HardwareState.hard_board_nowActive);
//	SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"*****%d ",GV_HardwareState.hard_board_whichIsActiveBit);



}else{
	
		GV_HardwareState.hard_board_nowActive = i+1;
GV_CommSlaveAskState.A_Addr=GV_HardwareState.hard_board_nowActive;
SYSTEM_DEBUG("\n 4s  ask get err    %d   %d \n",GV_CommSlaveAskState.A_Addr,GV_HardwareState.hard_board_nowActive);


}

//findHighestSetBit
//	SYSTEM_DEBUG("\n hard_board_HightActiveBitNum  ,%d,%d \n", GV_CommSlaveAskState.A_Addr,GV_HardwareState.hard_board_HightActiveBitNum);
//Delay(5);
if((GV_CommSlaveAskState.A_Addr<=GV_HardwareState.hard_board_count)&&(GV_CommSlaveAskState.A_Addr<=GV_HardwareState.hard_board_HightActiveBitNum)){
	
//	SYSTEM_DEBUG("\n co next %d ",GV_CommSlaveAskState.A_Addr );
c485_write_cmd(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1);
	ResumeTaskHandler(&AskUploadDataovertimefun);
}else{
//	SYSTEM_DEBUG("\n co over \n");
GV_HardwareState.hard_board_nowActive=0;
GV_CommSlaveAskState.A_Addr=0xfd;
c485_write_cmd(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1);
	SuspendTaskHandler(&AskUploadDataovertimefun);
}



};









void s485_SendSlaveAskdataAfter50MS(void){
//GV_CommSlaveAskState.A_Addr=1;//
//	SYSTEM_DEBUG(" nnn   %d",GV_HardwareState.hard_board_count);
//	GV_HardwareState.hard_board_whichIsActiveBit |= (1 << i);
	int i;
 for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {
        if (((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0)&&(i<GV_HardwareState.hard_board_HightActiveBitNum)) {//找到第一块被激活的板号
	
					GV_HardwareState.hard_board_nowActive=i+1;

            break;//判断首次出现1的情况
        }
				 if(i==GV_HardwareState.hard_board_HightActiveBitNum){
					 
//				SuspendTaskHandler(&Handle_SendSlaveAskdataAfter50MS);
//					 i=0;
					 
//					 GV_HardwareState.hard_board_nowActive=i;
					 
// SYSTEM_DEBUG("      no board active--- error  =%d",GV_HardwareState.hard_board_HightActiveBitNum);
//					 SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"hard_board_whichIsActiveBit");
//					 SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_HightActiveBitNum,"hard_board_HightActiveBitNum");
					 return;
 }
    }
					
GV_CommSlaveAskState.A_Addr=GV_HardwareState.hard_board_nowActive;
//	SYSTEM_DEBUG(" nnn   3");
	
//GV_CommSlaveAskState.A_Addr=4;//*******123456789
GV_CommSlaveAskState.A_DataType=0x00;

//SYSTEM_DEBUG(" ==s485_SendSlaveAskdataAfter50MS   2");
c485_write_cmd_print(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1,EM_NotPrint);

}















