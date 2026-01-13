


#include "./test/bsp_sdio_test.h"
//#include "./usart/bsp_usart.h"

#include "./DataConvert/data_compare.h"
#include "./SD/bsp_sdio_sd.h"







/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
//static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
//static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/



 void WIFI_PDN_INIT(void)
{
	static uint8_t wifi_is_init=0;
	if(wifi_is_init==0){
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*使能引脚时钟*/	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/*选择要控制的GPIO引脚*/															   
	GPIO_InitStruct.Pin = GPIO_PIN_13;	
	/*设置引脚的输出类型为推挽输出*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;      
	/*设置引脚为上拉模式*/
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	/*设置引脚速率为高速 */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST; 
	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
	/*禁用WiFi模块*/
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);  
	}
}

void SDIO_SD_Test(void){

	
uint32_t timeout=	SD_DATATIMEOUT;
WIFI_PDN_INIT();


  uint8_t SD_state = MSD_OK;
  
  SD_state = BSP_SD_Init();
  
  if(SD_state != MSD_OK)
  {
    SYSTEM_INFO("SD Initialization : FAIL.\r\n");	
  }		
   else
   {
    SYSTEM_INFO("SD Initialization :  OK.\r\n");		

    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, NUM_OF_BLOCKS);
    /* Wait until SD cards are ready to use for new operation */
    while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
    {
			
			if(timeout--==0){
			break;
			}
    }
    if(SD_state != MSD_OK)
    {
       SYSTEM_INFO("SD ERASE : FAILED.\r\n");  
    }
    else
    {
       SYSTEM_INFO("SD ERASE : OK.\r\n"); 
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);

//      SD_state = BSP_SD_WriteBlocks_DMA(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
			      SD_state = BSP_SD_WriteBlocks(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS,1000);
//			SYSTEM_INFO("SD BSP_SD_WriteBlocks : OK.\r\n");
      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
      {
      }                               
      if(SD_state != MSD_OK)
      {
        SYSTEM_INFO("SD WRITE : FAILED.\r\n");
      }
      else
      {
        printf("SD WRITE : OK.\r\n");
				HAL_Delay(1000);
//        SD_state = BSP_SD_ReadBlocks_DMA(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        SD_state = BSP_SD_ReadBlocks(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS,1000);				
        /* Wait until SD cards are ready to use for new operation */
        while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
        {
        }
        if(SD_state != MSD_OK)
        {
          SYSTEM_INFO("SD READ  : FAILED.\r\n");
        }
        else
        {
          SYSTEM_INFO("SD READ  : OK.\r\n");
          if(Buffercmp32(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            SYSTEM_INFO("SD COMPARE  : FAILED.\r\n");
          }
          else
          {
            SYSTEM_INFO("SD COMPARE  : OK.\r\n");
          }
        }
      }
    }
  } 













}



#if 1

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"


void SDIO_SD_Test2(void)
{	
	
	
	char SDPath[4]; /* SD逻辑驱动器路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            			  /* 文件成功读写数量 */
BYTE ReadBuffer2[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"welcome STM32 F429 board\r\n";  
uint8_t workBuffer[FF_MAX_SS];
MKFS_PARM opt,*popt=0;
	/*禁用WiFi模块*/
	WIFI_PDN_INIT();

   //链接驱动器，创建盘符
//   FATFS_LinkDriver(&SD_Driver, SDPath);
   //在外部SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
   res_sd = f_mount(&fs,"2:",1);  


   /*----------------------- 格式化测试 ---------------------------*/  
   /* 如果没有文件系统就格式化创建创建文件系统 */
   if(res_sd == FR_NO_FILESYSTEM)
   {
      printf("sd card FR_NO_FILESYSTEM\r\n");
      /* 格式化 */
//      res_sd=f_mkfs("0:",0,0);	
		 opt.align=1;
		 opt.au_size=4096;
		 opt.fmt=FM_FAT32;
		 opt.n_fat=2;
		 opt.n_root=512;
		 
res_sd=f_mkfs("2:", &opt,  workBuffer, sizeof(workBuffer))	;	 
//f_mkfs(path, popt, Buff, sizeof Buff)
      if(res_sd == FR_OK)
      {
        printf("sd card seccess1\r\n");
        /* 格式化后，先取消挂载 */
        res_sd = f_mount(NULL,"2:",1);			
        /* 重新挂载	*/			
        res_sd = f_mount(&fs,"2:",1);
      }
      else
      {
        
        printf("sd card fail\r\n");
        while(1);
      }
   }
   else if(res_sd!=FR_OK)
   {
      printf("fail (%d)\r\n",res_sd);
     
   }
   else
   {
      printf("fail (%d)\r\n",res_sd);
   }	
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("\r\n****** enter ******\r\n");	
    res_sd = f_open(&fnew, "0:test.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
      printf("test.txt succ\r\n");
      /* 将指定存储区内容写入到文件内 */
      res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
      if(res_sd==FR_OK)
      {
        printf("write d %d\n",fnum);
        printf("wite d\r\n%s\r\n",WriteBuffer);
      }
      else
      {
        printf("fail:(%d)\n",res_sd);
      }    
      /* 不再读写，关闭文件 */
      f_close(&fnew);
    }
    else
    {	

      printf("fail\r\n");
    }
    
  /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** ttt ******\r\n");
    res_sd = f_open(&fnew, "2:test.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(res_sd == FR_OK)
    {
      
      printf("succ\r\n");
      res_sd = f_read(&fnew, ReadBuffer2, sizeof(ReadBuffer2), &fnum); 
      if(res_sd==FR_OK)
      {
        printf("success %d\r\n",fnum);
        printf("read data\r\n%s \r\n", ReadBuffer2);	
      }
      else
      {
        printf("read file :(%d)\n",res_sd);
      }		
    }
    else
    {
 
      printf("open fail\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);	
    
    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"2:",1);
    
    /* 操作完成，停机 */
 
   
}



#endif














