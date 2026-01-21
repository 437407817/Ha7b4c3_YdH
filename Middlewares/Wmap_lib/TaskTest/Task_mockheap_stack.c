

#include "./TaskTest/Task_mockheap_stack.h"
//#include "./usart/bsp_usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "./hook/hook_mem.h"
#include "./sys/sysio.h"

// -------------------------- 配置参数（根据STM32H743硬件调整） --------------------------
#define TEST_STACK_DEPTH    8192    // 测试任务栈深度（单位：字，8192字=32KB）
#define HEAP_ALLOC_BLOCK    4096    // 每次堆申请的块大小（字节）
#define MAX_HEAP_ALLOC_CNT  200     // 最大堆申请次数（总申请≈200*4KB=800KB）

// 全局变量：保存堆申请的指针（避免被优化）
void *heap_ptr_array[MAX_HEAP_ALLOC_CNT] = {NULL};
// 全局变量：递归计数（替代原嵌套函数内的static变量）
static uint32_t recursion_cnt = 0;

/**
 * @brief  栈递归函数（移到外部，避免嵌套定义）
 * @note   逐步消耗栈空间，模拟栈深度耗尽
 */
void stack_recursion(void)
{
    uint8_t stack_buf[1024] = {0};  // 每次递归占用1KB栈
    recursion_cnt++;
SYSTEM_INFO("Stack recursion count: %d, Current minimum remaining stack space: %dByte\r\n",
       recursion_cnt,
       (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
    
    // 递归终止条件：栈剩余空间<2KB
    if(uxTaskGetStackHighWaterMark(NULL)*sizeof(StackType_t) > 2048)
    {
        memset(stack_buf, 0xFF, sizeof(stack_buf));  // 写入数据，避免优化
        stack_recursion();  // 递归调用
    }
}

/**
 * @brief  打印FreeRTOS堆内存状态（需开启configUSE_MALLOC_FAILED_HOOK）
 */
void PrintHeapState(void)
{
    size_t free_heap = xPortGetFreeHeapSize();    // 获取剩余堆大小
    size_t min_free_heap = xPortGetMinimumEverFreeHeapSize();  // 历史最小剩余堆
    
SYSTEM_INFO("Heap status: Free=%dByte, Min free=%dByte\r\n",
       (uint32_t)free_heap, (uint32_t)min_free_heap);
}

/**
 * @brief  超大栈占用测试任务
 * @note   通过局部数组+栈操作主动占用栈空间，测试栈极限
 */
void BigStackTestTask(void *pvParameters)
{
    // 方式1：定义超大局部数组（直接占用栈空间）
    // 注意：数组大小不能超过任务栈深度，否则直接栈溢出
    uint8_t big_stack_buf[28*1024] = {0};  // 28KB局部数组，占用栈空间
    
    while(1)
    {
        // 初始化局部数组（避免编译器优化）
        memset(big_stack_buf, 0xAA, sizeof(big_stack_buf));
// 更简洁的版本（嵌入式日志常用）
SYSTEM_INFO("Large stack task: Local array usage %dByte, Stack min free space: %dByte\r\n",
       sizeof(big_stack_buf),
       (uint32_t)uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
        
        // 执行栈递归测试（重置递归计数）
        recursion_cnt = 0;
        stack_recursion();
        
        vTaskDelay(pdMS_TO_TICKS(5000));  // 5秒循环一次
    }
}

/**
 * @brief  超大堆占用测试任务
 * @note   循环申请堆内存，直到堆耗尽，测试堆分配失败处理
 */
void BigHeapTestTask(void *pvParameters)
{
    uint32_t alloc_cnt = 0;
    
    while(1)
    {
        // 循环申请堆内存
        for(alloc_cnt = 0; alloc_cnt < MAX_HEAP_ALLOC_CNT; alloc_cnt++)
        {
            if(heap_ptr_array[alloc_cnt] == NULL)
            {
                heap_ptr_array[alloc_cnt] = pvPortMalloc(HEAP_ALLOC_BLOCK);
                if(heap_ptr_array[alloc_cnt] != NULL)
                {
                    // 写入数据，避免内存被优化
                    memset(heap_ptr_array[alloc_cnt], 0x55, HEAP_ALLOC_BLOCK);
SYSTEM_INFO("Heap alloc success: %d, size %dByte, total %dByte\r\n",
       alloc_cnt+1, HEAP_ALLOC_BLOCK, (alloc_cnt+1)*HEAP_ALLOC_BLOCK);
                    PrintHeapState();
                }
                else
                {
                    SYSTEM_INFO("Heap alloc failed: %d, insufficient free heap!\r\n", alloc_cnt+1);
                    break;  // 堆耗尽，终止申请
                }
            }
        }
        
        // 释放所有申请的堆内存
        SYSTEM_INFO("Freeing all heap memory...\r\n");
        for(uint32_t i = 0; i < MAX_HEAP_ALLOC_CNT; i++)
        {
            if(heap_ptr_array[i] != NULL)
            {
                vPortFree(heap_ptr_array[i]);
                heap_ptr_array[i] = NULL;
            }
        }
        PrintHeapState();
        SYSTEM_INFO("All allocated heap memory has been freed\r\n");
        
        vTaskDelay(pdMS_TO_TICKS(10000));  // 10秒循环一次（申请+释放）
    }
}

/**
 * @brief  创建内存极限测试任务
 */
void CreateMemTestTasks(void)
{
	BaseType_t xReturn = pdPASS;
    // 创建超大栈测试任务（栈深度设为TEST_STACK_DEPTH，优先级低于系统任务）
   xReturn= xTaskCreate(BigStackTestTask, "BigStackTask", TEST_STACK_DEPTH, 
                NULL, tskIDLE_PRIORITY + 2, NULL);
	
    vPrintStack_TaskCreationResult("BigStackTask", xReturn, TEST_STACK_DEPTH);
    // 创建超大堆测试任务（栈深度用默认值即可）
   xReturn= xTaskCreate(BigHeapTestTask, "BigHeapTask", 1024,  // 堆测试任务本身栈不需要太大
                NULL, tskIDLE_PRIORITY + 2, NULL);
	vPrintStack_TaskCreationResult("BigHeapTask", xReturn, 1024);
}


/**************************END OF FILE************************************/











