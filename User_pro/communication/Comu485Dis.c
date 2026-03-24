



#include "./communication/Comu485Dis.h"
#include "./sys/bsp_systime.h"   
#include "./usart/bsp_485_usart.h"  
#include "./GV_variable.h"
#include "./screen/Screen.h"
#include "./defines.h"
#include "./communication/Comu485.h"
#include "./screen/ScreenDataDis.h"
#include "./communication/Comu485DataFun.h"
#include "./converter/data_convert.h"
#include "./controller/TaskSingleWork.h"
#include "./controller/TaskFun.h"
#include "./defines.h"
#include "./usart/usart485verify.h"

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 
extern str_ScreenSettingDataState GV_ScreenSettingDataState;


extern str_BatterySetCalibration GV_BatterySetCalibration;

extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

extern str_CommSlaveAskState GV_CommSlaveAskState;

extern str_CommSendToSlaveBoardState BigD_GV_CommSlaveBoardState;
//extern str_CommSendToSlaveBoardState__packed BigD_GV_CommSlaveBoardState__packed;

extern str_BatterySetCalibration BigD_GV_BatterySetCalibration;

extern QUEUE_DATA_BUFF GV_qdf_485_1;










void SendCommu485data(void){



	comu_Sendslave_Rundata();

}




void comu_Sendslave_Rundata(void){


	//GV_CommSendToSlaveBoardState  StopParalleltChargeVol
	//	memcpy(&GV_ScreenSetRunBoardState.Set_AllUpperLimitParallelVol,&TMP_EEpromSettingState.EEP_AllUpperLimitParallelVol,EEPROM_SettingSaveDataCount);
	
//		    for (int i = 0; i < 8; i++)
//    {
//        GV_CommSendToSlaveBoardState.DischargeCurrent[i] = 0 - BigD_GV_CommSlaveBoardState.DischargeCurrent[i];
//    }
	//	SYSTEM_INFO("\n %d=0=%d  \n",GV_CommSendToSlaveBoardState.ChargeCurrent[0],GV_CommSendToSlaveBoardState.DischargeCurrent[0]);
	//SYSTEM_INFO("\n %d=1=%d  \n",GV_CommSendToSlaveBoardState.ChargeCurrent[1],GV_CommSendToSlaveBoardState.DischargeCurrent[1]);
	
	//SYSTEM_INFO("\n %d=2=%d  \n",sizeof(BigD_GV_CommSlaveBoardState),sizeof(GV_CommSendToSlaveBoardState));
	
	
	
	

	
	StructCopyAndVpReserve((uint8_t *)&BigD_GV_CommSlaveBoardState,(uint8_t *)&GV_CommSendToSlaveBoardState,sizeof(GV_CommSendToSlaveBoardState),BigD_GV_CommSlaveBoardState.StopParalleltChargeVol,12*10);//16*8
	
	StructOnlyVpReserve((uint8_t *)&BigD_GV_CommSlaveBoardState.SingleBattRun,16);

	

	
	
	
//	memmove(&BigD_GV_CommSlaveBoardState__packed,&BigD_GV_CommSlaveBoardState,19);
//	memmove(&BigD_GV_CommSlaveBoardState__packed.StopParalleltChargeVol,&BigD_GV_CommSlaveBoardState.StopParalleltChargeVol,128);
//	
	
//	_485_A_TX_EN();
//	delay_ms(100);
	//c485_write_cmd(&GV_CommSendToSlaveBoardState.Addr,sizeof(GV_CommSendToSlaveBoardState), CTRL_CODE_SET);
	
	for(uint16_t i=0;i<4;i++){
	c485_write_cmd_print(&BigD_GV_CommSlaveBoardState.CommS_Addr,sizeof(BigD_GV_CommSlaveBoardState), CTRL_CODE_SET,0,0);
		if(i!=4){
			delay_ms(100);
		}
	
	}
	_485_A_RX_EN();
		
	void* frame=&BigD_GV_CommSlaveBoardState.CommS_Addr;
//	SYSTEM_INFO("\n **********-----485-----**********\n");

	

		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,sizeof(BigD_GV_CommSlaveBoardState)," 485 send=");
		
	frame=NULL;


};

/**
  * @brief  从485访问来询问从机数据（每4秒询问一次）
  * @param  
  * @retval 无
  */
void comu_SendslaveAskdata(void){
GV_CommSlaveAskState.A_Addr=0xFE;
//	SYSTEM_DEBUG(" ==s485_SendSlaveAskdataAfter50MS   1");
	c485_write_cmd_print(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1,EM_NotPrint);
	ResumeTaskHandler(&Handle_SendSlaveAskdataAfter50MS);
	
SYSTEM_INFO("\n 485-FE-ask T=%d\n",GetSysRunTime());

}
#include "./usart/bsp_485_usart.h"   

