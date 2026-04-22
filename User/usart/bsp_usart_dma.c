/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "./usart/bsp_usart_dma.h"
#include "./sys/sysio.h"

#define DEBUG_DmaUsartPrint 0

//  #include "./usart/bsp_usart.h"
/* USER CODE BEGIN 0 */
DMA_HandleTypeDef hdma_usartx_rx;
DMA_HandleTypeDef hdma_usartx_tx;
//extern UART_HandleTypeDef huart_DMA_Handle;

STR_RCV_DMA_que_data RcvDmaQueData={0};
// 修改缓冲区定义，增加对齐属性
void USART_RX_DMA_SEGGERprint_OneByte(void);

	#if USE_OS
#include "FreeRTOS.h"	
#include "semphr.h"
extern SemaphoreHandle_t g_com_uart_send_sem;
//extern SemaphoreHandle_t g_com_nextdata_send_sem;
extern __IO uint32_t NowUse_rtos;
	#endif

str_DMA_usart_send GV_usartdmaSend = {
    .send_data = {0},  // 数组初始化为全0（字符串结束符+无效数据清0）
    .uart_tx_justSaveOver = 0,  // 初始化为未保存完成
    .uart_tx_thisdatas_sendover = 0,      // 初始化为未发送完成
    .read_out_len = 0,          // 初始读取长度为0
    .daret = 0,                 // 按需初始化（根据实际用途设值）
    .complete_timeout = TX_WAITTING_TIMEOUT,   // 超时阈值3000ms（可按需调整）
    .current_time = 0,          // 初始时间设0（后续用HAL_GetTick()更新）
    .last_tx_complete_time = 0  // 初始无上次触发时间
};


//static uint8_t g_rcvDataBuf[MAX_BUF_SIZE];
//static uint8_t received_data_len = 0;

// 定义环形缓冲区大小（根据需求调整，此处设为128字节）


void USARTx_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 2, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//  /* DMA2_Stream7_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 2, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}



void USART_TX_DMA_Config(UART_HandleTypeDef* uartHandle){

  if(uartHandle->Instance==DMA_USARTx)
  {
    /* USART1_TX Init */
    hdma_usartx_tx.Instance = DMA1_Stream0;
    hdma_usartx_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usartx_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usartx_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usartx_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usartx_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usartx_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usartx_tx.Init.Mode = DMA_NORMAL;
    hdma_usartx_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_usartx_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

		
    if (HAL_DMA_Init(&hdma_usartx_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&huart_DMA_Handle,hdmatx,hdma_usartx_tx);

		  // 3. 使能 DMA 中断（半传输 + 传输完成 + 传输错误）
//  __HAL_DMA_ENABLE_IT(&hdma_usartx_tx, DMA_IT_HT | DMA_IT_TC | DMA_IT_TE);
		__HAL_DMA_ENABLE_IT(&hdma_usartx_tx,  DMA_IT_TC | DMA_IT_TE);//与发送相关
		
		  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 2, 0);
			HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	}

}


