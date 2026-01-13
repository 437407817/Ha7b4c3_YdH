
#include "./test/bsp_qspi_flash_test.h"

#include "./DataConvert/data_compare.h"

#include "./user_test_config.h"

#if TEST_QSPI_FLASH
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize     (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

//__ALIGN_BEGIN unsigned char Tx_Buffer123[] __ALIGN_END =
//{
//	  0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46,
//};
uint8_t Tx_Buffer[] = "好stm32开发板\r\nhttp://firestm32.taobao.c感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

	//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;
	
	
void Flash_Test(void)
{
  uint32_t addr = FLASH_WriteAddress ;
  __IO uint8_t* qspi_addr = (__IO uint8_t*)(0x90000000);
	int state = QSPI_ERROR;

  
	printf("\r\n这是一个32M串行flash(W25Q256)内存映射模式读写实验(QSPI驱动) \r\n");
	
	/* 32M串行flash W25Q256初始化 */
	QSPI_FLASH_Init();
	if (1)
	{	
		printf("\r\n检测到QSPI FLASH W25Q256 !\r\n");
		printf("\r\n正在芯片擦除的%d~%d的内容!\r\n", addr, addr+W25Q256JV_PAGE_SIZE);   

    state = BSP_QSPI_Erase_Block(addr);
    if(state == QSPI_OK)
      printf("\r\n擦除成功!\r\n");
    else
    {

      printf("\r\n擦除失败!\r\n");
      while(1);
      
    }
    printf("\r\n正在向芯片%d地址写入数据，大小为%d!\r\n", addr, BufferSize);
		/* 将发送缓冲区的数据写到flash中 */
		BSP_QSPI_Write(Tx_Buffer, addr, BufferSize);
    printf("\r\n写入成功!\r\n");  
		
    /* QSPI memory reset */
    if (QSPI_ResetMemory() != QSPI_OK)
    {
      return ;
    }    
    if( QSPI_EnableMemoryMappedMode() != QSPI_OK )
    {
      return ;
    }
    printf("\r\n---使用memcpy函数读取QPSI的内容----\n\r");
    memcpy(Rx_Buffer,(uint8_t *)qspi_addr,BufferSize);  
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( 0 == TransferStatus1 )
		{    

			printf("\r\n读写(memcpy)测试成功!\n\r");
		}
		else
		{        

			printf("\r\n读写(memcpy)测试失败!");
		}
    printf("\r\n-------使用指针读取QPSI的内容-------");
		
    memset(Rx_Buffer,0,BufferSize);
		
    for(int i = 0; i < BufferSize; i++)
    { 
      Rx_Buffer[i] = *qspi_addr;
      qspi_addr++;
    }
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( 0 == TransferStatus1 )
		{    

			printf("\r\n读写(指针操作)测试成功!\n\r");
		}
		else
		{        

			printf("\r\n读写(指针操作)测试失败!\n\r");
		}    
    
	}

	
//	while(1);  
}


#endif
















/*********************************************END OF FILE**********************/
