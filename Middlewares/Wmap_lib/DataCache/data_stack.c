

#include "./DataCache/data_stack.h"
//#include "./usart/bsp_usart.h"

#include "./sys/sysio.h"



#if 1
/* 1. 定义栈底/栈大小（适配MDK/GCC，从启动文件映射） */
#if defined (__CC_ARM)  // MDK/ARMCC环境
extern uint32_t __initial_sp;  // 栈底地址（高地址，启动文件中定义）
#define STACK_BASE    ((uint32_t)&__initial_sp)
#define STACK_SIZE    0x0600    // H743建议设为8KB（0x2000），需和启动文件一致
#elif defined (__GNUC__) // GCC/CubeIDE环境
extern uint32_t _estack;       // 栈底地址（高地址）
extern uint32_t _Stack_Size;   // 栈大小（链接脚本中定义）
#define STACK_BASE    ((uint32_t)&_estack)
#define STACK_SIZE    ((uint32_t)&_Stack_Size)
#endif

/* 2. 栈填充标记 + 峰值记录 */
#define STACK_MARKER  0xAA
static uint32_t stack_peak_used = 0;  // 栈使用峰值

/* 3. 初始化栈标记（main开头调用） */
//void stack_init_marker(void)
//{
//    uint32_t *stack_ptr = (uint32_t*)(STACK_BASE - STACK_SIZE);
//    // 填充栈为标记值0xAA
//    while(stack_ptr < (uint32_t*)STACK_BASE)
//    {
//        *stack_ptr++ = STACK_MARKER;
//    }
//    stack_peak_used = STACK_BASE;
//}

/* 4. 修正版：获取MSP（兼容MDK/GCC） */
static uint32_t get_msp(void)
{
    uint32_t msp;
#if defined (__CC_ARM)  // MDK/ARMCC
    __asm
    {
        MRS msp, MSP
    }
#elif defined (__GNUC__) // GCC
    __asm volatile ("MRS %0, msp" : "=r" (msp));
#endif
    return msp;
}

/* 4. 修正版：栈初始化（仅填充未使用区域，不破坏上下文） */
void stack_init_marker(void)
{
    uint32_t current_msp = get_msp();
    uint32_t stack_top = STACK_BASE - STACK_SIZE;
    
    // 仅填充未使用的栈区域
    uint32_t *stack_ptr = (uint32_t*)stack_top;
    while(stack_ptr < (uint32_t*)current_msp && stack_ptr < (uint32_t*)STACK_BASE)
    {
        *stack_ptr++ = STACK_MARKER;
    }
    
    stack_peak_used = current_msp;
}




/* 5. 检查栈使用情况（主循环定期调用） */
void stack_check_usage(void)
{
    // 更新栈峰值
    uint32_t current_sp = get_msp();
    if(current_sp < stack_peak_used)
    {
        stack_peak_used = current_sp;
    }

    // 计算已使用/剩余栈大小
    uint32_t *stack_ptr = (uint32_t*)(STACK_BASE - STACK_SIZE);
    while(stack_ptr < (uint32_t*)STACK_BASE && *stack_ptr == STACK_MARKER)
    {
        stack_ptr++;
    }

    uint32_t used_bytes = STACK_BASE - (uint32_t)stack_ptr;
    uint32_t free_bytes = STACK_SIZE - used_bytes;
    uint32_t peak_used_bytes = STACK_BASE - stack_peak_used;

    // 打印栈信息
//    SYSTEM_INFO("=== H743 Stack Status ===\n");
//    SYSTEM_INFO("Total: %d KB\n", STACK_SIZE / 1024);
//    SYSTEM_INFO("Used:  %d KB (Current)\n", used_bytes / 1024);
//    SYSTEM_INFO("Free:  %d KB\n", free_bytes / 1024);
//    SYSTEM_INFO("Peak:  %d KB (Max Used)\n", peak_used_bytes / 1024);
//    SYSTEM_INFO("=========================\n");
		
		SYSTEM_INFO("=== H743 Stack Status ===\n");
    SYSTEM_INFO("Total: %d B\n", STACK_SIZE );
    SYSTEM_INFO("Used:  %d B (Current)\n", used_bytes );
    SYSTEM_INFO("Free:  %d B\n", free_bytes );
    SYSTEM_INFO("Peak:  %d B (Max Used)\n", peak_used_bytes );
    SYSTEM_INFO("=========================\n");
}
#endif
/**************************END OF FILE************************************/