void USART_RX_DMA_Config(UART_HandleTypeDef* uartHandle){

  if(uartHandle->Instance==DMA_USARTx)
  {
    /* USART1_RX Init */
    hdma_usartx_rx.Instance = DMA1_Stream1;
    hdma_usartx_rx.Init.Request = DMA_REQUEST_USART1_RX;
    hdma_usartx_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usartx_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usartx_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usartx_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usartx_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usartx_rx.Init.Mode = DMA_NORMAL;
    hdma_usartx_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usartx_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		
    if (HAL_DMA_Init(&hdma_usartx_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usartx_rx);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	}

}





extern SemaphoreHandle_t uart_send_res_sem;

//DMA传输完成自动调用（发送）
void HAL_USARTx_DMA_TxCpltCallback(void) {


      GV_usartdmaSend.uart_tx_thisdatas_sendover  = 1;  // 全部发送完成
	
//SYSTEM_DEBUG("TX Callback \n");
		
	
	#if USE_OS
	    // 必须：判断信号量是否有效
    if(uart_send_res_sem != NULL)
    {
        // 【中断上下文 专用】释放二值信号量 / 计数信号量
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xSemaphoreGiveFromISR( uart_send_res_sem, &xHigherPriorityTaskWoken);

        // 强制任务切换（必须加）
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
	#endif
	
  
}









// RingBuffer_t g_uartRingBuf;
// uint8_t g_ringBufData[MAX_RING_BUFF_SIZE];


void USART_TX_RX_DMA_Config(UART_HandleTypeDef* uartHandle){


USART_RX_DMA_Config(uartHandle);
USART_TX_DMA_Config(uartHandle);

  if(uartHandle->Instance==DMA_USARTx)
  {
#if USE_UART_DMA_RX
// 连续读取直到 RXNE 标志位清零，确保硬件缓冲区没有任何残留



 QueueInit(&RcvDmaQueData.g_uartRingBuf, RcvDmaQueData.g_ringBufData, MAX_RING_BUFF_SIZE);//接收用环形缓冲区,
		

//	SCB_CleanInvalidateDCache_by_Addr((uint32_t *)RcvDmaQueData.g_rcvDataBuf, MAX_BUF_SIZE);	
HAL_UART_Receive_DMA(uartHandle, RcvDmaQueData.g_rcvDataBuf, MAX_BUF_R_SIZE);             // 开启DMA接收

		memset(RcvDmaQueData.g_rcvDataBuf, 0, MAX_BUF_R_SIZE); // 清空内存，在__HAL_UART_CLEAR_IDLEFLAG前Received:为0，在他之后，Received:为1
		
// 注意：H7 清除 IDLE 标志需要写 ICR 寄存器，HAL 宏已封装
__HAL_UART_CLEAR_IDLEFLAG(uartHandle);



		// 彻底清除 RXNE 标志 + 清空 RDR 中的 0x00 残留（结合 ISR 读取）/* 2. 强制读取 ISR 寄存器，确保状态同步并配合后续清除动作 */
volatile uint32_t temp_isr = huart_DMA_Handle.Instance->ISR;  // 先读 ISR 寄存器(必须有)
		(void)temp_isr;  // 消除未使用变量警告

//// 检查 IDLE 标志位 (第 4 位)
//if (temp_isr & USART_ISR_IDLE) {
//    // 如果进入这里，说明使能中断前 IDLE 已经是 1 了
//    SYSTEM_DEBUG("Before Enable IT: IDLE Flag is SET (1)\n");
//} else {
//    SYSTEM_DEBUG("Before Enable IT: IDLE Flag is RESET (0)\n");
//}
/* 4. 再次确保数据寄存器是空的（可选） */
volatile uint32_t temp_rdr = huart_DMA_Handle.Instance->RDR;  // 再读 RDR 寄存器，硬件自动清零 RXNE
(void)temp_rdr;  // 消除未使用变量警告
//HAL_Delay(2);
		    // 3.3 主动清除RXNE标志（双重保障）
    __HAL_UART_CLEAR_FLAG(&huart_DMA_Handle, UART_FLAG_RXNE);
		
//__HAL_UART_ENABLE_IT(&huart_DMA_Handle, UART_IT_RXNE); 
__HAL_UART_ENABLE_IT(&huart_DMA_Handle, UART_IT_IDLE);    // 使能串口接收空闲中断

	HAL_NVIC_SetPriority(USARTx_DMA_IRQ, 3, 0);	      // 抢占优先级0，子优先级0
	HAL_NVIC_EnableIRQ(USARTx_DMA_IRQ);		          // 使能USART1中断通道  
		HAL_Delay(1);//必须有延迟才能正常工作（非常重要）
//USART_RX_DMA_SEGGERprint_OneByte();
//		SYSTEM_DEBUG("over4");
#endif

	}

}

void USART_TX_RX_DMA_ConfigALL(void){
	USARTx_DMA_Init();
USART_TX_RX_DMA_Config(&huart_DMA_Handle);
	
p_tx_rx_queue_init_dma();
}


void HAL_USARTx_DMA_ErrorCallback(void) {

    uint32_t error_code = HAL_UART_GetError(&huart_DMA_Handle);  // 获取错误码
    SYSTEM_DEBUG("UART Error: 0x%lx\n", error_code);  // 打印错误（需重定向printf）
    // 常见错误：0x04（溢出错误，数据未及时读取导致覆盖）
  
}








#include "./DataConvert/data_random.h"

#if TESTUsartDMASendSaveAndSend
//(发送)测试发送前保存程序，之后才能从缓冲区取出数据通过dma发送出去
void Test_Usart_DMA_SendSaveFun(void){
uint8_t Senbuff[q_QUEUE_NODE_DATA_LEN_UsartDMAsend] ;
//	fill_data_False_random((char *)Senbuff,20);
//	Senbuff[10]='\0';
//	SYSTEM_DEBUG("ddd =  %d  %d\n",sizeof(Senbuff),strlen((char *)Senbuff));
//	Senbuff[10]='\0';
//	SYSTEM_DEBUG("ddd =  %d  %d\n",sizeof(Senbuff),strlen((char *)Senbuff));
for(uint16_t i=0;i<q_QUEUE_NODE_NUM_UsartDMAsend;i++){
	
fill_data_False_random((char *)Senbuff,40);
//SYSTEM_DEBUG("string = %s\n",Senbuff);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,Senbuff,40,"send string = %s\n",Senbuff);
//	int ret = p_push_data_to_queue2(&q_tx_rx_queue_UsartDMAsend, (char *)Senbuff, sizeof(Senbuff)); // 不含'\0'
	Usart_SendDMA_SaveFun((char *)Senbuff,sizeof(Senbuff));

}
}
#endif


	
	
void Usart_SendDMA_SaveFun(char *Sendbuff,uint16_t buff_len){

	GV_usartdmaSend.dbret = p_push_data_to_queue2(&q_tx_rx_queue_UsartDMAsend, (char *)Sendbuff, buff_len); // 不含'\0'
	GV_usartdmaSend.uart_tx_justSaveOver = 1;
	GV_usartdmaSend.uart_tx_alldatas_sendfinish = 0;
	#if USE_OS
	//释放信号量
	
	
//				if(NowUse_rtos==1){
//        xSemaphoreGive(g_com_uart_send_sem);
//			}
	#endif
	
	#if DEBUG_DmaUsartPrint	
if (GV_usartdmaSend.dbret == 1) {
        SYSTEM_DEBUG("p_push_data_to_queue2 full,error\n");
    }
#endif	
}


//DMA发送（通过分组缓冲区的数据）
void Usart_SendDMA_SendFun(UART_HandleTypeDef *huart_x){
	
GV_usartdmaSend.current_time = HAL_GetTick();
if (GV_usartdmaSend.uart_tx_justSaveOver==1 || GV_usartdmaSend.uart_tx_thisdatas_sendover == 1 || 
    (GV_usartdmaSend.current_time - GV_usartdmaSend.last_tx_complete_time >GV_usartdmaSend.complete_timeout)) {
			
				if(GV_usartdmaSend.current_time - GV_usartdmaSend.last_tx_complete_time > GV_usartdmaSend.complete_timeout){
					GV_usartdmaSend.uart_tx_justSaveOver=0;
#if DEBUG_DmaUsartPrint					
	SYSTEM_DEBUG("over time ==:%d\n", GV_usartdmaSend.current_time - GV_usartdmaSend.last_tx_complete_time);
#endif	
	}
//				
			 GV_usartdmaSend.daret = p_pop_data_from_queue2(&q_tx_rx_queue_UsartDMAsend, (char *)GV_usartdmaSend.send_data, sizeof(GV_usartdmaSend.send_data), &GV_usartdmaSend.read_out_len);
//	SYSTEM_DEBUG("here : %d\n",GV_usartdmaSend.daret);
		if (GV_usartdmaSend.daret == 0) {
			GV_usartdmaSend.uart_tx_thisdatas_sendover=0;
//			SYSTEM_DEBUG("pop dara:====\n" );
			HAL_UART_Transmit_DMA(huart_x,GV_usartdmaSend.send_data,GV_usartdmaSend.read_out_len);
			
			if(GV_usartdmaSend.read_out_len==0){
			GV_usartdmaSend.uart_tx_alldatas_sendfinish = 1;
			
			}
//			SYSTEM_DEBUG("pop dara: %s , length:%d\n", GV_usartdmaSend.send_data, sizeof(GV_usartdmaSend.send_data));
			
    } else if (GV_usartdmaSend.daret == 1) {
#if DEBUG_DmaUsartPrint	
        SYSTEM_DEBUG("empty data %d %d %d\n",GV_usartdmaSend.current_time, GV_usartdmaSend.last_tx_complete_time, GV_usartdmaSend.uart_tx_thisdatas_sendover);
#endif	
//			SYSTEM_DEBUG("empty data%d\n",GV_usartdmaSend.read_out_len);
			GV_usartdmaSend.complete_timeout = TX_WAITTING_TIMEOUT;
//			HAL_Delay(1000);
			GV_usartdmaSend.uart_tx_thisdatas_sendover=0;
    } else if (GV_usartdmaSend.daret == 2) {
#if DEBUG_DmaUsartPrint
        SYSTEM_DEBUG("butter too small,cant receive data\n");
#endif	
    }
			
//			HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_10);
 GV_usartdmaSend.last_tx_complete_time = HAL_GetTick();
		
		}

//		SYSTEM_DEBUG("%d = %d = %d= %d\n",GV_usartdmaSend.uart_tx_justSaveOver,GV_usartdmaSend.uart_tx_thisdatas_sendover,
//		GV_usartdmaSend.current_time - GV_usartdmaSend.last_tx_complete_time,GV_usartdmaSend.current_time);
		
		
}


//DMA发送数据(发送)
void Usart_SendFUN_ALL(void){

Usart_SendDMA_SendFun(&huart_DMA_Handle);

}



#if TESTUsartDMASendSaveAndSend
void TEST_Usart_SendFUN_ALL(void){
	Test_Usart_DMA_SendSaveFun();
	 SYSTEM_DEBUG("send----------------");
	for(uint16_t i=0;i<q_QUEUE_NODE_NUM_UsartDMAsend;i++){
	Usart_SendFUN_ALL();
		HAL_Delay(1000);
	}
}
#endif	

///--------------------------

//仅测试ringbuffer功能
#if TEST_RingBuffer_SAVEANDREAD
void Test_Usart_QueueSaveFun(void){
uint8_t Quebuff[MAX_RING_BUFF_SIZE] ;
	uint8_t RT_data_len;
	uint8_t RTDataBuf[MAX_RING_BUFF_SIZE];
	STR_RCV_DMA_que_data RcvDmaQue_Data_t;
//	fill_data_False_random((char *)Senbuff,20);
//	Senbuff[10]='\0';
//	SYSTEM_DEBUG("ddd =  %d  %d\n",sizeof(Senbuff),strlen((char *)Senbuff));
//	Senbuff[10]='\0';
//	SYSTEM_DEBUG("ddd =  %d  %d\n",sizeof(Senbuff),strlen((char *)Senbuff));
	RingBuffer_Init(&RcvDmaQue_Data_t.g_uartRingBuf, RcvDmaQue_Data_t.g_rcvDataBuf, MAX_RING_BUFF_SIZE);
	
for(uint16_t icr=1;icr<10;icr++){

fill_data_False_random((char *)Quebuff,icr);


	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,Quebuff,icr,"\n send string  %d \n",icr);

//	int ret = p_push_data_to_queue2(&q_tx_rx_queue_UsartDMAsend, (char *)Senbuff, sizeof(Senbuff)); // 不含'\0'
	RingBuffer_WriteMulti(&RcvDmaQue_Data_t.g_uartRingBuf,(uint8_t *)Quebuff,icr);//write data

	RT_data_len=RingBuffer_ReadMulti(&RcvDmaQue_Data_t.g_uartRingBuf,RTDataBuf,MAX_RING_BUFF_SIZE);//get data
	
	if(RT_data_len>0){
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,RTDataBuf,RT_data_len,"S AND R :%d \n",i);
	}
	SYSTEM_DEBUG("------- \n");
	HAL_Delay(1000);
}
}


