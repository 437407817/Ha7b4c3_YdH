


#include "./test/bsp_sdram_test.h"
//#include "./usart/bsp_usart.h"

#include "./DataConvert/data_compare.h"

#include "./sdram/bsp_sdram.h" 





#if TEST_SDRAM
/**
  * @brief  测试SDRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SDRAM_Basic_Test(void)
{
  /*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
	
	/* 32位的数据 */
  uint32_t uhWritedata_32b = 0, uhReaddata_32b = 0; 
  
  SDRAM_INFO("SDRAM  reading writting sdram...");


  /*按8位格式读写数据，并校验*/
  
  /* 把SDRAM数据全部重置为0 ，SDRAM_SIZE是以8位为单位的 */
  for (counter = 0x00; counter < SDRAM_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SDRAM写入数据  8位 */
  for (counter = 0; counter < SDRAM_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  SDRAM_INFO("SDRAM  01...");
  /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<SDRAM_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //从该地址读出数据
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("8 wrong :%d",counter);
      return 0;
    }
  }
	SDRAM_INFO("SDRAM  02...");
  
  /*按16位格式读写数据，并检测*/
  
  /* 把SDRAM数据全部重置为0 */
  for (counter = 0x00; counter < SDRAM_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SDRAM写入数据  16位 */
  for (counter = 0; counter < SDRAM_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  SDRAM_INFO("SDRAM  03...");
    /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<SDRAM_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);  //从该地址读出数据
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("16 wrong :%d",counter);

      return 0;
    }
  }

	SDRAM_INFO("SDRAM  04...");
	 /*按32位格式读写数据，并检测*/
  
  /* 把SDRAM数据全部重置为0 */
  for (counter = 0x00; counter < SDRAM_SIZE/4; counter++)
  {
    *(__IO uint32_t*) (SDRAM_BANK_ADDR + 4*counter) = (uint32_t)0x00;
  }
  
  /* 向整个SDRAM写入数据  32位 */
  for (counter = 0; counter < SDRAM_SIZE/4; counter++)
  {
    *(__IO uint32_t*) (SDRAM_BANK_ADDR + 4*counter) = (uint32_t)(uhWritedata_32b + counter);
  }
  SDRAM_INFO("SDRAM  05...");
    /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<SDRAM_SIZE/4;counter++ )
  {
    uhReaddata_32b = *(__IO uint32_t*)(SDRAM_BANK_ADDR + 4*counter);  //从该地址读出数据
    
    if(uhReaddata_32b != (uint32_t)(uhWritedata_32b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("32 wrong :%d",counter);

      return 0;
    }
  }	
	
  SDRAM_INFO("SDRAM read write success!"); 
  /*检测正常，return 1 */
  return 1;
  

}

#endif










#if 0


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
void SDRAM_Check(void);
uint8_t SDRAM_RNG_generate(void);


//#define BufferSize 1000
#define BufferSize 10000

uint32_t RadomBuffer[BufferSize];

uint32_t ReadBuffer[BufferSize];

#define SDRAM_SIZE (IS42S16400J_SIZE/4)

uint32_t *pSDRAM;

long long count=0,sdram_count=0;

RNG_HandleTypeDef hrngp;


extern DMA_HandleTypeDef dmaHandle;

uint8_t SDRAM_Test(void){

   uint8_t sdramstatus = STATE_SDRAM_ERROR;
	#if 0
	SDRAM_RNG_generate();
	
	sdramstatus = BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR,RadomBuffer,BufferSize);
	
	sdramstatus = BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR,ReadBuffer,BufferSize);
	
	
	sdramstatus=Buffercmp32(RadomBuffer,ReadBuffer,BufferSize);
	
	
		if(sdramstatus == 0)
	{

		printf("SDRAM test was successful\r\n");
	}
	else
	{

		printf("SDRAM test failed\r\n");
	}   
	#endif
	
	
	SDRAM_RNG_generate();
	
//		sdramstatus=HAL_DMA_Start_IT(&dmaHandle, (uint32_t)RadomBuffer, SDRAM_DEVICE_ADDR, 100);
	sdramstatus = BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR,RadomBuffer,BufferSize);//SUCCESS
			if(sdramstatus == 0)
	{

		printf("HAL_DMA_Start_IT  test was successful\r\n");
	}
	else
	{

		printf("HAL_DMA_Start_IT  test failed\r\n");
	}  
//	SDRAM_RNG_generate();
	

