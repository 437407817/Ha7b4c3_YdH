#ifndef __GV_VARIABLE_H
#define	__GV_VARIABLE_H


#include "stm32fxxx.h"










typedef struct {
	
uint8_t ChipID1[4];
uint8_t ChipID2[4];
uint8_t ChipID3[3];	
	
uint8_t ChipisActive;	//1激活，2未激活可用，3

uint8_t overtime_day;
uint8_t overtime_month;	
uint8_t overtime_year;
//char code[40];

}str_ChipEncode_data;




typedef struct {
	
str_ChipEncode_data chipdata;
	
char code[40];

}str_ChipEncode;










extern str_ChipEncode GV_EEpromChipEncode;
extern str_ChipEncode GV_SaveEEpChipEncode;























#endif /* __BASICMOTION_H */

