/**
  ******************************************************************************
  * @file    p_data_queue.c
  * @author  wmap
  * @version V1.0
  * @date    2024-12-24
  * @brief   环形缓冲区，适用于接收外部数据时用作缓冲
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 IOT STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "./buffer/p_data_queue_outer.h"
#include "./sys/sysio.h"
#include "./usart/bsp_usart.h"
#include "./user_config.h"

//不要删，这是传统版本定义
#if 0
// 预先定义全局一维数组用于不同情况的 Q_QueueBuffer 结构体中的 elems 指针指向
Q_QUEUE_DATA_TYPE global_elems_1[q_QUEUE_NODE_NUM_1];
//Q_QUEUE_DATA_TYPE global_elems_2[q_QUEUE_NODE_NUM_2];

// 预先定义全局二维数组用于不同情况的 NODE_DATA_BUFF 结构体中的 node_buff 指针指向
char global_node_buff_1[q_QUEUE_NODE_NUM_1][q_QUEUE_NODE_DATA_LEN_1];
//char global_node_buff_2[q_QUEUE_NODE_NUM_2][q_QUEUE_NODE_DATA_LEN_2];

Q_QUEUE_DATA_TYPE  node_data_1[q_QUEUE_NODE_NUM_1];
//Q_QUEUE_DATA_TYPE  node_data_2[q_QUEUE_NODE_NUM_2];


Q_QueueBuffer q_tx_rx_queue_1;
NODE_DATA_BUFF GV_ndb_1;
#endif

//Q_QueueBuffer q_rx_queue_2;
//NODE_DATA_BUFF GV_ndb_2;

CREAT_p_queue(2,q_QUEUE_NODE_NUM_2,q_QUEUE_NODE_DATA_LEN_2);

CREAT_p_queue(UsartDMAsend,q_QUEUE_NODE_NUM_UsartDMAsend,q_QUEUE_NODE_DATA_LEN_UsartDMAsend);
//extern Q_QueueBuffer q_rx_queue_1;



//

//



//Q_QUEUE_DATA_TYPE *data_p; 
//extern char global_node_buff_2[q_QUEUE_NODE_NUM_2][q_QUEUE_NODE_DATA_LEN_2];



#if !USE_SHELL 
//extern QUEUE_DATA_BUFF   GV_qdf_1;
//extern QUEUE_DATA_BUFF   GV_qdf_2;


#if 0
#define usart_GV_qqb_x   q_tx_rx_queue_2
#define usart_QUEUE_NODE_DATA_LEN_x   q_QUEUE_NODE_DATA_LEN_2


void USART_A_IRQHandler(void)
{

	
#if defined (USART_A_RX_DMA_ENABLE)
	
	
	
	
		uint16_t t;
	if(USART_GetITStatus(USARTx_A,USART_IT_IDLE) == SET)          //检查中断是否发生
	{	
		
		#if 0
		DMA_Cmd(USART_RX_DMA_CHANNEL,DISABLE);                         //关闭DMA传输

		t = DMA_GetCurrDataCounter(USART_RX_DMA_CHANNEL);              //获取剩余的数据数量
		
        Usart_SendArray(USARTx_A,ReceiveBuff,RECEIVEBUFF_SIZE-t);       //向电脑返回数据（接收数据数量 = SENDBUFF_SIZE - 剩余未传输的数据数量）
		//Usart_SendString( USARTx_A,"XXXXXX\n");     
		DMA_SetCurrDataCounter(USART_RX_DMA_CHANNEL,RECEIVEBUFF_SIZE);    //重新设置传输的数据数量

		DMA_Cmd(USART_RX_DMA_CHANNEL,ENABLE);                          //开启DMA传输
		
		USART_ReceiveData(USARTx_A);                              //读取一次数据，不然会一直进中断
		USART_ClearFlag(USARTx_A,USART_FLAG_IDLE);                //清除串口空闲中断标志位
		
		#endif
		
	
		
		//USART_ClearITPendingBit(USARTx_A, USART_IT_IDLE);
		
		
		
		DMA_Cmd(USART_RX_DMA_CHANNEL,DISABLE); 
		
		Usart_SendString( USARTx_A,"XXXXXX\n");  
		DMA_SetCurrDataCounter(USART_RX_DMA_CHANNEL,usart_QUEUE_NODE_DATA_LEN_x);
		
		DMA_Cmd(USART_RX_DMA_CHANNEL,ENABLE); 
		
		USART_ReceiveData(USARTx_A);
		p_cbWriteFinish(&usart_GV_qqb_x);
		USART_ClearFlag(USARTx_A,USART_FLAG_IDLE);
	}
		#else
	uint8_t ucCh;
	Q_QUEUE_DATA_TYPE *data_p; 
	
	if (__HAL_UART_GET_FLAG(&huart_a, UART_FLAG_RXNE) != RESET)
	{	
//		ucCh  = USART_ReceiveData( USARTx_A );
		if (HAL_UART_Receive(&huart_a, &ucCh, 1, 100) == HAL_OK) {
    // 数据接收成功，可以对 ucCh 进行处理
} else {
    // 处理接收失败的情况，例如超时等
}
		
						/*获取写缓冲区指针，准备写入新数据*/
		data_p = p_cbWrite(&usart_GV_qqb_x); 
