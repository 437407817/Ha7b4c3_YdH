
  
#include "./Sys/sys_dwt.h"
#include "stm32_FH_xxx_hal.h"  // 根据实际使用的STM32芯片型号替换这里，比如stm32f10x.h等




// 全局变量：记录CYCCNT溢出次数（处理长延迟溢出）
static uint32_t DWT_Overflow_Count = 0;

// 初始化DWT，启用CYCCNT计数器（用于获取时钟周期数，方便做时间相关测量等操作）
void DWT_Init(void)
{
    // 1. 使能DWT模块
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // 2. 清零CYCCNT计数器
    DWT->CYCCNT = 0;
    // 3. 启动CYCCNT计数
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    

}





// H743专属：微秒级延迟（适配480MHz，处理溢出）
void DWT_Delay_us(uint32_t us) {
    if(us == 0) return;
    
    uint32_t target_cycles = SystemCoreClock / 1000000 * us; // 480MHz→480 cycles/μs
    uint32_t start_cycles = DWT->CYCCNT;
    uint32_t current_cycles;
    uint32_t overflow_count = DWT_Overflow_Count;
    
    // 循环等待，同时处理溢出
    do {
        current_cycles = DWT->CYCCNT;
        // 计算已过周期数（考虑溢出）
        uint32_t elapsed_cycles = (DWT_Overflow_Count - overflow_count) * (uint64_t)0xFFFFFFFF + (current_cycles - start_cycles);
        if(elapsed_cycles >= target_cycles) break;
    } while(1);
}

// H743专属：纳秒级延迟（最小2ns精度）
void DWT_Delay_ns(uint32_t ns) {
    if(ns == 0) return;
    
    // 480MHz = 480000000 Hz → 1周期 = 1/480000000 秒 = 2.083333ns
    uint32_t target_cycles = (ns * SystemCoreClock + 999999999) / 1000000000; // 四舍五入计算周期数
    uint32_t start_cycles = DWT->CYCCNT;
    
    while((DWT->CYCCNT - start_cycles) < target_cycles);
}


void DWT_Delay_ms(uint32_t ms) {
    if(ms == 0) return;
    // 1ms = 1000μs，直接调用μs延迟
    DWT_Delay_us(ms * 1000);
}






uint32_t dwt_getCurrentTick(void)
{
    return DWT->CYCCNT;
}

void dwt_clearCurrentTick(void)
{
     DWT->CYCCNT=0;
}