#endif

//一个一个字节读取BUFF数据
void USART_RX_DMA_SEGGERprint_OneByte(void){
    uint8_t keyVal;
    uint8_t recv_data; // 临时存储读取到的数据
//	SYSTEM_INFO("Read: %d, Write: %d\n", RcvDmaQueData.g_uartRingBuf.read_idx, RcvDmaQueData.g_uartRingBuf.write_idx);
        while (QueuePop(&RcvDmaQueData.g_uartRingBuf, &recv_data)==QUEUE_OK) {
            // 以十六进制和ASCII格式打印（根据需求选择）
            SYSTEM_INFO("Received: 0x%02X (%c)\n", recv_data, (recv_data >= 0x20 && recv_data <= 0x7E) ? recv_data : '.');
        }
}


//多个字节读取BUFF数据
void USART_RX_DMA_SEGGERprint_MultByte(void){
	uint8_t RDataBuf[MAX_RING_BUFF_SIZE];
	uint8_t R_data_len;
	R_data_len = QueuePopArray(&RcvDmaQueData.g_uartRingBuf,RDataBuf,MAX_RING_BUFF_SIZE);
	
	if(R_data_len>0){
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,RDataBuf,R_data_len," R DMA D : %d",R_data_len);
	}
	

}
#if TEST_DMA_RB_READ
void TEST_USART_RX_DMA_ALL(void){
//	需要先初始化USART_TX_RX_DMA_ConfigALL
//Test_Usart_QueueSaveFun();//存入buff
	#if 1
USART_RX_DMA_SEGGERprint_OneByte();
	#else
USART_RX_DMA_SEGGERprint_MultByte();	
	#endif
}



