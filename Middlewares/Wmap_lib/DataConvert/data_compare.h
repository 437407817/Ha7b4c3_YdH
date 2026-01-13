#ifndef __DATA_COMPARE_H
#define	__DATA_COMPARE_H

#include "stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>





void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);

uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

uint8_t Buffercmp32(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);


uint8_t find_different_bit_position(uint16_t old_flag, uint16_t new_flag);






#endif







