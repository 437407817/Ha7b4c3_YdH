/**
  ******************************************************************************
  * @file    ppp_diskio.c
  * @author  MCD Application Team
  * @brief   PPP Disk I/O driver generic driver template
             this driver is not functional and is intended to show
	           how to implement a FatFs diskio driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "flash_diskio.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define FLASH_SECTOR_SIZE 	W25Q_SECTOR_SIZE  //一个扇区的大小			  
////前12M字节给fatfs用,12M字节后,用于存放字库,字库占用3.09M.	剩余部分,给客户自己用	 			    
//#define	FLASH_SECTOR_COUNT 3072;	//总扇区数：256*16个，FATFS占12M，即(12/16)*256*16=3072
//#define FLASH_BLOCK_SIZE   	16     	//每个BLOCK有16个扇区



/* Block Size in Bytes */
/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
DSTATUS FLASHDISK_initialize (BYTE);
DSTATUS FLASHDISK_status (BYTE);
DRESULT FLASHDISK_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT FLASHDISK_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT FLASHDISK_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  FLASHDISK_Driver =
{
  FLASHDISK_initialize,
  FLASHDISK_status,
  FLASHDISK_read,
#if  _USE_WRITE == 1
  FLASHDISK_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  FLASHDISK_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS FLASHDISK_initialize(BYTE lun)
{
  Stat = STA_NOINIT;

  /* Configure the uPPP device */
  if(Flash_FATFS_Init() == STATE_SPI_OK)
  {
		SPI_Flash_WAKEUP();
//		SYSTEM_DEBUG("Flash_FATFS_Init\r\n");
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS FLASHDISK_status(BYTE lun)
{
  Stat = STA_NOINIT;

  if(SPI_FLASH_ReadID() == sFLASH_ID)
  {
//		SYSTEM_DEBUG("FLASHDISK_status\r\n");
    Stat &= ~STA_NOINIT;
  }
//SYSTEM_DEBUG("SPI_FLASH_ReadID\r\n");
  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT FLASHDISK_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_OK;
	
//	SYSTEM_DEBUG("FLASHDISK_read\r\n");
//	      /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间 */
//      sector+=512;      
      SPI_FLASH_BufferRead(buff, sector <<12, count<<12);//左移 12 位后就是 4096，正好是 1 个扇区对应的字节数,4k
	
//SPI_FLASH_BufferRead(buff, sector*FLASH_SECTOR_SIZE, count*FLASH_SECTOR_SIZE);
	
//  if(MEDIA_Read((uint32_t*)buff,
//                       sector,
//                       count) != PPP_OK)
//  {
//    res = RES_ERROR;
//  }

  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT FLASHDISK_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
	 uint32_t write_addr; 
  DRESULT res = RES_OK;
	#if USE_P
	      /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间 */
			sector+=512;//2*512=2048=2MB
      write_addr = sector<<12;   
	#else
//	write_addr = sector*FLASH_SECTOR_SIZE;
	
	#endif
	write_addr = sector<<12;
//	SYSTEM_DEBUG("FLASHDISK_write\r\n");
	SPI_FLASH_SectorErase(write_addr);
SPI_FLASH_BufferWrite((BYTE*)buff,write_addr, count<<12);// count*FLASH_SECTOR_SIZE=count<<12,4096
//  if(MEDIA_Write((uint32_t*)buff,
//                       sector,
//                       count) != PPP_OK)
//  {
//    res = RES_ERROR;
//  }

  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT FLASHDISK_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
//  MEDIA_Info_t Info;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
//    MEDIA_GetInfo(&Info);
    *(DWORD*)buff = W25Q_SECTOR_COUNT;
    res = RES_OK;
//	SYSTEM_DEBUG("GET_SECTOR_COUNT\r\n");
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = W25Q_SECTOR_SIZE;
    res = RES_OK;
//	SYSTEM_DEBUG("GET_SECTOR_SIZE\r\n");
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = W25Q_BLOCK_SIZE;
//	SYSTEM_DEBUG("GET_BLOCK_SIZE\r\n");
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

