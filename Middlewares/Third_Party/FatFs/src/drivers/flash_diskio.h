/**
  ******************************************************************************
  * @file    ppp_diskio.h
  * @author  MCD Application Team
  * @brief   Header for ppp_diskio.c module. the PPP diskio driver is not
             functional and is intended to show how to implement
	     a FatFS diskio driver.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_DISKIO_H
#define __FLASH_DISKIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32fxxx.h"
#include "./spi/bsp_spi.h"
#include "./spi/bsp_spi_flash.h"
#include "ff_gen_drv.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern const Diskio_drvTypeDef  FLASHDISK_Driver;

#endif /* __SD_DISKIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

