#ifndef _SYS_DWT_H_
#define _SYS_DWT_H_

#include <stdint.h>
/**
***********************************************************
* @brief _SYS_DWT_H_≥ı ºªØ
* @param
* @return 
***********************************************************
*/




void DWT_Init(void);




void DWT_Delay_ms(uint32_t ms);
void DWT_Delay_us(uint32_t us); 
void DWT_Delay_ns(uint32_t ns);




uint32_t dwt_getCurrentTick(void);


void dwt_clearCurrentTick(void);

void DWT_Start_Time(void);
void DWT_Stop_Time(void);
void DWT_Calc_Time(uint32_t *cycles, uint32_t *us_int, uint32_t *ns_remain);




#endif
