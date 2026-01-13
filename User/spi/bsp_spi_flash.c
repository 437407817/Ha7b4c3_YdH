 /**
  ******************************************************************************
  * @file    bsp_spi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./spi/bsp_spi_flash.h"
#include "./sys/sysio.h"





static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

 




 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  SPI_a_FLASH_CS_LOW();
  /* 发送扇区擦除指令*/
  SPI_FLASH_SendByte(W25X_SectorErase_4byte);
  /*发送擦除扇区地址的高8位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*发送擦除扇区地址的中前8位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 发送擦除扇区地址的中后8位 */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 发送擦除扇区地址的低8位 */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}


///**
// * @brief   对 FLASH 按页写入数据，调用本函数写入数据前需要先擦除扇区
// * @param   pBuffer：要写入数据的指针
// * @param   WriteAddr：写入数据地址
// * @param   NumByteToWrite：写入数据长度（以字节为单位）。必须小于等于SPI_FLASH_PerWritePageSize
// * @retval  无
// */
//void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
//{
//    // 使能写命令
//    SPI_FLASH_WriteEnable();
//    // 拉低片选线，开始通信
//    SPI_FLASH_CS_LOW();
//    // 发送页写入指令
//    SPI_FLASH_SendByte(W25Q256_PageProgram_4byte);
//    // 发送写入地址[24,31]bit
//    SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
//    // 发送写入地址[16,23]bit
//    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
//    // 发送写入地址[8,15]bit
//    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
//    // 发送写入地址[0,7]bit
//    SPI_FLASH_SendByte(WriteAddr & 0xFF);
//    for (int i = 0; i < NumByteToWrite; i++)
//    {
//        SPI_FLASH_SendByte(pBuffer[i]);
//    }
//    // 拉高片选线，结束通信
//    SPI_FLASH_CS_HIGH();
//    SPI_FLASH_busy_block();
//}
// 



/**
 * @brief   对 FLASH 按扇区写入数据
 * @param   pBuffer：要写入数据的指针
 * @param   WriteAddr：写入数据地址
 * @param   NumByteToWrite：写入数据长度（以字节为单位）。
 * @retval  无
 */
void SPI_FLASH_SecWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t page_num = 0;         // 当前要写入flash的地址属于第几扇区;
    uint32_t page_first_addr = 0;  // 当前要写入flash的地址所属扇区首地址
    uint32_t page_offset_addr = 0; // 要写入的地址相对于当前扇区首地址的偏移
	 uint8_t flash_sec_data[4096]; // flash一个扇区的数据
    uint8_t *page_data_point = flash_sec_data;
    // 确定要写入flash的地址所属页的首地址
    page_num = WriteAddr / 4096;
    page_first_addr = 4096 * page_num;
    page_offset_addr = WriteAddr - page_first_addr;
//    printf("page_num = %d\r\n",page_num);
//    printf("page_first_addr = %d\r\n",page_first_addr);
//    printf("page_offset_addr = %d\r\n",page_offset_addr);
 
    // 读出扇区数据存储到flash_sec_data数组中
    SPI_FLASH_BufferRead(page_data_point, page_first_addr, 4096);
    // 将要写入的数据赋值到page_data_point中的对应地址
    for (int i = page_offset_addr, j = 0; j < NumByteToWrite; i++, j++)
    {
        page_data_point[i] = pBuffer[j];
    }
    // 擦除要写入的扇区
    SPI_FLASH_SectorErase(page_first_addr);
    // 将整理好的数据重新写入到内部FLASH中
    for (int i = 0; i < 16; i++)
    {
        SPI_FLASH_PageWrite(page_data_point, page_first_addr, SPI_FLASH_PageSize);
        page_data_point += SPI_FLASH_PageSize;
        page_first_addr += SPI_FLASH_PageSize;
    }
}
 
