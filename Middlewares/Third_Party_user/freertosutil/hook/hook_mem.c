

#include "./hook/hook_mem.h"
//#include "./usart/bsp_usart.h"
#include "./rtosprintf/frtos_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"

#include "./sys/sysio.h"



// -------------------------- 全局配置（根据硬件调整） --------------------------
// 任务栈大小（单位：字，1字=4字节）
#define TASK1_STACK_DEPTH     128     // 极小栈（触发栈溢出）
#define TASK2_STACK_DEPTH     256     // 适中栈（正常运行）
#define TASK3_STACK_DEPTH     1024    // 较大栈（正常运行）
#define TOO_LARGE_STACK_DEPTH 32768   // 超大栈（创建失败）
#define TASKDelete_STACK_DEPTH	(TASK3_STACK_DEPTH*2)
// 任务句柄
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;
TaskHandle_t xTooLargeTaskHandle = NULL;

// 全局标记：控制栈溢出触发（避免重复递归）
static uint8_t bStackConsumeTriggered = 0;

//void SYSTEM_I_PRINT(fmt,arg...){
//if(NowUse_rtos_printf==0){

//logDebug(fmt,arg...);
//}else{

//printf(fmt,arg...);
//}

//}

/**
 * @brief 打印堆内存状态（辅助调试）
 */
// -------------------------- 工具函数 --------------------------
 void vPrintHeapState(const char *pcTag)
{
    // 关中断打印，避免截断
    //taskENTER_CRITICAL();
    SYSTEM_I_PRINT("[%s] Free Heap: %dByte | Min Ever Free Heap: %dByte\r\n",
           pcTag,
           (uint32_t)xPortGetFreeHeapSize(),
           (uint32_t)xPortGetMinimumEverFreeHeapSize());
    //taskEXIT_CRITICAL();
}

// -------------------------- FreeRTOS钩子函数（核心） --------------------------
/**
 * @brief 栈溢出钩子函数（模式2，最严格检测）
 * @param xTask: 溢出任务句柄 | pcTaskName: 溢出任务名
 * @note  FreeRTOS检测到栈溢出时自动调用，需在FreeRTOSConfig.h开启宏
 */
// -------------------------- 栈溢出钩子：强制删除任务（核心修复） --------------------------
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    taskENTER_CRITICAL();
    SYSTEM_I_PRINT("\r\n=====================================\r\n");
    SYSTEM_I_PRINT("[FATAL ERROR]Stack Overflow Detected!\r\n");
    SYSTEM_I_PRINT("Task Name    : %s\r\n", pcTaskName);
    SYSTEM_I_PRINT("Task Handle  : 0x%08X\r\n", (uint32_t)xTask);
    SYSTEM_I_PRINT("Current MSP  : 0x%08X\r\n", (uint32_t)__get_MSP());
    SYSTEM_I_PRINT("=====================================\r\n");
    
    // 强制删除溢出任务（无论句柄是否匹配）
    if(xTask != NULL)
    {
        vTaskDelete(xTask);
        SYSTEM_I_PRINT("[INFO] %s deleted due to stack overflow\r\n", pcTaskName);
        // 清空句柄，避免重复操作
        if(xTask == xTask1Handle) xTask1Handle = NULL;
    }
    
    taskEXIT_CRITICAL();
    // 溢出后直接返回，不阻塞
    return;
}

/**
 * @brief 堆分配失败钩子函数
 * @note  pvPortMalloc分配失败时调用（如超大栈任务创建失败）
 */
// -------------------------- 堆分配失败钩子 --------------------------
void vApplicationMallocFailedHook(void)
{
    taskENTER_CRITICAL();
    SYSTEM_I_PRINT("\r\n=====================================\r\n");
    SYSTEM_I_PRINT("[ERROR] Heap Allocation Failed!\r\n");
    vPrintHeapState("Malloc Failed");
    SYSTEM_I_PRINT("=====================================\r\n");
    taskEXIT_CRITICAL();
}

// 新增：任务创建结果通用打印函数（核心封装）
 void vPrintStack_TaskCreationResult(const char *pcTaskName, 
                                     BaseType_t xCreationResult, 
                                     uint32_t ulStackDepthInWords)
{
    // 关中断打印，和vPrintHeapState保持一致的线程安全策略
    taskENTER_CRITICAL();
    if(xCreationResult == pdPASS)
    {
        // 同时打印 Words 和 Bytes（更直观，可选）
        SYSTEM_I_PRINT("[OK] %s created (Stack: %d Words / %d Bytes)\r\n",
               pcTaskName,
               ulStackDepthInWords,
               ulStackDepthInWords * sizeof(StackType_t));
			
			  SYSTEM_I_PRINT("[After %s Creation] Free Heap: %dByte | Min Ever Free Heap: %dByte\r\n",
           pcTaskName,(uint32_t)xPortGetFreeHeapSize(),(uint32_t)xPortGetMinimumEverFreeHeapSize());
			
    }
    else
    {
//        SYSTEM_I_PRINT("[ERR] %s creation failed\r\n", pcTaskName);
			        SYSTEM_I_PRINT("[ERR] %s creation failed (EXPECTED, Need: %dByte, Free: %dByte)\r\n",pcTaskName,
               TOO_LARGE_STACK_DEPTH * sizeof(StackType_t),(uint32_t)xPortGetFreeHeapSize());
    }
    taskEXIT_CRITICAL();
}

