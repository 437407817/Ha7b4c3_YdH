

#ifndef __COMU485DATAFUN_H
#define	__COMU485DATAFUN_H





#include "stm32fxxx.h"




void s485GetData_Dispose(uint8_t** p_data,uint8_t num);





void s485_SendSlaveAskdataAfter50MS(void);













#endif /* __COMU485DATAFUN_H */