/**
 * @brief   FALSH不定量数据写入函数，调用本函数写入数据前需要先擦除扇区
 * @param   pBuffer：要写入数据的指针
 * @param   WriteAddr：写入数据地址
 * @param   NumByteToWrite：写入数据长度。(以字节为单位)
 * @retval  无
 */
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    uint32_t NumOfSec = 0;       // 要写多少个扇区
    uint32_t NumOfSingle = 0;    // 剩下多少不满一个扇区的数据
    uint32_t Numofaddrtosec = 0; // 当前写入地址距离扇区结束地址还有多少个字节
    if (0 == WriteAddr % 4096)
    {
        Numofaddrtosec = 0;
    }
    else
    {
        Numofaddrtosec = 4096 - WriteAddr % 4096;
        if (Numofaddrtosec >= NumByteToWrite)
        {
            Numofaddrtosec = NumByteToWrite;
        }
    }
    NumOfSec = (NumByteToWrite - Numofaddrtosec) / 4096;
    NumOfSingle = (NumByteToWrite - Numofaddrtosec) % 4096;
//    printf("Numofaddrtosec = %d\r\n",Numofaddrtosec);
//    printf("NumOfSec = %d\r\n",NumOfSec);
//    printf("NumOfSingle = %d\r\n",NumOfSingle);
//    printf("unwritten_sector_num = %d\r\n",Numofaddrtosec);
    if (0 != Numofaddrtosec)
    {
        SPI_FLASH_SecWrite(pBuffer, WriteAddr, Numofaddrtosec);
        WriteAddr += Numofaddrtosec;
        pBuffer += Numofaddrtosec;
        if (0 != NumOfSec)
        {
            for (int i = 0; i < NumOfSec; i++)
            {
                SPI_FLASH_SecWrite(pBuffer, WriteAddr, SPI_FLASH_Secsize);
                WriteAddr += SPI_FLASH_Secsize;
                pBuffer += SPI_FLASH_Secsize;
            }
        }
        if (0 != NumOfSingle)
        {
            SPI_FLASH_SecWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else
    {
        if (0 != NumOfSec)
        {
            for (int i = 0; i < NumOfSec; i++)
            {
                SPI_FLASH_SecWrite(pBuffer, WriteAddr, SPI_FLASH_Secsize);
                WriteAddr += SPI_FLASH_Secsize;
                pBuffer += SPI_FLASH_Secsize;
            }
        }
        if (0 != NumOfSingle)
        {
            SPI_FLASH_SecWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    SPI_FLASH_busy_block();
}
 
 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 写页写指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);

  /*发送写地址的高8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  /*发送写地址的中前8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中后8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低8位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_SPI_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

#if 0
 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;	
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*先写满当前页*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
#endif
 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送 读 指令 */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* 发送 读 地址高8位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /* 发送 读 地址中前8位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 发送 读 地址中后8位 */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* 发送 读 地址低8位 */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
	/* 读取数据 */
  while (NumByteToRead--)
  {
    /* 读取一个字节*/
    *pBuffer = SPI_FLASH_ReadByte();
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
//	SPI_FLASH_busy_block();
}


 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_ReadByte();

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_ReadByte();

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_ReadByte();

  /* 停止通讯：CS高电平 */
  SPI_FLASH_CS_HIGH();

	/*把数据组合起来，作为函数的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}



/**
 * @brief   读取 FLASH Device ID函数
 * @param   无
 * @retval  FLASH ID
 */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
    uint32_t temp = 0;
    uint8_t temp0 = 0;
    uint8_t temp1 = 0;
    uint8_t temp2 = 0;
    // 拉低片选线，开始通信
    SPI_FLASH_CS_LOW();
    // 发送获取W25Q256芯片ID指令
    SPI_FLASH_SendByte(W25X_DeviceID);
    // 接收数据
    temp0 = SPI_FLASH_ReadByte();
    temp1 = SPI_FLASH_ReadByte();
    temp2 = SPI_FLASH_ReadByte();
    // 拉高片选线，结束通信
    SPI_FLASH_CS_HIGH();
    temp = temp0 << 16 | temp1 << 8 | temp2;
    return temp;
}

/**
 * @brief   使能写命令
 * @param   无
 * @param   无
 * @retval  无
 */
void SPI_FLASH_WriteEnable(void)
{
    // 拉低片选线，开始通信
    SPI_FLASH_CS_LOW();
    // 发送写使能指令
    SPI_FLASH_SendByte(W25X_WriteEnable);
    // 拉高片选线，结束通信
    SPI_FLASH_CS_HIGH();
//    SPI_FLASH_busy_block();
}


/**
 * @brief   等待写入、擦除等操作完成
 * @param   无
 * @param   无
 * @retval  无
 */
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;
    // 拉低片选线，开始通信
    SPI_FLASH_CS_LOW();
    // 发送写状态寄存器1指令
    SPI_FLASH_SendByte(W25X_ReadStatusReg);
    do
    {
        // 获取写状态寄存器1的值并做判断。0：空闲、1：忙碌
        FLASH_Status = SPI_FLASH_ReadByte();
    } while (SET == (FLASH_Status & WIP_Flag));
 
    // 拉高片选线，结束通信
    SPI_FLASH_CS_HIGH();
}