void TEST_USART_RX_DMA_ALL_while(void){

while(1){

TEST_USART_RX_DMA_ALL();
	HAL_Delay(200);
}
}
#endif

///--------------------------
//weak void HAL_USARTx_DMA_RxCpltCallback(void){}

#if (USE_UARTx_DMA)
#if (USE_UART_DMA_RX)
void HAL_USARTx_DMA_RxCpltCallback(void){
#if 1
    // 循环模式下，缓冲区满后的数据长度 = 缓冲区大小
RcvDmaQueData.received_data_len = MAX_BUF_R_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usartx_rx);

		 if (RcvDmaQueData.received_data_len > 0) {
            QueuePushArray(&RcvDmaQueData.g_uartRingBuf, RcvDmaQueData.g_rcvDataBuf, RcvDmaQueData.received_data_len);
			 
        }
SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,RcvDmaQueData.g_rcvDataBuf,5,"finish %d",RcvDmaQueData.received_data_len);
		HAL_UART_Receive_DMA(&huart_DMA_Handle, RcvDmaQueData.g_rcvDataBuf, MAX_BUF_R_SIZE);             // 重新开启DMA传输
	#endif
//HAL_UART_Receive_IT(&huart_shell_Handle, &RcvDmaQueData.g_rcvDataBuf, 1);
}
#endif








