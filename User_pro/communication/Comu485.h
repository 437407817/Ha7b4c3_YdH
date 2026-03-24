#ifndef __COMU485_H
#define	__COMU485_H


#include "stm32_FH_xxx.h"


/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   LED编号  亮/灭  异或校验数据
        0x55   0xAA    0x03      0x06     0x00     0x01      0xFB
***********************************************************************
*/
/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   数据    CRC低   CRC高
        0x55   0xAA    0x03      0x06    0x00    0x01     0x01 
***********************************************************************
*/
#define FRAME_HEAD_0           0x5a  
#define FRAME_HEAD_1           0xa5

#define CTRL_CODE_SET          0x01               //设置+启动（停止）功能字
#define CTRL_CODE_ASK          0x02               //询问数据功能字
#define CTRL_CODE_rectify      0x03               //校正数据功能字


#define CTRL_CODE_upload       0x10               //返回电压数据功能字


#define c485_CRC16 1


void c485_write_cmd(uint8_t *data,uint16_t size, uint8_t cmd,uint8_t OverGetRX);

//void c485_read_cmd(uint16_t addr, uint8_t num);

//void c485_write_cmd_print(uint8_t *data,uint16_t size, uint8_t cmd, uint16_t debugprint);
void c485_write_cmd_print(uint8_t *data,uint16_t size, uint8_t cmd,uint8_t OverGetRX, uint16_t debugprint);
//uint8_t c485_ReadfromQ_dataFromatVerify(uint8_t** q_data, uint8_t q_len);













#endif /* __Comu485Dis_H */
