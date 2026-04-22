#ifndef _USART485VERIFY_H_
#define _USART485VERIFY_H_

#include <stdint.h>



#include "./pro_com/usart_485_232_send.h"


#if c485_232_CRC16
#define PACKET_DATA_LEN_MIN    20                  //最小包长度
#define PACKET_DATA_LEN_MAX    122                 //最大包长度
#else
#define PACKET_DATA_LEN_MIN    10                  //最小包长度
#define PACKET_DATA_LEN_MAX    120                 //最大包长度
#endif

#define MAX_BUF_SIZE           (PACKET_DATA_LEN_MAX * 8)										//


//void s485_convert_queue(uint8_t **outalldata,uint8_t** OutData,uint8_t* Datanum,uint8_t* Command_num);
void s485_convert_queue(uint8_t **outalldata,uint16_t* Command_address,uint8_t** OutData,uint8_t* Datanum);

/**
***********************************************************
* @brief USB转串口应用初始化函数
* @param
* @return 
***********************************************************
*/
void Usart485ComAppInit(void);

/**
***********************************************************
* @brief USB转串口任务处理函数
* @param
* @return 
***********************************************************
*/
void Usart485ComTask(void);

#endif