/**
 * @brief   忙碌阻塞函数
 * @param   none
 * @param   none
 * @param   none
 * @retval  none
 */
void SPI_FLASH_busy_block(void)
{
    uint8_t FLASH_Status = 0;
    while (1)
    {
        // 拉低片选线，开始通信
        SPI_FLASH_CS_LOW();
        // 发送读状态寄存器1指令
        SPI_FLASH_SendByte(W25X_ReadStatusReg);
        // 接收读取状态寄存器1寄存器内容
        FLASH_Status = SPI_FLASH_ReadByte();
        // 拉高片选线，结束通信
        SPI_FLASH_CS_HIGH();
        if (0 == (1 & FLASH_Status))
        {
            break;
        }
    }
}




#if 0

 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}





uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待发送缓冲区为空，TXE事件 */
  while (__HAL_SPI_GET_FLAG( &spi_a, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  WRITE_REG(spi_a.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (__HAL_SPI_GET_FLAG( &spi_a, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return READ_REG(spi_a.Instance->DR);
}

#endif



// /**
//  * @brief  向FLASH发送 写使能 命令
//  * @param  none
//  * @retval none
//  */
//void SPI_FLASH_WriteEnable(void)
//{
//  /* 通讯开始：CS低 */
//  SPI_FLASH_CS_LOW();

//  /* 发送写使能命令*/
//  SPI_FLASH_SendByte(W25X_WriteEnable);

//  /*通讯结束：CS高 */
//  SPI_FLASH_CS_HIGH();
//	
//}


#if 0
 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* 选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 读状态寄存器 命令 */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 若FLASH忙碌，则等待 */
  do
  {
    /* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */

  /* 停止信号  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}
#endif

//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  /* 选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 掉电 命令 */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* 停止信号  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  /*选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发上 上电 命令 */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* 停止信号 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();                   //等待TRES1
}   

 /**
  * @brief  使 SPI_FLASH 进入 4 字节地址模式
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*选择 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送状态寄存器 3 命令 */
	SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = SPI_FLASH_ReadByte();
	
	/* 停止信号 FLASH: CS 高 */
	SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*选择 FLASH: CS 低 */
		SPI_FLASH_CS_LOW();
		
		/* 进入4字节模式 */
		SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* 停止信号 FLASH: CS 高 */
		SPI_FLASH_CS_HIGH();
	}
}


/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_SPI_ERROR("SPI overtime!errorCode = %d",errorCode);
  return 0;
}
   































void SPI_FLASH_Init(void){

MX_SPI_A_Init();
SPI_FLASH_Mode_Init();


}
















/*********************************************END OF FILE**********************/