//	sdramstatus = BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR,RadomBuffer,BufferSize);

	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)RadomBuffer,BufferSize,"");
	
	
			if(sdramstatus == 0)
	{

		printf("BSP_SDRAM_WriteData_DMA was successful\r\n");
	}
	else
	{

		printf("BSP_SDRAM_WriteData_DMA failed\r\n");
	}   
	sdramstatus = BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR,ReadBuffer,BufferSize);
//	HAL_DMA_Start_IT(&dmaHandle, (uint32_t)SDRAM_DEVICE_ADDR, (uint32_t)&ReadBuffer, 100);
	
//	sdramstatus = BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR,ReadBuffer,BufferSize);
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)ReadBuffer,BufferSize,"");
	
				if(sdramstatus == 0)
	{

		printf("BSP_SDRAM_WriteData_DMA was successful\r\n");
	}
	else
	{

		printf("BSP_SDRAM_WriteData_DMA failed\r\n");
	} 

	
	sdramstatus=Buffercmp32(RadomBuffer,ReadBuffer,BufferSize);
	
	
		if(sdramstatus == 0)
	{

		printf("SDRAM DMA test was successful\r\n");
	}
	else
	{

		printf("SDRAM DMA test failed\r\n");
	}   
 return 0;

}




uint8_t SDRAM_RNG_generate(void)
{

	
	/*使能RNG时钟*/
    __RNG_CLK_ENABLE();
	/*初始化RNG模块产生随机数*/
    hrngp.Instance = RNG;
    HAL_RNG_Init(&hrngp);

    printf("Start generating 10,000 random numbers for testing\r\n");   
    for(count=0;count<BufferSize;count++)

    {
        RadomBuffer[count]=HAL_RNG_GetRandomNumber(&hrngp);

    }    
    printf("10,000 random numbers for testing have been generated.\r\n");
		
		
//		SDRAM_Check();
		
		return 0;
		
}




void SDRAM_Check(void)
{
  pSDRAM=(uint32_t*)SDRAM_DEVICE_ADDR;
	count=0;
	printf("Start writing to SDRAM\r\n");
	for(sdram_count=0;sdram_count<SDRAM_SIZE;sdram_count++)
	{
		*pSDRAM=RadomBuffer[count];
		count++;
		pSDRAM++;
		if(count>=10000)

		{
			count=0;
		}
	}
	printf("Total number of bytes written:%d\r\n",(uint32_t)pSDRAM-SDRAM_DEVICE_ADDR);

	count=0;
	pSDRAM=(uint32_t*)SDRAM_DEVICE_ADDR;
	printf("Start reading SDRAM and compare it with the original random number.\r\n");
	sdram_count=0;
	for(;sdram_count<SDRAM_SIZE;sdram_count++)
	{
		if(*pSDRAM != RadomBuffer[count])
		{
			printf("Data comparison error - Exit~\r\n");
			break;
		}
		count++;
		pSDRAM++;
		if(count>=10000)
		{
			count=0;
		}
	}

	printf("Compare by total number of bytes:%d\r\n",(uint32_t)pSDRAM-SDRAM_DEVICE_ADDR);

	if(sdram_count == SDRAM_SIZE)
	{

		printf("SDRAM test was successful\r\n");
	}
	else
	{

		printf("SDRAM test failed\r\n");
	}   
}



#if 1
/**
  * @brief  测试SDRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SDRAM_Test2(void)
{
  /*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SDRAM_INFO("Detecting SDRAM, reading and writing SDRAM in 8-bit and 16-bit modes....");


  /*按8位格式读写数据，并校验*/
  
  /* 把SDRAM数据全部重置为0 ，IS42S16400J_SIZE是以8位为单位的 */
  for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_DEVICE_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SDRAM写入数据  8位 */
  for (counter = 0; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_DEVICE_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<IS42S16400J_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_DEVICE_ADDR + counter);  //从该地址读出数据
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("8-bit data read - write error！");
      return 0;
    }
  }
	
  #if 0
  /*按16位格式读写数据，并检测*/
  
  /* 把SDRAM数据全部重置为0 */
  for (counter = 0x00; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_DEVICE_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SDRAM写入数据  16位 */
  for (counter = 0; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_DEVICE_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<IS42S16400J_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_DEVICE_ADDR + 2*counter);  //从该地址读出数据
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("16 - bit data read - write error！");

      return 0;
    }
  }

  
#endif
  SDRAM_INFO("The read - write test of SDRAM is normal.！"); 
  /*检测正常，return 1 */
  return 1;
  

}

#endif


#endif


























