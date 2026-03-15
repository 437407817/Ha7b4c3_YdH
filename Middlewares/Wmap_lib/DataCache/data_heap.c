

#include "./DataCache/data_heap.h"
//#include "./usart/bsp_usart.h"

#include "./sys/sysio.h"

#if 1
// 【MDK环境】获取堆剩余空间（需在启动文件中定义__heap_base/__heap_limit）
#if defined (__CC_ARM)
extern uint32_t __heap_base;    // 堆起始地址（启动文件.s中定义）
extern uint32_t __heap_limit;   // 堆结束地址（启动文件.s中定义）

uint32_t get_heap_used(void)
{
    uint8_t *p = (uint8_t*)&__heap_base;
    uint32_t used = 0;
    // 遍历堆区域，统计已分配的内存（简单版，仅作参考）
    while(p < (uint8_t*)&__heap_limit)
    {
        if(*p != 0) // 非0表示已被使用
        {
            used++;
        }
        p++;
    }
    return used;
}

uint32_t get_heap_total(void)
{
    return (uint32_t)&__heap_limit - (uint32_t)&__heap_base;
}
#endif

// 【STM32CubeIDE/GCC环境】获取堆剩余空间
//#if defined (__GNUC__)
//#include <malloc.h>
//uint32_t get_heap_used(void)
//{
//    struct mallinfo mi = mallinfo();
//    return mi.uordblks; // 已使用堆空间
//}

//uint32_t get_heap_total(void)
//{
//    return __heap_end - __heap_start; // 堆总大小
//}
//#endif

// 打印堆内存状态（建议在TouchScan中定期打印）
void PrintHeapUsageRate(void)
{
    uint32_t total = get_heap_total();
    uint32_t used = get_heap_used();
    uint32_t free = total - used;
//    SYSTEM_INFO("Heap Total: %d KB, Used: %d KB, Free: %d KB\n", total/1024, used/1024, free/1024);
	    printf("Heap Total: %d B, Used: %d B, Free: %d B\r\n", total, used, free);
}

#endif

#include <stdint.h>
#include <stdio.h>

#include <stdint.h>

//#include "mem_monitor.h"
#include <stdio.h>
#include <stdint.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


//extern void __heapstats(int (*)(const char *format, ...), void *stream);

//void ShowInternalStats(void) {
//    printf("\n--- C Library Internal Heap Info --- \n");
//    __heapstats((int (*)(const char *, ...))printf, NULL);
//}


///* 声明链接器符号 */
//extern uint32_t Image$$ARM_LIB_HEAP$$Base;
//extern uint32_t Image$$ARM_LIB_HEAP$$Length;

//static uint32_t s_max_heap_top = 0; // 记录历史最高水位

//void PrintHeapUsageRate(void) {
//    // 1. 修正：获取真实的 Base 和 Size
//    uint32_t heap_base  = (uint32_t)&Image$$ARM_LIB_HEAP$$Base;
//    uint32_t heap_total = (uint32_t)&Image$$ARM_LIB_HEAP$$Length; // 必须加 &

//    // 2. 探测当前地址
//    void *temp_ptr = malloc(1);
//    uint32_t current_ptr = (uint32_t)temp_ptr;
//    
//    // 3. 更新最高水位线 (真正反映堆危险程度的指标)
//    if (current_ptr > s_max_heap_top) {
//        s_max_heap_top = current_ptr;
//    }
//    free(temp_ptr);

//    // 4. 计算逻辑
//    uint32_t current_used = (current_ptr > heap_base) ? (current_ptr - heap_base) : 0;
//    uint32_t max_used     = (s_max_heap_top > heap_base) ? (s_max_heap_top - heap_base) : 0;
//    float usage_percent   = ((float)max_used / (float)heap_total) * 100.0f;

//    printf("\r\n--- STM32H7 Heap Monitor ---\r\n");
//    printf("Heap Range:  0x%08X - 0x%08X\r\n", heap_base, heap_base + heap_total);
//    printf("Total Size:  %u Bytes\r\n", heap_total);
//    printf("Current Used: %u Bytes (Instantly)\r\n", current_used);
//    printf("Peak Used:    %u Bytes (High Watermark)\r\n", max_used);
//    printf("Usage Rate:   %.2f%% (Based on Peak)\r\n", usage_percent);
//    printf("----------------------------\r\n");
//}




void test_heap_change(void)
{
    // 1. Initial state: Print the initial heap usage
    printf("===== Initial Heap Status =====\r\n");
    PrintHeapUsageRate();

    // 2. Allocate heap memory: Simulate memory allocation in business code
  volatile   uint8_t *buf1 = (uint8_t*)malloc(10); // Allocate 1KB heap memory
   volatile  uint8_t *buf2 = (uint8_t*)malloc(20); // Allocate 2KB heap memory
    printf("\n===== After Allocating 30B Heap Memory =====\r\n");
    PrintHeapUsageRate(); // Used memory should increase by about 3KB at this point

    // 3. Use the memory (avoid compiler optimizing out unused variables)
    if(buf1) memset(buf1, 0xAA, 10);
    if(buf2) memset(buf2, 0x55, 20);

    // 4. Free part of the heap memory: Simulate memory release in business code
    free(buf1);
    buf1 = NULL;
    printf("\n===== After Freeing 1KB Heap Memory =====\r\n");
    PrintHeapUsageRate(); // Used memory should decrease by about 1KB at this point

    // 5. Free the remaining heap memory
    free(buf2);
    buf2 = NULL;
    printf("\n===== After Freeing All Allocated Heap Memory =====\r\n");
    PrintHeapUsageRate(); // Used memory should return to near initial state
}


/**************************END OF FILE************************************/











