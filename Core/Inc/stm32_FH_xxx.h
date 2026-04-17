#ifndef __STM32_FH_XXX_H
#define __STM32_FH_XXX_H	


#include "./stm32_FH_xxx_hal.h"

#include "./user_config.h"
//#include "./user_test_config.h"//不要在总文件定义单个文件

/**
  * @brief STM32H7XX NUCLEO BSP Driver version number
  */
#define STM32HFXXX_NUCLEO_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define STM32HFXXX_NUCLEO_BSP_VERSION_SUB1   (0x03U) /*!< [23:16] sub1 version */
#define STM32HFXXX_NUCLEO_BSP_VERSION_SUB2   (0x03U) /*!< [15:8]  sub2 version */
#define STM32HFXXX_NUCLEO_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define STM32HFXXX_NUCLEO_BSP_VERSION        ((STM32HFXXX_NUCLEO_BSP_VERSION_MAIN << 24)\
                                            |(STM32HFXXX_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                            |(STM32HFXXX_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                            |(STM32HFXXX_NUCLEO_BSP_VERSION_RC))
																						
int32_t BSP_GetVersion(void);

//#include "./usart/bsp_usart.h"

//#include "./sys/bsp_systime.h" 

//#include "./GV_variable.h" 

//#include "shell.h"
//#include <elog.h>

//#include "./sdram/bsp_sdram.h" 





#endif

