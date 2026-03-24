#ifndef __SD_485DIS_H
#define	__SD_485DIS_H


#include "stm32fxxx.h"

#include "./usart/p_data_queue.h"







uint8_t SD_c485_ReadfromQ_dataFromatVerify(uint8_t** q_data, uint8_t q_len);
void SD_s485_copy_data_from_queue(QUEUE_DATA_BUFF *qdf,uint8_t **frame,uint16_t* length,uint8_t* Command_num);
void pull_data_from_SD_485(void);
void SD_s485dataswitch(uint8_t** p_data,uint8_t num,uint8_t command_num);
void SD_getTime_Fun(uint8_t* p_data,uint8_t num);

void SD_getOK_Fun(uint8_t** p_data,uint8_t num);

void pull_data_from_SD_485(void);











#endif /* __SD_485DIS_H */