// -------------------------- 测试任务定义 --------------------------
/**
 * @brief 测试任务1：极小栈，主动触发栈溢出
 */
#if 1
// -------------------------- 核心工具函数 --------------------------一直耗尽资源
/**
 * @brief 递归消耗栈空间函数（全局定义，避免嵌套）
 * @note  每次递归占用100字节栈，无限递归快速耗尽栈空间
 */
static void vStackConsume(void)
{
    uint8_t ucStackBuf[10] = {0};  // 占用栈空间
    memset(ucStackBuf, 0xFF, sizeof(ucStackBuf)); // 避免编译器优化
		vTaskDelay(pdMS_TO_TICKS(100));
		        SYSTEM_I_PRINT("Task1 (Min Stack)  | Remaining Stack: %dByte\r\n",
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
    vStackConsume(); // 无限递归，触发栈溢出
}

static void vTestTask1(void *pvParameters)
{
    uint32_t ulCount = 0;
    
    for(;;) // FreeRTOS推荐用for(;;)替代while(1)
    {
        // 打印任务运行状态+剩余栈空间
        SYSTEM_I_PRINT("Task1 (Min Stack) | Count: %d | Remaining Stack: %dByte\r\n",
               ulCount++,
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
        
        // 运行5次后触发栈溢出（仅触发一次）
        if(ulCount >= 5 && bStackConsumeTriggered == 0)
        {
            bStackConsumeTriggered = 1;
            SYSTEM_I_PRINT("Task1: Starting stack consume (overflow will occur)...\r\n");
            vStackConsume(); // 调用递归函数耗尽栈
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1秒循环
    }
}

#else
static uint32_t ulRecursionDepth = 0; // 递归深度计数（全局静态，避免嵌套）
/**
 * @brief 可控递归消耗栈函数（全局定义，解决嵌套错误）
 * @note  每递归5次主动让出CPU，避免霸占调度
 */
static void vStackConsume(TaskHandle_t xTask)
{
    uint8_t buf[100] = {0};  // 每次递归消耗100字节栈
    memset(buf, 0xFF, sizeof(buf)); // 避免编译器优化
    ulRecursionDepth++;

    // 核心：每递归5次触发调度，让高优先级任务运行
    if(ulRecursionDepth % 5 == 0)
    {
        taskYIELD();
    }

    // 有限递归：剩余栈>100字节时继续，避免无限霸占CPU
    if(uxTaskGetStackHighWaterMark(xTask) * sizeof(StackType_t) > 100)
    {
        vStackConsume(xTask);
    }
    else
    {
        taskYIELD(); // 栈不足时主动调度
    }
}
// -------------------------- 核心修改：Task1改为“可控递归+主动调度” --------------------------
/**
 * @brief Task1：极小栈，可控递归触发栈溢出（无嵌套函数）
 */
static void vTestTask1(void *pvParameters)
{
    uint32_t ulCount = 0;
    TaskHandle_t xSelfTask = xTaskGetCurrentTaskHandle(); // 获取自身任务句柄

    for(;;)
    {
        taskENTER_CRITICAL();
        SYSTEM_I_PRINT("Task1 (Min Stack) | Count: %d | Remaining Stack: %dByte\r\n",
               ulCount++,
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
        taskEXIT_CRITICAL();
        
        // 运行2次后触发递归（减少CPU霸占时间）
        if(ulCount >= 2)
        {
            taskENTER_CRITICAL();
            SYSTEM_I_PRINT("Task1: Starting stack consume (will yield CPU)...\r\n");
            taskEXIT_CRITICAL();
            ulRecursionDepth = 0; // 重置递归计数
            vStackConsume(xSelfTask); // 调用全局递归函数，无嵌套
            vTaskDelay(pdMS_TO_TICKS(10)); // 递归后主动延时
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#endif
/**
 * @brief 测试任务2：适中栈，正常运行
 */
static void vTestTask2(void *pvParameters)
{
    uint32_t ulCount = 0;
    
    for(;;)
    {
        SYSTEM_I_PRINT("Task2 (Medium Stack) | Count: %d | Remaining Stack: %dByte\r\n",
               ulCount++,
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
        
        vTaskDelay(pdMS_TO_TICKS(1500)); // 1.5秒循环
    }
}
static void vTestTaskDelete(void *pvParameters)
{
    uint32_t ulCount = 0;
    
    for(;;)
    {
        SYSTEM_I_PRINT("TaskDelete (Medium Stack) | Count: %d | Remaining Stack: %dByte\r\n",
               ulCount++,
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
        
        vTaskDelay(pdMS_TO_TICKS(1500)); // 1.5秒循环
    }
}
/**
 * @brief 测试任务3：较大栈，正常运行
 */
static void vTestTask3(void *pvParameters)
{
    uint32_t ulCount = 0;
    uint8_t ucLargeBuf[1500] = {0}; // 占用1500字节栈（验证大栈充足性）
    
    for(;;)
    {
        memset(ucLargeBuf, ulCount % 0xFF, sizeof(ucLargeBuf)); // 避免优化
        SYSTEM_I_PRINT("Task3 (Large Stack) | Count: %d | Remaining Stack: %dByte\r\n",
               ulCount++,
               (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
        
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2秒循环
    }
}

/**
 * @brief 任务创建器：统一创建所有测试任务
 */
static void vTaskCreator(void *pvParameters)
{
    BaseType_t xReturn;
     size_t ulFreeHeapBefore;
	
    SYSTEM_I_PRINT("\n=== Start Task Creation Test ===\r\n");
    vPrintHeapState("Before Creation");

    // 1. 创建任务1（极小栈，预期成功但触发溢出）
    xReturn = xTaskCreate(vTestTask1, "Task1", TASK1_STACK_DEPTH, 
                          NULL, tskIDLE_PRIORITY + 1, &xTask1Handle);
//    if(xReturn == pdPASS)
//        SYSTEM_I_PRINT("[OK] Task1 created (Stack: %d Words)\r\n", TASK1_STACK_DEPTH);
//    else
//        SYSTEM_I_PRINT("[ERR] Task1 creation failed\r\n");
		vPrintStack_TaskCreationResult("Task1", xReturn, TASK1_STACK_DEPTH);
//		vPrintHeapState("After Task1 Creation");
    // 2. 创建任务2（适中栈，预期成功）
    xReturn = xTaskCreate(vTestTask2, "Task2", TASK2_STACK_DEPTH, 
                          NULL, tskIDLE_PRIORITY + 2, &xTask2Handle);
		vPrintStack_TaskCreationResult("Task2", xReturn, TASK2_STACK_DEPTH);
//		vPrintHeapState("After Task2 Creation");
	
		 xReturn = xTaskCreate(vTestTaskDelete, "TaskDelete", TASKDelete_STACK_DEPTH, 
                          NULL, tskIDLE_PRIORITY + 2, &xTask2Handle);
		vPrintStack_TaskCreationResult("TaskDelete", xReturn, TASKDelete_STACK_DEPTH);
		
//		vPrintHeapState("After TaskDelete Creation");
		vTaskDelete(xTaskGetHandle("TaskDelete"));
		vPrintHeapState("After TaskDelete Delete");
	
	
    // 3. 创建任务3（较大栈，预期成功）
    xReturn = xTaskCreate(vTestTask3, "Task3", TASK3_STACK_DEPTH, 
                          NULL, tskIDLE_PRIORITY + 3, &xTask3Handle);
//    if(xReturn == pdPASS)
//        SYSTEM_I_PRINT("[OK] Task3 created (Stack: %d Words)\r\n", TASK3_STACK_DEPTH);
//    else
//        SYSTEM_I_PRINT("[ERR] Task3 creation failed\r\n");
		vPrintStack_TaskCreationResult("Task3", xReturn, TASK3_STACK_DEPTH);
		
    // 打印Task1-3创建后的堆状态
//    vPrintHeapState("After Task1-3 Creation");
		

//    ulFreeHeapBefore = xPortGetFreeHeapSize();
    // 4. 创建超大栈任务（预期失败，触发堆分配失败钩子）
    xReturn = xTaskCreate(vTestTask3, "TooLargeTask", (uint16_t)TOO_LARGE_STACK_DEPTH, 
                          NULL, tskIDLE_PRIORITY + 4, &xTooLargeTaskHandle);
		vPrintStack_TaskCreationResult("TooLargeTask", xReturn, TOO_LARGE_STACK_DEPTH);
//    if(xReturn == pdPASS)
//        SYSTEM_I_PRINT("[OK] TooLargeTask created (UNEXPECTED!)\r\n");
//    else
//        //SYSTEM_I_PRINT("[ERR] TooLargeTask creation failed (EXPECTED, Stack: %d Words)\r\n", TOO_LARGE_STACK_DEPTH);
//        SYSTEM_I_PRINT("[ERR] TooLargeTask creation failed (EXPECTED, Need: %dByte, Free: %dByte)\r\n",
//               TOO_LARGE_STACK_DEPTH * sizeof(StackType_t),
//               (uint32_t)ulFreeHeapBefore);
    SYSTEM_I_PRINT("=== Task Creation Test Completed ===\r\n");
    vPrintHeapState("After Creation");

    // 创建器任务完成使命，自行删除
    vTaskDelete(NULL);
}

// -------------------------- 测试启动入口（main中调用） --------------------------测试栈溢出现象
void vStartStackOverflowTest(void)
{
    // 创建任务创建器（自身栈1024字，足够）
    xTaskCreate(vTaskCreator, "TaskCreator", 1024, 
                NULL, tskIDLE_PRIORITY + 5, NULL);
	vTaskStartScheduler(); 
	while(1); 
}










