
  
#include "./Sys/sys_dwt.h"
#include "stm32_FH_xxx_hal.h"  // 根据实际使用的STM32芯片型号替换这里，比如stm32f10x.h等




// 全局变量：记录CYCCNT溢出次数（处理长延迟溢出）
//static uint32_t DWT_Overflow_Count = 0;

// 初始化DWT，启用CYCCNT计数器（用于获取时钟周期数，方便做时间相关测量等操作）
void DWT_Init(void)
{
    // 1. 使能 TRACE 单元
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    // 2. 解锁 DWT (H7 必须执行此步，否则计数器无法写入或启动)
    DWT->LAR = 0xC5ACCE55; 
    
    // 3. 清零并启动
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}





// H743专属：微秒级延迟（适配480MHz，处理溢出）
// 微秒级延迟（480MHz 专用，安全无溢出）
void DWT_Delay_us(uint32_t us)
{
    if(us == 0) return;

    uint64_t total_cycles = (uint64_t)us * (SystemCoreClock / 1000000);
    uint32_t start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < total_cycles);
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



///---------------------------------------


// 全局变量：DWT计时相关
volatile uint32_t DWT_Start_Cycle;  // 计时起始周期
volatile uint32_t DWT_End_Cycle;    // 计时结束周期


/**
 * @brief  开始计时（记录当前CYCCNT值）
 * @retval 无
 */
void DWT_Start_Time(void)
{
    // 先重置计数器（可选，也可直接记录当前值）
    DWT->CYCCNT = 0;
    DWT_Start_Cycle = DWT->CYCCNT;
}

/**
 * @brief  停止计时（记录当前CYCCNT值）
 * @retval 无
 */
void DWT_Stop_Time(void)
{
    DWT_End_Cycle = DWT->CYCCNT;
}

/**
 * @brief  计算耗时（纯整数运算，无float）
 * @param  cycles: 输出参数，消耗的CPU周期数
 * @param  us_int: 输出参数，耗时的整数微秒部分（比如100.501μs → 100）
 * @param  ns_remain: 输出参数，耗时的剩余纳秒部分（比如100.501μs → 501）
 * @retval 无
 */
void DWT_Calc_Time(uint32_t *cycles, uint32_t *us_int, uint32_t *ns_remain)
{
    // 1. 计算周期数（处理溢出，更健壮）
    if(DWT_End_Cycle >= DWT_Start_Cycle)
    {
        *cycles = DWT_End_Cycle - DWT_Start_Cycle;
    }
    else
    {
        // 处理32位计数器溢出（比如从0xFFFFFFFF到0）
        *cycles = (0xFFFFFFFF - DWT_Start_Cycle) + DWT_End_Cycle + 1;
    }
    
    // 2. STM32H743主频480MHz = 480000000 Hz
    const uint32_t SystemCoreClock = 480000000;
    
    // 3. 纯整数计算：总纳秒数（四舍五入）
    uint64_t total_ns = (*cycles * 1000000000ULL + SystemCoreClock / 2) / SystemCoreClock;
    
    // 4. 拆分：整数微秒 + 剩余纳秒（无浮点数，精度无损失）
    *us_int = total_ns / 1000;    // 1μs = 1000ns → 总纳秒数/1000=整数微秒
    *ns_remain = total_ns % 1000; // 余数=剩余纳秒（0~999）
}




























