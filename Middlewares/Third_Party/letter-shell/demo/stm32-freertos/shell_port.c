/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
//#include "serial.h"
#include "stm32_FH_xxx.h"
//#include "usart.h"
//#include "cevent.h"
#include "log.h"
#include "./usart/bsp_usart_shell.h"
 #include "./buffer/que.h"
 #include "semphr.h"
#include "./user_config.h"
#include "cmsis_os.h"
 #include "./shell_port.h"
 #include "./sys/sysio.h"
 
Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;
//osSemaphoreId thread_uart_rx_sem;
osThreadId thread_shellTaskHandle;
/* 创建接收队列 */
//creat_que(rx_letter_shell_que, 40);
static char ch;
#if (USE_LETTER_SHELL&&!USE_OS)
void USART_SHELL_IRQHandler(void){

	HAL_UART_IRQHandler(&huart_shell_Handle);	
}

void HAL_UART_Shell_RxCpltCallback(void)
{
#if USE_OS
        osSignalSet(thread_shellTaskHandle, 0x01);
#endif
//	osThreadFlagsSet(thread_shellTaskHandle, 0x01);
//SYSTEM_DEBUG("111%c",ch);
//		 InQue(rx_letter_shell_que, ch);
	shellHandler(&shell, ch);

		HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t*)&ch, 1);

}
#endif

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
//    serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
	HAL_UART_Transmit(&huart_shell_Handle, (uint8_t *)data, len, 0xFFFF);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
	HAL_StatusTypeDef status;
//    return serialReceive(&debugSerial, (uint8_t *)data, len, 0);
//	while (__HAL_UART_GET_FLAG(&huart_shell_Handle, UART_FLAG_RXNE) == RESET){};
	status = HAL_UART_Receive(&huart_shell_Handle, (uint8_t *)data, 1, 0xFFFF);
	    if (status == HAL_OK) {
        return 1;
    } else {
        return 0;
    }
	
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void LetterShell_OS_Init(void)
{
    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
    shell.read = userShellRead;
	#if SHELL_USING_LOCK == 1
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
	#endif
    shellInit(&shell, shellBuffer, 512);
    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS)
    {
        logError("shell task creat failed");
    }
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, LetterShell_OS_Init);


void letterShell_no_os_WhileInit(void)
{
	//注册自己实现的写函数
    shell.write = userShellWrite;
	shell.read = userShellRead;
	//调用shell初始化函数
    shellInit(&shell, shellBuffer, 512);
//	__HAL_UART_CLEAR_NEFLAG(&huart_shell_Handle);
//volatile uint32_t temp_isr = huart_shell_Handle.Instance->ISR;  // 先读 ISR 寄存器(必须有)
//		(void)temp_isr;
	__HAL_UART_ENABLE_IT(&huart_shell_Handle,UART_IT_RXNE);
}

void letter_Shell_NoOsWhileTask(void)
{
    Shell *tshell = &shell;
    char data;

    while(1)
    {
        if (tshell->read && tshell->read(&data, 1) == 1)
        {
            shellHandler(tshell, data);
        }
    }
}

