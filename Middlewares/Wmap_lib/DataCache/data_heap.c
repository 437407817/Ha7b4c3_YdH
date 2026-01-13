

#include "./DataCache/data_heap.h"
//#include "./usart/bsp_usart.h"

#include "./sys/sysio.h"


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
void print_heap_status(void)
{
    uint32_t total = get_heap_total();
    uint32_t used = get_heap_used();
    uint32_t free = total - used;
//    SYSTEM_INFO("Heap Total: %d KB, Used: %d KB, Free: %d KB\n", total/1024, used/1024, free/1024);
	    SYSTEM_INFO("Heap Total: %d B, Used: %d B, Free: %d B\n", total, used, free);
}



//void touch_drv_while_test(void)
//{
//    static uint32_t print_cnt = 0;
//    TouchScan(&touchInfo2);

//    // 每100次扫描打印一次堆状态（约2秒打印一次）
//    if(++print_cnt >= 100)
//    {
//        print_heap_status();
//        print_cnt = 0;
//    }

//    if (touchInfo2.state == DOWN) 
//    {
//        SYSTEM_INFO("Touch at (%d, %d), size: %d\n", touchInfo2.point.x, touchInfo2.point.y, touchInfo2.point.size);
//    }
//}



/**************************END OF FILE************************************/











