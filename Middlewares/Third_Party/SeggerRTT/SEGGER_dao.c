#include "./SEGGER_dao.h"
#include "./SEGGER_RTT.h"

#include "string.h"


#include "stm32_FH_xxx_hal.h"


//#include "./sys/bsp_systime.h"   
#include "./buffer/p_data_queue.h"
#include "./buffer/p_data_queue_outer.h"



//#include "./TM/tm_stm32f4_gpio.h"//包含此文件，报错，但未知其原因
#if 0
 #include "./can/bsp_can.h" 
//#include "defines.h"

//#include "./screen/ScreenDataDis.h"
//#include "./usart/p_data_queue.h"



 #include "./sys/sysio.h"

//#include "./test/bsp_test_can.h"   
//#include "./usart/p_data_queue.h"

/**
  * @brief  切换采集通道channel for collection
  * @param  *i 当前通道第几个的状态地址  *openstate 1开启轮训转换采集 0 关闭所有通道
  * @retval 无
  */

CanTxMsg TxMessageSegger;			     //发送缓冲区

void SEGGER_PcDao_CanToMcuTransform(uint8_t *array, uint16_t num){

//Usart_SendArray(pUSARTx,array,num);

}

void SEGGER_PcDao_McuToCanTransform(CAN_TypeDef* CANx1 ,uint8_t *array, uint16_t num){

//Usart_SendArray(pUSARTx,array,num);
	
	
	for(uint8_t i=0;i<num/8;i++){
	
	 //CAN_SetMsg(&TxMessageSegger,CAN_Send_Id_1,array+i*8);
		//CAN_Transmit(CANx1, &TxMessageSegger);
		
	}
}


	//QUEUE_DATA_TYPE *data_p; 
#endif


#define usart_GV_qqb_x   q_tx_rx_queue_2
#define usart_QUEUE_NODE_DATA_LEN_x   q_QUEUE_NODE_DATA_LEN_2

void SEGGER_PcDao_UsartToMcuTransform(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num){


static Q_QUEUE_DATA_TYPE *data_p; 

	//	ucCh  = USART_ReceiveData( USARTx_A );
		
						/*获取写缓冲区指针，准备写入新数据*/
		data_p = p_cbWrite(&usart_GV_qqb_x); 
			for(uint8_t i=0;i<num;i++){
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		

			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->q_head + data_p->q_len) = *(array+i);
				
			if( ++data_p->q_len >= usart_QUEUE_NODE_DATA_LEN_x)
			{
				p_cbWriteFinish(&usart_GV_qqb_x);
			}


     }else return;
	 }		
		p_cbWriteFinish(&usart_GV_qqb_x);

}

//void SEGGER_PcDao_McuToUsartTransform(USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num){

//Usart_SendArray(pUSARTx,array,num);

//}

void SEGGER_PcDao_McuToUsartTransform(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num) {
    HAL_StatusTypeDef status;
    status = HAL_UART_Transmit(huart, array, num, 1000); // 超时时间设置为 1000 毫秒
    if (status != HAL_OK) {
        // 处理发送失败的情况，例如可以添加日志输出或者错误处理代码
    }
}


void SEGGER_PcDao_Test256DataPrint(void){
uint8_t Segger_Buf_Write[256];
	for (uint16_t i=0; i<256; i++ ) //填充缓冲
	{   
		Segger_Buf_Write[i] =i+1;
		SYSTEM_INFO("0x%02X ", Segger_Buf_Write[i]);
		if(i%16 == 15)    
		SYSTEM_INFO("\r\n");    
	}
}











/*********************************************END OF FILE**********************/
