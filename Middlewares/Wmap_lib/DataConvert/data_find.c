#include "./DataConvert/data_find.h"




















/**
 * 查找uint8_t变量中最高位为1的位置（从1开始计数，最高位为8）
 * @param value 待检查的uint8_t值
 * @return 最高位1的位置（1-8），若值为0则返回0
 */
uint8_t findHighestSetBit(uint8_t value) {
    if (value == 0) {
        return 0; // 特殊返回值表示没有设置任何位
    }
    
    uint8_t position = 1;
    while (value > 1) {
        value >>= 1;
        position++;
    }
    return position;
}

#if 0

uint8_t result;

result = findHighestSetBit(0b00000001); // 返回0（最低位）
result = findHighestSetBit(0b00001000); // 返回3
result = findHighestSetBit(0b10000000); // 返回7（最高位）
result = findHighestSetBit(0);          // 返回0xFF（表示没有设置位）

#endif



#if 0
//性能优化（查表法
static const uint8_t highestBitTable[256] = {
    0xFF, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

uint8_t findHighestSetBit(uint8_t value) {
    return highestBitTable[value];
} // 返回0xFF（表示没有设置位）

#endif