//		SYSTEM_DEBUG("\n data_p  %c\n",ucCh);
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		
			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->q_head + data_p->q_len) = ucCh;
				
			if( ++data_p->q_len >= usart_QUEUE_NODE_DATA_LEN_x)
			{
				p_cbWriteFinish(&usart_GV_qqb_x);
			}
		}else return;	
	}
	
	if (__HAL_UART_GET_FLAG(&huart_a, UART_FLAG_IDLE) != RESET)                                        //数据帧接收完毕
	{
			/*写入缓冲区完毕*/
			p_cbWriteFinish(&usart_GV_qqb_x);

//		ucCh = USART_ReceiveData( USARTx_A );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
//		HAL_UART_Receive(&huart_a, &ucCh, 1, 100);
    // 数据接收成功，可以对 ucCh 进行处理

__HAL_UART_CLEAR_IDLEFLAG(&huart_a);

	}	


	#endif /* USARTx_A_RX_DMA_ENABLE */
	
	
}	

#endif
#endif












































void p_tx_rx_queue_init_all(UART_HandleTypeDef* uartHandle){
	#if 0
if(uartHandle->Instance == USART_A)
{
	//SYSTEM_DEBUG(" ==xxxxx====1");
	p_tx_rx_queue_init(&q_rx_queue_1,&GV_ndb_1,q_QUEUE_NODE_NUM_1,q_QUEUE_NODE_DATA_LEN_1,global_elems_1,node_data_1,global_node_buff_1);

	//p_tx_rx_queue_init(QUEUE_DATA_BUFF *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length);
}
#else
if(uartHandle->Instance == USART_A)
{
	//SYSTEM_DEBUG(" ==xxxxx====1");
	p_tx_rx_queue_init(&q_tx_rx_queue_2,&GV_ndb_2,q_QUEUE_NODE_NUM_2,q_QUEUE_NODE_DATA_LEN_2,global_elems_2,node_data_2,global_node_buff_2);

	//p_tx_rx_queue_init(QUEUE_DATA_BUFF *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length);
}

#endif

//


}




void p_tx_rx_queue_init_dma(void){

p_tx_rx_queue_init(&q_tx_rx_queue_UsartDMAsend,&GV_ndb_UsartDMAsend,q_QUEUE_NODE_NUM_UsartDMAsend,
	q_QUEUE_NODE_DATA_LEN_UsartDMAsend,global_elems_UsartDMAsend,node_data_UsartDMAsend,global_node_buff_UsartDMAsend);

}




void Test_get_all_data_from_queue(UART_HandleTypeDef* uartHandle){


if(uartHandle->Instance == USART_A)
{
	p_pull_data_from_queue(&q_tx_rx_queue_2);

	//p_cbPrint_buff(GV_ndb_1.node_buff[0]);
	//SYSTEM_DEBUG(" 0: %d = %d = %d = %d ",GV_ndb_1.node_buff[0][0],GV_ndb_1.node_buff[0][1],GV_ndb_1.node_buff[0][2],GV_ndb_1.node_buff[0][3]);

}

}



void Test_all_queue(void){
//p_pull_data_from_queue(&q_rx_queue_1);
p_pull_data_from_queue(&q_tx_rx_queue_2);
//p_pull_data_from_queue(&q_tx_rx_queue_3);

	
}






