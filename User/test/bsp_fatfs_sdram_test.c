


#include "./test/bsp_fatfs_sdram_test.h"

#include "./DataConvert/data_compare.h"



#include "ff_gen_drv.h"
#include "sdram_diskio.h"





FATFS RAMDISKFatFs;  /* File system object for RAM disk logical drive */
FIL MyFile;          /* File object */
char RAMDISKPath[4]; /* RAM disk logical drive path */
static uint8_t buffer[FF_MAX_SS]; /* a work buffer for the f_mkfs() */


void FATFS_SDRAM_Test(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */                              
  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */ 
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */

  

  /*##-1- Link the RAM disk I/O driver #######################################*/
  if(FATFS_LinkDriver(&SDRAMDISK_Driver, RAMDISKPath) == 0)
  {
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&RAMDISKFatFs, (TCHAR const*)RAMDISKPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      /*##-3- Create a FAT file system (format) on the logical drive #########*/
      if(f_mkfs((TCHAR const*)RAMDISKPath, NULL,  buffer, sizeof(buffer)) != FR_OK) 
      {
        /* FatFs Format Error */
        Error_Handler();
      }
      else
      {      
        /*##-4- Create and Open a new text file object with write access #####*/
      if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
      {
        /* 'STM32.TXT' file Open for write Error */
        Error_Handler();
      }
      else
      {
        /*##-5- Write data to the text file ################################*/
        res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
        
        if((byteswritten == 0) || (res != FR_OK))
        {
          /* 'STM32.TXT' file Write or EOF Error */
          Error_Handler();
        }
        else
        {
            /*##-6- Close the open text file #################################*/
          f_close(&MyFile);
          
            /*##-7- Open the text file object with read access ###############*/
          if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
          {
            /* 'STM32.TXT' file Open for read Error */
            Error_Handler();
          }
          else
          {
              /*##-8- Read data from the text file ###########################*/
            res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
            
              if((bytesread == 0) || (res != FR_OK))
            {
              /* 'STM32.TXT' file Read or EOF Error */
              Error_Handler();
            }
            else
            {
                /*##-9- Close the open text file #############################*/
              f_close(&MyFile);
              
                /*##-10- Compare read data with the expected data ############*/
                if ((bytesread != byteswritten))
                {                
                  /* Read data is different from the expected data */
                  Error_Handler(); 
                }
                else
                {
                  /* Success of the demo: no error occurrence */
//                  BSP_LED_On(LED3);
                }
              }
            }
          }
        }
      }
    }
  }
  
  /*##-11- Unlink the RAM disk I/O driver ####################################*/
  FATFS_UnLinkDriver(RAMDISKPath);
  
  /* Infinite loop */
//  while (1)
//  {
//  }
}




























