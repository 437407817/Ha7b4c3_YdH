#ifndef __COMU485DIS_H
#define	__COMU485DIS_H


#include "stm32fxxx.h"

#include "./usart/p_data_queue.h"

//void comu_SendslaveRectifydata(void);


void comu_Sendslave_Rundata(void);

void SendCommu485data(void);

void comu_SendslaveRectifydata(void);



void comu_SendslaveAskdata(void);


void s485_copy_data_from_queue(QUEUE_DATA_BUFF *qdf,uint8_t **frame,uint16_t* length,uint8_t** Data,uint8_t* Datanum,uint8_t* Command_num);

void pull_data_from_485(void);

void s485dataswitch(uint8_t** p_data,uint8_t num,uint8_t command_num);




#endif /* __Comu485Dis_H */
