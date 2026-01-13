#ifndef __DATA_RANDOM_H
#define	__DATA_RANDOM_H

#include "stm32_FH_xxx_hal.h"
#include <stdio.h>

#include <stdlib.h>



void MX_RNG_Init(void);

//void random_generate(uint8_t *pBuffer, uint32_t uwBufferLenght);

HAL_StatusTypeDef  random_generate(uint32_t *pBuffer, uint32_t uwBufferLenght);

void fill_data_False_random(char *data, uint16_t max_len) ;

#endif







