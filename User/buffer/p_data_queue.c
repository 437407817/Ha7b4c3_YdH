/**
  ******************************************************************************
  * @file    p_data_queue.c
  * @author  wmap
  * @version V1.0
  * @date    2024-12-24
  * @brief   环形缓冲区，适用于接收外部数据时用作缓冲
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 IOT STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./buffer/p_data_queue.h"
#include "./sys/sysio.h"
#include "./usart/bsp_usart.h"



#if 0
// 预先定义全局一维数组用于不同情况的 Q_QueueBuffer 结构体中的 elems 指针指向
Q_QUEUE_DATA_TYPE global_elems_1[q_QUEUE_NODE_NUM_1];
Q_QUEUE_DATA_TYPE global_elems_2[q_QUEUE_NODE_NUM_2];

// 预先定义全局二维数组用于不同情况的 NODE_DATA_BUFF 结构体中的 node_buff 指针指向
char global_node_buff_1[q_QUEUE_NODE_NUM_1][q_QUEUE_NODE_DATA_LEN_1];
char global_node_buff_2[q_QUEUE_NODE_NUM_2][q_QUEUE_NODE_DATA_LEN_2];

Q_QUEUE_DATA_TYPE  node_data_1[q_QUEUE_NODE_NUM_1];
Q_QUEUE_DATA_TYPE  node_data_2[q_QUEUE_NODE_NUM_2];


Q_QueueBuffer q_rx_queue_1;
NODE_DATA_BUFF GV_ndb_1;

Q_QueueBuffer q_rx_queue_2;
NODE_DATA_BUFF GV_ndb_2;
#endif



void p_cbInit(Q_QueueBuffer *cb, int size,Q_QUEUE_DATA_TYPE *elems) {
    cb->size = size;
    cb->read = 0;
    cb->write = 0;
    cb->read_using = 0;
    cb->write_using = 0;

    //cb->elems = global_elems_1;
	    cb->elems = elems;
    // 初始化elems数组元素的成员
    for (int i = 0; i < size; i++) {
        cb->elems[i].q_head = NULL;
        cb->elems[i].q_len = 0;
    }
}









// 判断缓冲队列是否已满
int p_cbIsFull(Q_QueueBuffer *cb) {
    return cb->write == (cb->read ^ cb->size);
}

// 判断缓冲队列是否全空
int p_cbIsEmpty(Q_QueueBuffer *cb) {
    return cb->write == cb->read;
}

// 对缓冲队列的指针加 1
int p_cbIncr(Q_QueueBuffer *cb, int p) {
    return (p + 1) & (2 * cb->size - 1);
}

// 获取可写入的缓冲区指针
Q_QUEUE_DATA_TYPE* p_cbWrite(Q_QueueBuffer *cb) {
    if (p_cbIsFull(cb)) {
        return NULL;
    } else {
        // 当wriet和write_using相等时，表示上一个缓冲区已写入完毕，需要对写指针加1
        if (cb->write == cb->write_using) {
            cb->write_using = p_cbIncr(cb, cb->write);
        }
    }
    return &(cb->elems[cb->write_using & (cb->size - 1)]);
}

// 数据写入完毕，更新写指针到缓冲结构体
void p_cbWriteFinish(Q_QueueBuffer *cb) {
    cb->write = cb->write_using;
}

// 获取可读取的缓冲区指针
Q_QUEUE_DATA_TYPE* p_cbRead(Q_QueueBuffer *cb) {
    if (p_cbIsEmpty(cb)) {
        return NULL;
    }
    // 当read和read_using相等时，表示上一个缓冲区已读取完毕(即已调用cbReadFinish)，需要对写指针加1
    if (cb->read == cb->read_using) {
        cb->read_using = p_cbIncr(cb, cb->read);
    }
    return &(cb->elems[cb->read_using & (cb->size - 1)]);
}

// 数据读取完毕，更新读指针到缓冲结构体
void p_cbReadFinish(Q_QueueBuffer *cb) {
    // 重置当前读完的数据节点的长度
    (cb->elems[cb->read_using & (cb->size - 1)]).q_len = 0;
    cb->read = cb->read_using;
}

// 队列的指针指向的缓冲区全部销毁
void p_camera_queue_free(NODE_DATA_BUFF *ndb, int node_num) {
    uint32_t i = 0;
    for (i = 0; i < node_num; i++) {
        if (ndb->node_data[i].q_head!= NULL) {
            // 若是动态申请的空间才要free
            // free(node_data[i].q_head);
            ndb->node_data[i].q_head = NULL;
        }
    }
}


// 缓冲队列初始化函数（针对第一种情况）
void p_tx_rx_queue_init(Q_QueueBuffer *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length,Q_QUEUE_DATA_TYPE *elems,Q_QUEUE_DATA_TYPE *node_data,char (*node_buff)[qnd_length]) {
    uint32_t i = 0;
	ndb->node_data=node_data;

	memset((ndb)->node_data, 0, qnn_size * sizeof(Q_QUEUE_DATA_TYPE));
	//SYSTEM_DEBUG("global_node_buff_1 address：%p\n", global_node_buff_1);
	//SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t*)node_data_1,sizeof(node_data_1)," ==xxxxx====2  %d",sizeof(node_data_1));
	    // 初始化缓冲队列
    //p_cbInit(&(qdf)->q_rx_queue,qnn_size,elems);
    p_cbInit(qdf,qnn_size,elems);
    // 初始化node_data数组元素的成员
    for (i = 0; i < qnn_size; i++) {
       // ndb->node_data[i].q_head = global_node_buff_1[i];//123456
        ndb->node_data[i].q_head = node_buff[i];

        // 初始化队列缓冲指针，指向实际的内存
        qdf->elems[i] = (ndb->node_data[i]);

        memset(ndb->node_data[i].q_head, 0, qnd_length);
    }
	//p_cbPrint_size((qdf)->q_rx_queue);	

}





// 向队列推送数据（针对第一种情况）
void p_push_data_to_queue(Q_QueueBuffer *qdf, char *src_dat, uint16_t src_len) {
    Q_QUEUE_DATA_TYPE *data_p;
    uint16_t i = 0;

	for(i=0;i<src_len;i++)
	{
		/*获取写缓冲区指针，准备写入新数据*/
		data_p = p_cbWrite(qdf);
		
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		
			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->q_head + i) = src_dat[i];
				data_p->q_len++;
		//	SYSTEM_DEBUG("\r\ndata_p->q_len =%d",data_p->q_len);
		}else return;	
		
		
	}	

    // 写入缓冲区完毕
    p_cbWriteFinish(qdf);

    // 打印队列最终状态信息（可根据需求决定是否保留）
    // p_cbPrint_size((qdf)->q_rx_queue);
}


