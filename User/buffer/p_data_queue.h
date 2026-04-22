#ifndef __P_DATA_QUEUE_H_
#define __P_DATA_QUEUE_H_

#include "stm32_FH_xxx_hal.h"
#include "./sys/sysio.h"
#include <string.h>
#include <stdio.h>





// 数据类型结构体
typedef struct {
    char  *q_head;    // 缓冲区头指针
    uint16_t q_len;   // 接收到的数据长度
} Q_QUEUE_DATA_TYPE;





// 队列结构
typedef struct {
    int         size;            // 缓冲区大小
    int         read;            // 读指针
    int         write;           // 写指针
    int read_using;              // 正在读取的缓冲区指针
    int write_using;             // 正在写入的缓冲区指针
    Q_QUEUE_DATA_TYPE    *elems;  // 缓冲区地址，指向全局定义的数组
    void (*p_queuecb)(uint8_t **frame, uint16_t* length);
} Q_QueueBuffer;



// 节点数据缓冲区结构体
typedef struct str_nood_data_buff {
    char *node_buff;              // 指向全局定义的二维数组
    Q_QUEUE_DATA_TYPE *node_data;  // 指向全局定义的结构体数组
} NODE_DATA_BUFF;


// 定义不同的队列节点数量和数据长度宏
#define q_QUEUE_NODE_NUM_1        (8)  //缓冲队列的个数需要为2的幂,否则出错
#define q_QUEUE_NODE_DATA_LEN_1   (1 * 50)

#define q_QUEUE_NODE_NUM_2        (1*4) //缓冲队列的个数需要为2的幂,否则出错
#define q_QUEUE_NODE_DATA_LEN_2   (1 * 50)

#define q_QUEUE_NODE_NUM_UsartDMAsend        (1*4) //缓冲队列的个数需要为2的幂,否则出错//最大多少组
#define q_QUEUE_NODE_DATA_LEN_UsartDMAsend   (1 * 16) //每组多少个数


//#define creat_que(que_name, len)	\
//    unsigned char que_buf_##que_name[len] = {0}; \
//    que_t que_##que_name = {que_buf_##que_name, len, 0, 0}; \
//    static que_t* que_name = &que_##que_name
//定义创建队列的方法
#define CREAT_p_queue(que_name, node_num,node_len)	\
Q_QUEUE_DATA_TYPE global_elems_##que_name[node_num];	\
char global_node_buff_##que_name[node_num][node_len];	\
Q_QUEUE_DATA_TYPE  node_data_##que_name[node_num];	\
Q_QueueBuffer q_tx_rx_queue_##que_name;	\
NODE_DATA_BUFF GV_ndb_##que_name;	



//输出队列的状态信息
#define p_cbPrint_size(cb)		    SYSTEM_DEBUG("\n %s :size=0x%x,read=%d, write=%d,read_using=%d,  write_using=%d \n",STR_PARAM(cb), cb.size, cb.read, cb.write,cb.read_using ,cb.write_using);

#define p_cbPrint_buff(bf)		    do{SYSTEM_DEBUG_ARRAY((uint8_t *)bf,sizeof(bf))}while(0)











void p_cbInit(Q_QueueBuffer *cb, int size,Q_QUEUE_DATA_TYPE *elems);

int p_cbIsFull(Q_QueueBuffer *cb);
int p_cbIsEmpty(Q_QueueBuffer *cb);
int p_cbIncr(Q_QueueBuffer *cb, int p);
Q_QUEUE_DATA_TYPE* p_cbWrite(Q_QueueBuffer *cb);
void p_cbWriteFinish(Q_QueueBuffer *cb);
Q_QUEUE_DATA_TYPE* p_cbRead(Q_QueueBuffer *cb);
void p_cbReadFinish(Q_QueueBuffer *cb);
void p_camera_queue_free(NODE_DATA_BUFF *ndb, int node_num);

//void p_tx_rx_queue_init(QUEUE_DATA_BUFF *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length,Q_QUEUE_DATA_TYPE *elems);
void p_tx_rx_queue_init(Q_QueueBuffer *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length,Q_QUEUE_DATA_TYPE *elems,Q_QUEUE_DATA_TYPE *node_data,char (*node_buff)[qnd_length]);


void p_push_data_to_queue(Q_QueueBuffer *qdf, char *src_dat, uint16_t src_len);

int p_push_data_to_queue2(Q_QueueBuffer *qdf,  char *src_dat, uint16_t src_len);
int p_pop_data_from_queue2(Q_QueueBuffer *qdf, char *dest_dat, uint16_t dest_max_len, uint16_t *out_len);


void p_copy_data_from_queue(Q_QueueBuffer *qdf, uint8_t **frame, uint16_t* length);
void p_pull_data_from_queue(Q_QueueBuffer *qdf);
void p_queue_Callback(void (*pFunc)(uint8_t **frame, uint16_t* length), Q_QueueBuffer *qdf);
void p_clear_queue_Callback(Q_QueueBuffer *qdf);








#endif