void comu_SendslaveRectifydata(void){
_485_A_TX_EN();Delay(200);
	for(uint16_t i=0;i<5;i++){
	c485_write_cmd(&BigD_GV_BatterySetCalibration.cal_Addr, sizeof(BigD_GV_BatterySetCalibration), CTRL_CODE_rectify,1);
		delay_ms(200);
	}
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8,"----////=====");
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&BigD_GV_BatterySetCalibration.cal_Addr,sizeof(BigD_GV_BatterySetCalibration),"BigD_GV_BatterySetCalibration=***");
SYSTEM_INFO("\n 485----rectify\n");

}











#if 0
/**
  * @brief  从串口数据中取出数据
  * @param  frame：整个数据指针 length：整个数据指针长度 Data处理后截取的数据 addr数据中第一个地址 Datanum数据中有多少字节,Command_num命令字
  * @retval 无
  */
void s485_copy_data_from_queue(QUEUE_DATA_BUFF *qdf,uint8_t **frame,uint16_t* length,uint8_t** Data,uint8_t* Datanum,uint8_t* Command_num){
	
	Q_QUEUE_DATA_TYPE *rx_data;	
	/*从缓冲区读取数据，进行处理，*/
	rx_data = p_cbRead(&(qdf)->q_rx_queue); 

	if(rx_data != NULL)//缓冲队列非空
	{		
		*length=rx_data->q_len;

		*frame = (uint8_t *)malloc(rx_data->q_len);
		
    memcpy(*frame,rx_data->q_head,rx_data->q_len);
		//*(*frame+rx_data->q_len) = '\0';
//c485_ReadfromQ_dataFromatVerify
//		SYSTEM_DEBUG("rx_data->q_len=:%d*****",rx_data->q_len);
		
		
			if(c485_ReadfromQ_dataFromatVerify(frame,*length)==1){
		
			*Datanum=0;
#if 1
			SYSTEM_DEBUG_ARRAY_MESSAGE(*frame,*length,"get wrong data is ");
#endif
			//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)frame,*length,"wrong data is ");//wrong
			p_cbReadFinish(&(qdf)->q_rx_queue);
		return;
		}
			
					*Command_num=*(*frame+3);
		

		//SYSTEM_DEBUG("11 frame[4]: %02x frame[5]: %02X  ",(uint8_t)*(*frame+4),(uint8_t)*(*frame+5));//正确
		//SYSTEM_DEBUG("22 frame[4]: %02x frame[5]: %02x  ",(uint8_t)frame[0][4],(uint8_t)frame[0][5]);//正确
		//*addr=((uint16_t)*(*frame+4)<<8)|frame[0][5];
		//SYSTEM_DEBUG(" *addr: %04x   ",*addr);
		
	#if (c485_CRC16==1)	
	*Datanum =*length-6;
	#else	
	*Datanum =*length-4;
	#endif	

		//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);
		*Data=(uint8_t *)malloc(*Datanum );
				if(Data == NULL) {
				SYSTEM_ERROR("memory malloc wrong\n");
					p_cbReadFinish(&(qdf)->q_rx_queue);
		return;
	}
		// SYSTEM_DEBUG("frame ....address：%p ",*frame+7);
		memcpy(*Data,*frame+4,*Datanum );
	
		//SYSTEM_DEBUG_ARRAY((uint8_t*)*Data,*length-7);
	
//		SYSTEM_DEBUG(" ==xxxxx====length: %d",*length);
//		SYSTEM_DEBUG("received data：%s  ,length: %d",*frame,rx_data->q_len);
//		SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);

		//使用完数据必须调用cbReadFinish更新读指针
		p_cbReadFinish(&(qdf)->q_rx_queue);
	}
}

#endif


void pull_data_from_485(void)
{
	
uint8_t *frame = NULL;
uint8_t* f_data=NULL;	
uint16_t s_leng=0;//必须为0

uint8_t s_num = 0;
	uint8_t s_command_num=0;
//copy_data_from_queue(&frame,&leng);
	//SCREEN_DataSolveDV_cmd(&s_addr,&f_data,leng,frame);
	
//	if(p_cbRead(&GV_qdf_485_1.q_rx_queue)==NULL){
//	return;
//	}
	
//			s485_copy_data_from_queue(&GV_qdf_485_1,&frame,&s_leng,&f_data,&s_num,&s_command_num);
//	SYSTEM_ERROR("num pull_data_from_485 --------,yes3 \n");
	 s485_convert_queue(&frame,&f_data,&s_num,&s_command_num);
	
	if(s_num!=0){
//		 SYSTEM_DEBUG("485received data：%s  ,length: %d",frame,s_leng);
		//SYSTEM_DEBUG("received data：s_addr %x  ,length: %d",s_addr,s_num);
#if 0			 
   SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,s_leng,"485 get data : ,length: %d",s_num);
#endif		
		
		s485dataswitch(&f_data,s_num,s_command_num);



	}	
//		free(frame);
//	free(f_data);
	f_data= NULL;
	frame = NULL;
	s_leng=0;

}






void s485dataswitch(uint8_t** p_data,uint8_t num,uint8_t command_num){
//SYSTEM_DEBUG_ARRAY_MESSAGE(*p_data,num,"   485_short_data ");
	SYSTEM_INFO("s485dataswitch \n");
switch(command_num){

	case 0x10: s485GetData_Dispose(p_data,num); break;
	
	default:  break;

}

}











