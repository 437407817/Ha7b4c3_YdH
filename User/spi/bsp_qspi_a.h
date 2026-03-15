#ifndef __BSP_QSPI_A_H
#define __BSP_QSPI_A_H

#include "stm32_FH_xxx.h"
#include <stdio.h>




/* 
    W25Q64JV有256块，每块有16个扇区，每个扇区Sector有16页，每页有128字节，共计16MB
*/
#if 0

/* QSPI引脚和时钟相关配置宏定义 */

#define QSPI_CS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()//注意修改启动的时钟源
#define QSPI_CLK_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()//注意修改启动的时钟源
#define QSPI_BK1_D0_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()//注意修改启动的时钟源
#define QSPI_BK1_D1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()//注意修改启动的时钟源
#define QSPI_BK1_D2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()//注意修改启动的时钟源
#define QSPI_BK1_D3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()//注意修改启动的时钟源

#define QSPI_FORCE_RESET()              __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()            __HAL_RCC_QSPI_RELEASE_RESET()

#define QSPI_CS_PIN                     GPIO_PIN_10 //QSPI_CS
#define QSPI_CS_GPIO_PORT               GPIOB
#define QSPI_CS_GPIO_AF                 GPIO_AF9_QUADSPI//有坑，自行比较CubeMX生成的正确指向

#define QSPI_CLK_PIN                    GPIO_PIN_2 //QSPI_CLK
#define QSPI_CLK_GPIO_PORT              GPIOB
#define QSPI_CLK_GPIO_AF                GPIO_AF9_QUADSPI//有坑，自行比较CubeMX生成的正确指向

#define QSPI_BK1_D0_PIN                 GPIO_PIN_11 //QSPI_BK1_D0
#define QSPI_BK1_D0_GPIO_PORT           GPIOD
#define QSPI_BK1_D0_GPIO_AF             GPIO_AF9_QUADSPI//有坑，自行比较CubeMX生成的正确指向

#define QSPI_BK1_D1_PIN                 GPIO_PIN_12 //QSPI_BK1_D1
#define QSPI_BK1_D1_GPIO_PORT           GPIOD
#define QSPI_BK1_D1_GPIO_AF             GPIO_AF9_QUADSPI//有坑，自行比较CubeMX生成的正确指向

#define QSPI_BK1_D2_PIN                 GPIO_PIN_2 //QSPI_BK1_D2
#define QSPI_BK1_D2_GPIO_PORT           GPIOE
#define QSPI_BK1_D2_GPIO_AF             GPIO_AF9_QUADSPI//有坑，自行比较CubeMX生成的正确指向

#define QSPI_BK1_D3_PIN                 GPIO_PIN_13 //QSPI_BK1_D3
#define QSPI_BK1_D3_GPIO_PORT           GPIOD
#define QSPI_BK1_D3_GPIO_AF             GPIO_AF9_QUADSPI//有坑，自行比较CubeMX生成的正确指向
                 



#define QSPI_FLASH_MEM_ADDR         			     0X90000000 //W25Qxx系列芯片的首地址！！


/* W25Q64JV基本信息 */
#define QSPI_FLASH_SIZE     						  23                       /* Flash大小，2^23 = 8MB,建议+1，在实际初始化时，需QSPI的内存映射加大一倍，*/
																																	 /*	防止被cache或者其他方式访问到最后一个字节,但下面的QSPI_FLASH_SIZES*/
																																	 /* 写实际大小防止写越界*/
#define QSPI_SECTOR_SIZE    						  (4 * 1024)               /* 扇区大小，4KB */
#define QSPI_PAGE_SIZE      						  128                      /* 页大小，128字节 */
#define QSPI_FLASH_SIZES                  8 * 1024 * 1024          /* Flash大小，16 * 1024 * 1024 = 16MB*/


/* W25Q64JV相关命令 */
#define CMD_WRITE_ENABLE	                      0x06    /* 写使能指令,对应手册上的指令为: Write Enable */
#define CMD_GET_REG1		                        0x05    /* 读取状态命令,对应手册上的指令为: Read Status Register-1 */
#define CMD_WRITE                               0x32    /* 对应手册上的指令是: Quad Input Page Program   */
#define CMD_READ                                0xEB    /* 对应手册上的指令是: Fast Read Quad I/O  */
#define CMD_ERASE_BLOCK_64K                     0xD8    /* 清除扇区，64K扇区,对应手册上的指令为: Block Erase (64KB) */
#define CMD_ERASE_CHIP                          0xC7    /* 清除整块芯片，耗时长,对应手册上的指令为: Chip Erase*/




#endif 



#endif /* __SPI_FLASH_H */

