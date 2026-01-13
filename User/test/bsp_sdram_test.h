#ifndef __BSP_SDRAM_TEST_H
#define	__BSP_SDRAM_TEST_H




#include "stm32_FH_xxx.h"
//#include "stdlib.h"	    
#include "./sys/sysio.h"
#include "./user_test_config.h"
#include "./sdram/bsp_sdram.h"  


#if TEST_SDRAM
uint8_t SDRAM_Basic_Test(void);
#endif 




uint8_t SDRAM_Test(void);
uint8_t SDRAM_Test2(void);
//uint8_t SDRAM_Test3(void);






    		
















#endif /* __BSP_IIC_H */


