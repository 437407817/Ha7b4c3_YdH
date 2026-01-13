#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./spi/bsp_spi.h"

/*SPI接口定义-结尾****************************/



#define SPI_FLASH_CS_LOW   SPI_a_FLASH_CS_LOW
#define SPI_FLASH_CS_HIGH  SPI_a_FLASH_CS_HIGH





/* Private typedef -----------------------------------------------------------*/
//#define  sFLASH_ID                       0xEF3015     //W25X16
//#define  sFLASH_ID                       0xEF4015	    //W25Q16
//#define  sFLASH_ID                        0XEF4017     //W25Q64
//#define  sFLASH_ID                       0XEF4018     //W25Q128
#define  sFLASH_ID                       0XEF4019     //W25Q256

#define W25Q256_SECTOR_SIZE    4096 //(4KB)
#define W25Q256_TOTAL_SECTORS  (32 * 1024 * 1024 / W25Q256_SECTOR_SIZE)
// 获取擦除块大小，W25Q256的擦除块为64KB（即16个扇区）
#define W25Q256_BLOCK_SIZE   16  //(64*1024/W25Q256_SECTOR_SIZE)


#define W25Q_SECTOR_SIZE			W25Q256_SECTOR_SIZE
#define W25Q_SECTOR_COUNT			W25Q256_TOTAL_SECTORS
#define W25Q_BLOCK_SIZE				W25Q256_BLOCK_SIZE



//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

#define SPI_FLASH_Secsize   4096


/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 //写使能命令
#define W25X_WriteDisable		      0x04 //写屏蔽命令
#define W25X_ReadStatusReg		    0x05 //读状态寄存器1命令

#define W25X_WriteStatusReg		  	0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 //页写入命令令（1页256字节）
#define W25X_PageProgram_4byte   	0x12    //4字节模式页写入命令（1页256字节）
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 //扇区擦除命令（一次擦除一个扇区/4KB）

#define W25X_SectorErase_4byte   	0x21    //4字节模式扇区擦除命令（一次擦除一个扇区/4KB）
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	  	0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 

#define W25X_JedecDeviceID		    0x9F //读取设备ID命令
#define W25X_Enter4ByteMode		    0xB7	//进入4字节地址模式命令
#define W25X_ReadStatusRegister3      0x15	//读状态寄存器3命令

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/





// 定义W25Q256的读取命令
#define W25Qxxx_READ_DATA 0x03
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


#define FLASH_SPI_ERROR(fmt,arg...)          SYSTEM_DEBUG(fmt,##arg)


uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);








void SPI_FLASH_busy_block(void);
void SPI_FLASH_WaitForWriteEnd(void);




void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_Mode_Init(void);

void SPI_FLASH_SendByte(uint8_t byte);
uint8_t SPI_FLASH_ReadByte(void);

uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);


void SPI_FLASH_Init(void);


#endif /* __SPI_FLASH_H */