/**
 * @brief 向环形队列压入数据
 * @param qdf：队列结构体指针
 * @param src_dat：待压入的数据指针
 * @param src_len：待压入的数据长度
 * @retval 0：成功；-1：队列满，压入失败
 */
int p_push_data_to_queue2(Q_QueueBuffer *qdf,  char *src_dat, uint16_t src_len) {
    if (qdf == NULL || src_dat == NULL || src_len == 0) {
        return 1; // 参数无效
    }

    Q_QUEUE_DATA_TYPE *write_node = p_cbWrite(qdf); // 获取可写节点
    if (write_node == NULL) {
        return 1; // 队列满，无法写入
    }

    // 复制数据到队列节点（确保不超过节点缓冲区长度）
    // 注：需提前保证节点缓冲区大小 >= src_len，否则会溢出
    memcpy(write_node->q_head, src_dat, src_len);
    write_node->q_len = src_len; // 记录数据长度

    p_cbWriteFinish(qdf); // 标记写入完成，更新写指针
    return 0; // 成功
}

/**
 * @brief 从环形队列弹出数据（出列）
 * @param qdf：队列结构体指针
 * @param dest_dat：接收数据的缓冲区指针
 * @param dest_max_len：接收缓冲区的最大长度（防止溢出）
 * @param out_len：实际读取到的数据长度（输出参数）
 * @retval 0：成功；-1：队列空或参数无效；-2：接收缓冲区太小
 */
int p_pop_data_from_queue2(Q_QueueBuffer *qdf, char *dest_dat, uint16_t dest_max_len, uint16_t *out_len) {
    if (qdf == NULL || dest_dat == NULL || out_len == NULL || dest_max_len == 0) {
        return 1; // 参数无效
    }

    Q_QUEUE_DATA_TYPE *read_node = p_cbRead(qdf); // 获取可读节点
    if (read_node == NULL) {
        *out_len = 0;
        return 1; // 队列空，无数据可读
    }

    // 检查接收缓冲区是否足够
    if (read_node->q_len > dest_max_len) {
        *out_len = 0;
        return 2; // 缓冲区不足
    }

    // 复制数据到外部缓冲区
    memcpy(dest_dat, read_node->q_head, read_node->q_len);
    *out_len = read_node->q_len; // 输出实际长度

    p_cbReadFinish(qdf); // 标记读取完成，更新读指针
    return 0; // 成功
}



