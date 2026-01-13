

#include "nr_shell_cfg.h" 
#include "FreeRTOS.h"
#include "nr_micro_shell.h"
#include "semphr.h"

#include "cmsis_os.h"
//#include "cmsis_os2.h"
 #include "./usart/que.h"
 #include "user_config.h"
// #include "./usart/bsp_usart.h"



 #include "./usart/bsp_usart_shell.h"






/* 创建接收队列 */
creat_que(rx_nr_shell_que, 40);

/**************************************************************************
* 函数名称： Uart0_IRQHandler
* 功能描述： 串口接收中断
**************************************************************************/
#if 1
#if (USE_OS&USE_NR_MICRO_SHELL)


static char ch;
osSemaphoreId thread_uart_rx_sem;
osThreadId thread_shellTaskHandle;
//osSemaphoreId_t thread_uart_rx_sem;
//osThreadId_t thread_shellTaskHandle;
/* 创建接收队列 */

void USART_SHELL_IRQHandler(void){

	HAL_UART_IRQHandler(&huart_shell_Handle);	
}

void HAL_UART_Shell_RxCpltCallback(void)
{

        osSignalSet(thread_shellTaskHandle, 0x01);
//	osThreadFlagsSet(thread_shellTaskHandle, 0x01);
//SYSTEM_DEBUG("111%c",ch);
		 InQue(rx_nr_shell_que, ch);
		HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t*)&ch, 1);
	
}



void thread_shell(void  const* pvParameters)
{
    unsigned char ch;
    shell_init();
    while(1)
    {
        while(!QueIsEmpty(rx_nr_shell_que))
        {
            OutQue(rx_nr_shell_que, &ch);
            shell(ch);
        }
//					printf("111");
        
				vTaskDelay(pdMS_TO_TICKS(100));
    }
}
#endif
#endif


#if USE_OS 




void nr_os_shell_init(void){


    osSemaphoreDef(thread_uart_rx_sem);
    thread_uart_rx_sem = osSemaphoreCreate(osSemaphore(thread_uart_rx_sem),1);
    osThreadDef(thread_shell, thread_shell, 3, 0, 128);
//	    osThreadDef(thread_shell, 3, 0, 128);
    thread_shellTaskHandle = osThreadCreate(osThread(thread_shell), NULL);
	// 1. 定义信号量属性（替代 V1 的 osSemaphoreDef 宏）

	
//	osSemaphoreAttr_t thread_uart_rx_sem_attr = {
//    .name = "thread_uart_rx_sem"  // 信号量名称（可选，用于调试）
//		};
//	
//	thread_uart_rx_sem = osSemaphoreNew(1, 1, &thread_uart_rx_sem_attr);
//	// 3. 定义线程属性（替代 V1 的 osThreadDef 宏）
//osThreadAttr_t thread_shell_attr = {
//    .name = "thread_shell",        // 线程名称
//    .priority = osPriorityNormal,  // 线程优先级（与原 V1 一致）
//    .stack_size = 128              // 线程栈大小（与原 V1 一致）
//};
//	// 4. 创建线程（替代 V1 的 osThreadCreate）
//// 参数：线程入口函数，传入参数（NULL），属性指针
//thread_shellTaskHandle = osThreadNew(thread_shell, NULL, &thread_shell_attr);
	
	

HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t*)&ch, 1);

}

#else





void nr_no_os_it_shell_init(void){


shell_init();
//USART_ITConfig(USARTx_A, USART_IT_RXNE, ENABLE);
	__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_RXNE);
	
	
}



void nr_no_os_while_shell_init(void){
shell_init();
	


while(1){
	shell(USART_Shell_ReceiveChar());//循环中获取命令，不需要中断复位

}


}

#endif








