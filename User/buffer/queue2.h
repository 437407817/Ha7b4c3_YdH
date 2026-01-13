#ifndef __QUEUE2_H_
#define __QUEUE2_H_

#include "stm32_FH_xxx_hal.h"
#include "./sys/sysio.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>



#include <stdint.h>
#if 0
typedef struct
{
    uint32_t head;        //数组下标，指向队头
    uint32_t tail;        //数组下标，指向队尾
    uint32_t size;        //队列缓存长度（初始化时赋值）
    uint8_t *buffer;      //队列缓存数组（初始化时赋值）
} QueueType_t;

typedef enum
{
	QUEUE_OK = 0,       //队列正常
	QUEUE_ERROR,        //队列错误
	QUEUE_OVERLOAD,     //队列已满
	QUEUE_EMPTY         //队列已空
} QueueStatus_t;

/**
****************************************************************
* @brief   初始化（创建）队列，每个队列须先执行该函数才能使用
* @param   queue, 队列变量指针
* @param   buffer, 队列缓存区地址
* @param   size, 队列缓存区长度
* @return 
****************************************************************
*/
void QueueInit(QueueType_t *queue, uint8_t *buffer, uint32_t size);

/**
****************************************************************
* @brief   压入数据到队列中
* @param   queue, 队列变量指针
* @param   data, 待压入队列的数据
* @return  压入队列是否成功
****************************************************************
*/
QueueStatus_t QueuePush(QueueType_t *queue, uint8_t data);

/**
****************************************************************
* @brief   从队列中弹出数据
* @param   queue, 队列变量指针
* @param   pdata, 待弹出队列的数据缓存地址
* @return  弹出队列是否成功
****************************************************************
*/
QueueStatus_t QueuePop(QueueType_t *queue, uint8_t *pdata);

/**
****************************************************************
* @brief   压入一组数据到队列中
* @param   queue, 队列变量指针
* @param   pArray, 待压入队列的数组地址
* @param   len, 待压入队列的元素个数
* @return  实际压入到队列的元素个数
****************************************************************
*/
uint32_t QueuePushArray(QueueType_t *queue, uint8_t *pArray, uint32_t len);

/**
****************************************************************
* @brief   从队列中弹出一组数据
* @param   queue, 队列变量指针
* @param   pArray, 待弹出队列的数据缓存地址
* @param   len, 待弹出队列的数据的最大长度
* @return  实际弹出数据的数量
****************************************************************
*/
uint32_t QueuePopArray(QueueType_t *queue, uint8_t *pArray, uint32_t len);

/**
****************************************************************
* @brief   获取队列中数据的个数
* @param   queue, 队列变量指针
* @return  队列中数据的个数
****************************************************************
*/
uint32_t QueueCount(QueueType_t *queue);
#endif

// 环形缓冲区结构体
typedef struct {
    uint8_t *buf;       // 缓冲区数组
    uint16_t size;      // 缓冲区大小
    uint16_t read_idx;  // 读指针（下一个待读取位置）
    uint16_t write_idx; // 写指针（下一个待写入位置）
} RingBuffer_t;


//#define RING_BUF_SIZE 5







 void RingBuffer_Init(RingBuffer_t *rb, uint8_t *buf, uint16_t size);
 bool RingBuffer_IsEmpty(RingBuffer_t *rb);
 bool RingBuffer_IsFull(RingBuffer_t *rb);
 bool RingBuffer_WriteByte(RingBuffer_t *rb, uint8_t data);
 bool RingBuffer_ReadByte(RingBuffer_t *rb, uint8_t *data);
 void RingBuffer_WriteMulti(RingBuffer_t *rb, const uint8_t *data, uint16_t len);
uint16_t RingBuffer_GetAvailable(RingBuffer_t *rb);
uint16_t RingBuffer_ReadMulti(RingBuffer_t *rb, uint8_t *buf, uint16_t len);



#endif