#if (USE_UART_DMA)
#if (USE_UART_DMA_RX)

void USARTx_DMA_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
#if 1
  /* USER CODE END USART1_IRQn 0 */
  	if (__HAL_UART_GET_FLAG(&huart_DMA_Handle, UART_FLAG_IDLE) != RESET)                 // 获取接收IDLE标志位是否被置位
	{           
		__HAL_UART_CLEAR_IDLEFLAG(&huart_DMA_Handle); 
		HAL_UART_DMAStop(&huart_DMA_Handle);
//		// 必须无效化 Cache 才能读到真实数据
//        SCB_InvalidateDCache_by_Addr((uint32_t *)RcvDmaQueData.g_rcvDataBuf, MAX_BUF_SIZE);
		
		                                             // 停止DMA传输，防止干扰
		RcvDmaQueData.received_data_len = MAX_BUF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usartx_rx);
//SYSTEM_DEBUG("RcvDmaQueData.received_data_len %d",RcvDmaQueData.received_data_len);
//		SCB_InvalidateDCache_by_Addr((uint32_t *)RcvDmaQueData.g_rcvDataBuf, MAX_BUF_SIZE);
		        if (RcvDmaQueData.received_data_len > 0) {
            QueuePushArray(&RcvDmaQueData.g_uartRingBuf, RcvDmaQueData.g_rcvDataBuf, RcvDmaQueData.received_data_len);
							
        }
						SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,RcvDmaQueData.g_rcvDataBuf,5,"rec=== %d",RcvDmaQueData.received_data_len);
		HAL_UART_Receive_DMA(&huart_DMA_Handle, RcvDmaQueData.g_rcvDataBuf, MAX_BUF_SIZE);             // 重新开启DMA传输
	}
#endif	
	HAL_UART_IRQHandler(&huart_DMA_Handle);//发送DMA必须有
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

#else
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart_DMA_Handle);//发送DMA必须有
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}



#endif
#endif



/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usartx_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usartx_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}
#endif











/* USER CODE END 1 */



