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
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_usart.h"
#include "./buffer/p_data_queue_outer.h"
#include "./buffer/que.h"


#define INTERCOM_TX_BUFF_SIZE    512
uint8_t tx_buff[INTERCOM_TX_BUFF_SIZE];// 发送缓冲区
__IO ITStatus UartReady = RESET;

extern uint8_t ucTemp;  

UART_HandleTypeDef huart_a;




//DMA_HandleTypeDef hdma_usart1_rx;
//DMA_HandleTypeDef hdma_usart1_tx;
 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void USART_A_Config(void)
{ 
  
  huart_a.Instance          = USART_A;
  
  huart_a.Init.BaudRate     = USART_A_BAUDRATE;
  huart_a.Init.WordLength   = UART_WORDLENGTH_8B;
  huart_a.Init.StopBits     = UART_STOPBITS_1;
  huart_a.Init.Parity       = UART_PARITY_NONE;
  huart_a.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart_a.Init.Mode         = UART_MODE_TX_RX;
  
  
	
	
	if (HAL_UART_Init(&huart_a) != HAL_OK)
    {
        //Error_Handler(); // 初始化失败处理
    }
		#if !(USE_LETTER_SHELL)
		#if USE_SERIAL_IT
	p_tx_rx_queue_init_all(&huart_a);
	
	//  /*配置串口接收、空闲中断 */
  __HAL_UART_ENABLE_IT(&huart_a,UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart_a,UART_IT_IDLE);
		#endif
		

		
		#endif
}







/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{  

    GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART_A)
  {
  USART_A_CLK_ENABLE();
	
	USART_A_RX_GPIO_CLK_ENABLE();
  USART_A_TX_GPIO_CLK_ENABLE();
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = USART_A_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USART_A_TX_AF;
  HAL_GPIO_Init(USART_A_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = USART_A_RX_PIN;
  GPIO_InitStruct.Alternate = USART_A_RX_AF;
  HAL_GPIO_Init(USART_A_RX_GPIO_PORT, &GPIO_InitStruct); 
 
 

	
	}  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
	
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}




void USART_Config_A(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
      
  USART_A_RX_GPIO_CLK_ENABLE();
  USART_A_TX_GPIO_CLK_ENABLE();
  /* 使能 UART 时钟 */
  USART_A_CLK_ENABLE();
	
  huart_a.Instance          = USART_A;
  huart_a.Init.BaudRate     = USART_A_BAUDRATE;
  huart_a.Init.WordLength   = UART_WORDLENGTH_8B;
  huart_a.Init.StopBits     = UART_STOPBITS_1;
  huart_a.Init.Parity       = UART_PARITY_NONE;
  huart_a.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart_a.Init.Mode         = UART_MODE_TX_RX;
	huart_a.Init.OverSampling = UART_OVERSAMPLING_16;
  
  HAL_UART_Init(&huart_a); 
	
	
	
   /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = USART_A_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USART_A_RX_AF;
  HAL_GPIO_Init(USART_A_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = USART_A_TX_PIN;
  GPIO_InitStruct.Alternate = USART_A_TX_AF;
  HAL_GPIO_Init(USART_A_TX_GPIO_PORT, &GPIO_InitStruct);
  
  

	
// 初始化时配置（例如在MX_USART1_UART_Init中）

	
}




UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;


/* 初始化函数（由 CubeMX 生成，此处简化） */
void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
		  // 关联UART句柄与DMA发送句柄（F429 必须配置）
  __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
}

void MX_DMA_Init2(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();

    hdma_usart1_tx.Instance = DMA1_Stream0;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK) {
        Error_Handler();
    }
  // 使能 DMA 传输完成 + 错误中断
  __HAL_DMA_ENABLE_IT(&hdma_usart1_tx, DMA_IT_TC | DMA_IT_TE);

  // 配置 NVIC，使能 DMA2 Stream7 中断
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 1, 0); // 抢占优先级1，子优先级0
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

static uint8_t txBuffer[] = "Hello from STM32F429 UART1 DMA!\r\n";

/* UART 发送函数（非阻塞） */
void UART_Send_DMA(void)
{
    HAL_UART_Transmit_DMA(&huart1, txBuffer, strlen((char*)txBuffer));
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 USART1 和 GPIOA 时钟
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // PA9 (TX) 配置
  GPIO_InitStruct.Pin = USART_A_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USART_A_TX_AF;
  HAL_GPIO_Init(USART_A_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = USART_A_RX_PIN;
  GPIO_InitStruct.Alternate = USART_A_RX_AF;
  HAL_GPIO_Init(USART_A_RX_GPIO_PORT, &GPIO_InitStruct); 
}


void um(void){

    MX_GPIO_Init();             // 如 LED 控制
    MX_DMA_Init2();
    MX_USART1_UART_Init();
}











//extern UART_HandleTypeDef huart1;





















// 发送一个字符
void USART_A_SendChar(uint8_t ch)
{
    HAL_UART_Transmit(&huart_a, &ch, 1, HAL_MAX_DELAY);
}

// 接收一个字符
uint8_t USART_A_ReceiveChar(void)
{
    uint8_t ch;
	while (__HAL_UART_GET_FLAG(&huart_a, UART_FLAG_RXNE) == RESET){};
    HAL_UART_Receive(&huart_a, &ch, 1, HAL_MAX_DELAY);
    return ch;
}










/*****************  发送字符串 **********************/
//void Usart_SendString(uint8_t *str)
//{
//	unsigned int k=0;
//  do 
//  {
//      HAL_UART_Transmit(&huart_printf,(uint8_t *)(str + k) ,1,1000);
//      k++;
//  } while(*(str + k)!='\0');
//  
//}


/////重定向c库函数scanf到串口USART_A，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//		
//	int ch;
//	while (__HAL_UART_GET_FLAG(&huart_printf, UART_FLAG_RXNE) == RESET);
//	HAL_UART_Receive(&huart_printf, (uint8_t *)&ch, 1, 1000);	
//	return (ch);
//}











/*********************************************END OF FILE**********************/
