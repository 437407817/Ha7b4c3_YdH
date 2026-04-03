#ifndef __DATA_RANDOM_H
#define	__DATA_RANDOM_H

#include "stm32_FH_xxx_hal.h"
#include <stdio.h>

#include <stdlib.h>





typedef struct
{
  uint8_t        up_ten;    // +10
  uint8_t        up_five;   // +5
  uint8_t        up_two;    // +2
  uint8_t        up_one;    // +1
  uint8_t        zero;      // +0
  uint8_t        down_one;  // -1
  uint8_t        down_two;  // -2
  uint8_t        down_five; // -5
  uint8_t        down_ten;  // -10
} RNG_UpDownIncreaseTypeDef;

// 2. 枚举：对应结构体的操作类型（方便映射权重和数值变化）
typedef enum
{
  OP_UP_TEN,
  OP_UP_FIVE,
  OP_UP_TWO,
  OP_UP_ONE,
  OP_ZERO,
  OP_DOWN_ONE,
  OP_DOWN_TWO,
  OP_DOWN_FIVE,
  OP_DOWN_TEN,
  OP_MAX  // 枚举总数
} RNG_OpTypeDef;

// 3. 权重-操作映射表（记录每个操作的权重区间和数值变化量）
typedef struct
{
  RNG_OpTypeDef op;        // 操作类型
  int8_t        delta;     // 数值变化量（+10/-1等）
  uint8_t       weight;    // 权重值
  uint8_t       start;     // 权重区间起始（0~9，总和10）
  uint8_t       end;       // 权重区间结束（0~9）
} WeightMapTypeDef;


void MX_RNG_Init(void);

//void random_generate(uint8_t *pBuffer, uint32_t uwBufferLenght);

HAL_StatusTypeDef  random_generate(uint32_t *pBuffer, uint32_t uwBufferLenght);

void fill_data_False_random(char *data, uint16_t max_len) ;



void Weight_RANDOM_INIT(void);
int8_t RNG_SelectOpByWeight(void);



#endif







