

#include "./DataConvert/data_compare.h"
//#include "./usart/bsp_usart.h"




/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;
  
  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}



uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}


uint8_t Buffercmp32(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}


uint8_t find_different_bit_position(uint16_t old_flag, uint16_t new_flag) {
    // 计算异或结果，不同的位会被置为1
    uint8_t xor_result = old_flag ^ new_flag;
    
    // 如果两个值完全相同，返回0xFF表示没有不同位
    if (xor_result == 0) {
        return 0xFF;
    }
    
    // 查找第一个不同的位（从低位到高位）
    uint8_t position = 0;
    while ((xor_result & 0x01) == 0) {
        xor_result >>= 1;
        position++;
    }
    
    return position;
}





/**************************END OF FILE************************************/











