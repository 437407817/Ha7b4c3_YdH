



#include "./communication/SD_485Dis.h"
#include "./sys/bsp_systime.h"   

#include "./GV_variable.h"
#include "./screen/Screen.h"
#include "./defines.h"
#include "./communication/Comu485.h"
#include "./screen/ScreenDataDis.h"
#include "./communication/Comu485DataFun.h"
#include "./converter/data_convert.h"



extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 
extern str_ScreenSettingDataState GV_ScreenSettingDataState;


extern str_BatterySetCalibration GV_BatterySetCalibration;

extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

extern str_CommSlaveAskState GV_CommSlaveAskState;

//extern str_CommSendToSlaveBoardState BigD_GV_CommSlaveBoardState;

extern str_BatterySetCalibration BigD_GV_BatterySetCalibration;

extern QUEUE_DATA_BUFF GV_qdf_485_B;


//extern SD_485_time_ITEM_t GV_SD_485_time_ITEM;

extern str_Screen_SYS_SettingDataState Screen_SYS_SettingDataState;


extern str_Screen_SYS_SHOW_SettingDataState Screen_SYS_SHOW_SettingDataState;










//每个项目不同，需要修改
uint8_t SD_c485_ReadfromQ_dataFromatVerify(uint8_t** q_data, uint8_t q_len){
uint16_t crc16_res;
uint8_t	cl,ch;
			//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->len);
		if(!(((q_data[0][0]=='O')&&(q_data[0][1]=='K')&&(q_data[0][2]=='!'))||
			((q_data[0][0]=='2')&&(q_data[0][1]=='0')&&(q_data[0][4]=='-')&&(q_data[0][7]=='-')))){
SYSTEM_ERROR("SD OK! OR 20** data is not right \n");
SYSTEM_ERROR("%c %c %c %c \n",q_data[0][0],q_data[0][1],q_data[0][4],q_data[0][7]);				
	return 1;
		}
			
		//SYSTEM_DEBUG("11 q_data[4]: %02x q_data[5]: %02X  ",(uint8_t)*(*q_data+4),(uint8_t)*(*q_data+5));//正确
		//SYSTEM_DEBUG("22 q_data[4]: %02x q_data[5]: %02x  ",(uint8_t)q_data[0][4],(uint8_t)q_data[0][5]);//正确

	
	
		return 0;


}






/**
  * @brief  从串口数据中取出数据
  * @param  frame：整个数据指针 length：整个数据指针长度 Data处理后截取的数据 addr数据中第一个地址 Datanum数据中有多少字节,Command_num命令字
  * @retval 无
  */
void SD_s485_copy_data_from_queue(QUEUE_DATA_BUFF *qdf,uint8_t **frame,uint16_t* length,uint8_t* Command_num){
	
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
		//SYSTEM_DEBUG_ARRAY_MESSAGE(*frame,*length,"------get  data is ");
		
		
		
			if(SD_c485_ReadfromQ_dataFromatVerify(frame,*length)==1){
		

			SYSTEM_DEBUG_ARRAY_MESSAGE(*frame,*length,"get wrong data is ");
			//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)frame,*length,"wrong data is ");//wrong
			p_cbReadFinish(&(qdf)->q_rx_queue);
		return;
		}
			
*Command_num=*(*frame+0);
		

		//SYSTEM_DEBUG("11 frame[4]: %02x frame[5]: %02X  ",(uint8_t)*(*frame+4),(uint8_t)*(*frame+5));//正确
		//SYSTEM_DEBUG("22 frame[4]: %02x frame[5]: %02x  ",(uint8_t)frame[0][4],(uint8_t)frame[0][5]);//正确
		//*addr=((uint16_t)*(*frame+4)<<8)|frame[0][5];
		//SYSTEM_DEBUG(" *addr: %04x   ",*addr);
		



	
		//SYSTEM_DEBUG_ARRAY((uint8_t*)*Data,*length-7);
	
//		SYSTEM_DEBUG(" ==xxxxx====length: %d",*length);
//		SYSTEM_DEBUG("received data：%s  ,length: %d",*frame,rx_data->q_len);
//		SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);

		//使用完数据必须调用cbReadFinish更新读指针
		p_cbReadFinish(&(qdf)->q_rx_queue);
	}
}




void pull_data_from_SD_485(void)
{
	
uint8_t *frame = NULL;

uint16_t s_leng=0;//必须为0


	uint8_t s_command_num=0;
//copy_data_from_queue(&frame,&leng);
	//SCREEN_DataSolveDV_cmd(&s_addr,&f_data,leng,frame);
	
	//SYSTEM_DEBUG("sd   485++++++");
			SD_s485_copy_data_from_queue(&GV_qdf_485_B,&frame,&s_leng,&s_command_num);

	if(s_leng!=0){
		 //SYSTEM_DEBUG("485received data：%s  ,length: %d",frame,s_leng);
	
			 
   //SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,s_leng,"485 all data : ,length: %d",s_leng);
		
		
		SD_s485dataswitch(&frame,s_leng,s_command_num);



	}	
		free(frame);

	frame = NULL;
	s_leng=0;

}