// 从队列拉取数据
//void p_pull_data_from_queue(QUEUE_DATA_BUFF *qdf) {
//    Q_QUEUE_DATA_TYPE *data_p;
//    uint8_t *frame = NULL;
//    uint16_t leng = 0;

//    data_p = p_cbRead(qdf->q_rx_queue);
//    if (data_p!= NULL) {
//        // 根据实际数据长度分配内存空间
//        frame = (uint8_t *)malloc(data_p->q_len * sizeof(uint8_t));
//        if (frame == NULL) {
//            printf("内存分配失败！\n");
//            return;
//        }

//        // 复制数据到新分配的内存空间
//        memcpy(frame, data_p->q_head, data_p->q_len);
//        leng = data_p->q_len;

//        // 调用回调函数处理数据
//        if (qdf->q_rx_queue.p_queuecb!= NULL) {
//            (*qdf->q_rx_queue.p_queuecb)(&frame, &leng);
//        }

//        // 打印数据信息
//        // SYSTEM_DEBUG("usart test received length: %d", leng);
//        // SYSTEM_DEBUG("usart test received data：%s,length: %d", frame, leng);
//        // SYSTEM_DEBUG_ARRAY((uint8_t*)frame, leng);

//        // 释放内存
//        free(frame);
//        frame = NULL;
//        leng = 0;

//        // 更新读指针
//        p_cbReadFinish(qdf->q_rx_queue);
//    }
//}

// 从队列复制数据（针对两种情况通用，通过传入不同的节点数量和数据长度宏来适配）在复制的数据末尾添加字符串结束符 \0，将从队列中读取的字节数据转换为 C 语言标准的字符串格式。
void p_copy_data_from_queue(Q_QueueBuffer *qdf, uint8_t **frame, uint16_t* length) {
    Q_QUEUE_DATA_TYPE *rx_data;

    rx_data = p_cbRead(qdf);
    if (rx_data!= NULL) {  // 缓冲队列非空
        *length = rx_data->q_len;

        // 使用传入的外部数组（调用者需要保证其大小足够容纳数据）来存储复制的数据
        // 假设外部数组大小足够，这里不再动态分配内存
					*frame = (uint8_t *)malloc(rx_data->q_len+1);
			if (*frame == NULL) {  // 检查 malloc 是否成功（避免空指针）
    *length = 0;
    return;
}
			
        memcpy(*frame, rx_data->q_head, rx_data->q_len);
		*(*frame+rx_data->q_len) = '\0';//方便打印情况下可以终止
			
		//SYSTEM_DEBUG(" ==xxxxx====length: %d",*length);
		//SYSTEM_DEBUG("==received data：%s  ,length: %d",*frame,rx_data->q_len);
		//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);
	//free(frame);xxxxxx
	//frame = NULL;xxxxx
			
        // 使用完数据必须调用cbReadFinish更新读指针
        p_cbReadFinish(qdf);
    }
}





// 从队列拉取数据打印到jlink终端，不对数据做任何处理，只打印（针对两种情况通用，通过传入不同的节点数量和数据长度宏来适配）
void p_pull_data_from_queue(Q_QueueBuffer *qdf) {


		uint8_t *frame = NULL;

uint16_t leng=0;//必须为0

p_copy_data_from_queue(qdf,&frame,&leng);
	
	if(qdf->p_queuecb != NULL){
	(*qdf->p_queuecb)(&frame,&leng);
	}
	
			if(leng!=0){
				SYSTEM_DEBUG("usart received data=:%s  ,length: %d",frame,leng);
      SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,leng,"");		
			}

	
		free(frame);
	frame = NULL;
	leng=0;

}



// 设置队列回调函数
void p_queue_Callback(void (*pFunc)(uint8_t **frame, uint16_t* length), Q_QueueBuffer *qdf) {
    qdf->p_queuecb = pFunc;
}

// 清除队列回调函数
void p_clear_queue_Callback(Q_QueueBuffer *qdf) {
    qdf->p_queuecb = NULL;
}













