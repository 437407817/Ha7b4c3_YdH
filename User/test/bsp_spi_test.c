


#include "./test/bsp_spi_test.h"
//#include "./usart/bsp_usart.h"

#include "./DataConvert/data_compare.h"




/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/* I2C1 init function */
RNG_HandleTypeDef hrng2;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "wrocome stm32 boardr\nhttp://fire-stm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;

uint8_t TransferStatus1;

void SPI_FLASH_Test(void)
{
	uint8_t i=0;
	DeviceID = SPI_FLASH_ReadDeviceID();
	    hrng2.Instance = RNG;
    HAL_RNG_Init(&hrng2);
	i=HAL_RNG_GetRandomNumber(&hrng2);
	HAL_Delay( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\nSPI FLASH W25Q256 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\nwrite data is \r\n%s", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);
				SYSTEM_DEBUG_ARRAY_MESSAGE(Rx_Buffer,BufferSize,"COMMON_BufferRead  FLASH_ReadAddress=%d",FLASH_ReadAddress);
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( 0 == TransferStatus1 )
		{    
			
			printf("\r\n16M串行flash(W25Q256) success \n\r");
		}
		else
		{        
			
			printf("\r\n16M串行flash(W25Q256)failed n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		
		printf("\r\ncan not find W25Q256 ID!\n\r");
	}
	
//	SPI_Flash_PowerDown(); 
}








