void SD_s485dataswitch(uint8_t** p_data,uint8_t num,uint8_t command_num){
//SYSTEM_DEBUG_ARRAY_MESSAGE(*p_data,num,"   sd_485_short_data ");
	
switch(command_num){

	case '2': SD_getTime_Fun(*p_data,num); break;
	
	case 'O': SD_getOK_Fun(p_data,num); break;
	default:  break;

}

}


void SD_getTime_Fun(uint8_t* p_data,uint8_t num){

#if 0
//		SYSTEM_DEBUG("real==SD TIME data: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",(*(p_data+0)),(*(p_data+1)),(*(p_data+2)),(*(p_data+3)),(*(p_data+4)),(*(p_data+5)),(*(p_data+6)),(*(p_data+7)),(*(p_data+8)),
//	(*(p_data+9)),(*(p_data+10)),(*(p_data+11)),(*(p_data+12)),(*(p_data+13)),(*(p_data+14)),(*(p_data+15)),(*(p_data+16)),(*(p_data+17)),(*(p_data+18)));

	//uint8_t asciiToDigit(char c)
	GV_SD_485_time_ITEM.sd_year=((uint16_t)(asciiToDigit(*(p_data+0)))*1000)+((uint16_t)(asciiToDigit(*(p_data+1)))*100)+((uint16_t)(asciiToDigit(*(p_data+2)))*10)+((uint16_t)(asciiToDigit(*(p_data+3))));
GV_SD_485_time_ITEM.sd_month=((uint16_t)(asciiToDigit(*(p_data+5)))*10)+((uint8_t)(asciiToDigit(*(p_data+6))));
GV_SD_485_time_ITEM.sd_day=((uint16_t)(asciiToDigit(*(p_data+8)))*10)+((uint8_t)(asciiToDigit(*(p_data+9))));

GV_SD_485_time_ITEM.sd_hour=((uint16_t)(asciiToDigit(*(p_data+11)))*10)+((uint8_t)(asciiToDigit(*(p_data+12))));
GV_SD_485_time_ITEM.sd_minute=((uint16_t)(asciiToDigit(*(p_data+14)))*10)+((uint8_t)(asciiToDigit(*(p_data+15))));
GV_SD_485_time_ITEM.sd_second=((uint16_t)(asciiToDigit(*(p_data+17)))*10)+((uint8_t)(asciiToDigit(*(p_data+18))));
	
	//SYSTEM_DEBUG("SD TIME data: %d-%d-%d %d:%d:%d",GV_SD_485_time_ITEM.sd_year,GV_SD_485_time_ITEM.sd_month,GV_SD_485_time_ITEM.sd_day,GV_SD_485_time_ITEM.sd_hour,GV_SD_485_time_ITEM.sd_minute,GV_SD_485_time_ITEM.sd_second);
	if(Screen_SYS_SettingDataState.sys_sd_tmp_flag==1){//第一次进入页面，需要刷新时间设置和时间间隔设置
	Screen_SYS_SettingDataState.sys_sd_tmp_flag=0;
	
		memcpy(&Screen_SYS_SHOW_SettingDataState.sys_NOW_year,&GV_SD_485_time_ITEM.sd_year,12);
		memcpy(&Screen_SYS_SettingDataState.sys_sd_year,&GV_SD_485_time_ITEM.sd_year,12);
		
		SCREEN_write_cmd(EM_SYS_SHOW_YEAR,&Screen_SYS_SHOW_SettingDataState.sys_NOW_year,12/2);
		
		SCREEN_write_cmd(EM_SYS_SET_YEAR,&Screen_SYS_SettingDataState.sys_sd_year,18/2);
		
	void* frame=&Screen_SYS_SettingDataState.sys_sd_year;
	
	
		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,19," setting time =");
	
   frame=NULL;
return;
	}else{
	
	memcpy(&Screen_SYS_SHOW_SettingDataState.sys_NOW_year,&GV_SD_485_time_ITEM.sd_year,12);
		SCREEN_write_cmd(EM_SYS_SHOW_YEAR,&Screen_SYS_SHOW_SettingDataState.sys_NOW_year,12/2);
		
		
		
	
	}
	
	
	
	#endif
	
	
}

void SD_getOK_Fun(uint8_t** p_data,uint8_t num){



SYSTEM_DEBUG("SD OK/n");




}








