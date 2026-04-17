


#include "./test/bsp_usart_test.h"
#include "./usart/bsp_usart.h"
 #include "./sys/sysio.h"
#include "./sys/bsp_systime.h" 

#include "./DataConvert/data_random.h"
#include "./buffer/p_data_queue_outer.h"
#include "./usart/bsp_usart_shell.h"
#include "./user_test_config.h"

extern UART_HandleTypeDef huart_a; 

//uint8_t txData2[] = "Second group data";
//#define SENDBUFF_SIZE                     		10

extern DMA_HandleTypeDef hdma_usart1_tx;
//extern __IO ITStatus UartReady ;


extern Q_QueueBuffer q_tx_rx_queue_3;
















 uint8_t TEXT_TO_SEND[]={"ALIENTEK Apollo STM32H7 DMA \n"}; 
 uint8_t TEXT_TO_SEND2[]={"helluuu world!\n"}; 
extern UART_HandleTypeDef huart_a;

extern DMA_HandleTypeDef hdma_usart1_rx;
 
 
 
 uint16_t usart1_dma_rx_len = 0;
 
// 接收缓冲区（循环模式下，缓冲区大小建议 ≥ 最大单次接收数据长度）
#define USART1_DMA_RX_BUF_SIZE 256
uint8_t usart1_dma_rx_buf[USART1_DMA_RX_BUF_SIZE] = {1,2,6,9,6};
 // 接收状态：记录已接收数据长度（循环模式下需计算偏移）

 
void USART_DMA_Test(void){

//uint8_t Senbuff[20] ;  //定义数据发送数组

//uint8_t Senbuff2[20] ;

//	uart1_tx_complete=0;
//	HAL_UART_Transmit_DMA(&huart_a, (uint8_t *)Senbuff, sizeof(Senbuff)); 
//	while(HAL_UART_GetState(&huart_a) == HAL_UART_STATE_BUSY_TX);

	
	  // 启动 DMA 接收：从 USART1 接收数据到缓冲区，长度为缓冲区大小
//  HAL_UART_Receive_DMA(&huart_a, usart1_dma_rx_buf, USART1_DMA_RX_BUF_SIZE);
	

  while (1)
  {
// 已接收长度 = 缓冲区大小 - 剩余空闲大小
//usart1_dma_rx_len = USART1_DMA_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
//		HAL_Delay(1000);
//    // 处理接收的数据（如打印、解析）
//    if (usart1_dma_rx_len > 0) {
//      HAL_UART_Transmit(&huart_a, (uint8_t*)"Received: ", 10, 100);
//      HAL_UART_Transmit(&huart_a, usart1_dma_rx_buf, usart1_dma_rx_len, 100);
//      HAL_UART_Transmit(&huart_a, (uint8_t*)"\r\n", 2, 100);
//      usart1_dma_rx_len = 0;  // 重置接收长度
//    }
//		HAL_UART_Transmit(&huart_shell_Handle, usart1_dma_rx_buf, 10, 100);
//		 Test_USART_SHELL_print();
//TEST_USART_RX_DMA_SEGGERprint1();HAL_Delay(200);
//Test_Usart_QueueSaveFun();HAL_Delay(5200);
/* USER CODE END WHILE */

/* USER CODE BEGIN 3 */
}


}


#if 0
void USART_DMA_Test_success01(void){
uint8_t Senbuff[] = {1,1,1,1,1,0};  //定义数据发送数组
uint8_t Senbuff2[] = "Hello Windows2!\n"; 
uint8_t Senbuff3[] = "Hello Windows3!\n"; 


	uart1_tx_complete=0;
	HAL_UART_Transmit_DMA(&huart_a, (uint8_t *)Senbuff, sizeof(Senbuff)); 
	while(HAL_UART_GetState(&huart_a) == HAL_UART_STATE_BUSY_TX);

	HAL_UART_Transmit_DMA(&huart_a, (uint8_t *)Senbuff2, sizeof(Senbuff2)); 
		while(HAL_UART_GetState(&huart_a) == HAL_UART_STATE_BUSY_TX);

  while (1)
  {

		
		HAL_Delay(500);

static int i = 0;

//		HAL_Delay(500);
		if(uart1_tx_complete==1){
			
			HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_10);
			if(i++ % 2)
{

HAL_UART_Transmit_DMA(&huart_a,TEXT_TO_SEND,sizeof(TEXT_TO_SEND));
}
else{
HAL_UART_Transmit_DMA(&huart_a,TEXT_TO_SEND2,sizeof(TEXT_TO_SEND2));
}
		uart1_tx_complete=0;
		}

	}
}

#endif










