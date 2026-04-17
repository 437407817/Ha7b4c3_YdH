

#include "./TaskTest/Task_StressTest.h"

#include "./sys/sysio.h"

 #include "./buffer/user_buffers.h"
#include "./usart/bsp_usart_shell.h"

#include "stm32_FH_xxx.h"

#include "./Sys/sys_dwt.h"



#define USE_TEST 1
#define USE_DTCM 1

 #if USE_DTCM
#define DTCM_CODE __attribute__((section(".dtcm_text")))
 #else
 #endif
// -------------------------- 测试案例 --------------------------
/**
 * @brief  测试案例1：简单整数运算
 * @retval 无
 */
 #if USE_TEST
 #if USE_DTCM
DTCM_CODE void Test_Int_Calc(void)
 #else
 __attribute__((section(".sram1_text"))) void Test_Int_Calc(void)
 #endif
  #else
 void Test_Int_Calc(void)
 #endif

{
    uint32_t a = 0, b = 123456, c = 789012;
    for(uint32_t i = 0; i < 10000; i++)
    {
        a = b * c + i;
        b = a / (i + 1);
    }
		
}

/**
 * @brief  测试案例2：浮点运算（STM32H7有FPU，速度快）
 * @retval 无
 */
 #if USE_TEST
 #if USE_DTCM
DTCM_CODE void Test_Float_Calc(void)
 #else
 __attribute__((section(".sram1_text"))) void Test_Float_Calc(void)
 #endif
#else
void Test_Float_Calc(void)
#endif
{
    float x = 1.234f, y = 5.678f, z = 0.0f;
    for(uint32_t i = 0; i < 10000; i++)
    {
        z = x * y + (float)i / 1000.0f;
        x = z * 3.1415926f;
    }
		
}

/**
 * @brief  测试案例3：数组拷贝
 * @retval 无
 */
 #if USE_TEST
 #if USE_DTCM
 DTCM_CODE void Test_Array_Copy(void)
 #else
 __attribute__((section(".sram1_text"))) void Test_Array_Copy(void)
 #endif
 #else
void Test_Array_Copy(void)
 #endif
{
    uint8_t src[1024] = {0};
    uint8_t dst[1024] = {0};
    // 初始化源数组
    for(uint32_t i = 0; i < 1024; i++)
    {
        src[i] = i;
    }
    // 拷贝数组（核心测试部分）
    for(uint32_t i = 0; i < 1024; i++)
    {
        dst[i] = src[i];
    }
		
	
}




//与ram相关，不同区域ram速度不同
void Test_All_StressTest(void){

//DWT_Init();

// Variable definition
uint32_t cycles;
uint32_t us;
uint32_t ns;

// 2. Test case 1: Simple integer calculation
printf("=== Test 1: Simple Integer Calculation ===\r\n");
DWT_Start_Time();          // Start timing
Test_Int_Calc();           // Execute test function
DWT_Stop_Time();           // Stop timing
DWT_Calc_Time(&cycles, &us, &ns);  // Calculate elapsed time
printf("Elapsed cycles: %d\r\n", cycles);
printf("Elapsed time:  %d %03d ns\r\n\r\n", us, ns);//100.501

// 3. Test case 2: Floating-point calculation
printf("=== Test 2: Floating-point Calculation ===\r\n");
DWT_Start_Time();
Test_Float_Calc();
DWT_Stop_Time();
DWT_Calc_Time(&cycles, &us, &ns);
printf("Elapsed cycles: %d\r\n", cycles);
printf("Elapsed time:  %d %03d ns\r\n\r\n", us, ns);

// 4. Test case 3: Array copy
printf("=== Test 3: Array Copy (1024 bytes) ===\r\n");
DWT_Start_Time();
Test_Array_Copy();
DWT_Stop_Time();
DWT_Calc_Time(&cycles, &us, &ns);
printf("Elapsed cycles: %d\r\n", cycles);
printf("Elapsed time:  %d %03d ns\r\n\r\n", us, ns);
	
	
	
	

}






//LR_IROM1 0x08000000 0x00100000  {    ; 加载域：FLASH (2MB)
//  ER_IROM1 0x08000000 0x00100000  {    ; 执行域：FLASH
//    *.o (RESET, +First)
//    *(InRoot$$Sections)
//    .ANY (+RO)
//  }

//  RW_DTCMRAM 0x20000000 0x00020000  {  ; 执行域：DTCM RAM (128KB)
//    Task_StressTest.o (.dtcm_text)              ; 自定义DTCM代码段（只读）

//  }

//  RW_SRAM1 0x24000000 0x00080000  {    ; 执行域：SRAM1 (512KB)
//    Task_StressTest.o (.sram1_text)             ; 自定义SRAM1代码段（只读）
//	   .ANY (+RW +ZI)                  ; 所有默认数据优先放SRAM1（512KB足够）
//  }

//;  RW_SRAM2 0x24080000 0x00080000  {    ; 执行域：SRAM2 (256KB)
//;    .ANY (+RW +ZI +RO)                     ; 其他数据默认放SRAM2
//;  }
//}
//LR_IROM2 0x08100000 0x00100000  {
//  ER_IROM2 0x08100000 0x00100000  {  ; load address = execution address
//   .ANY (+RO)
//  }
//}









/**************************END OF FILE************************************/











