#include "./DataConvert/data_set.h"





#include "./sys/sysio.h"





// 2. 赋值函数：你要的核心函数
void setInt16Array(int16_t *array, uint16_t len, GetIncreaseValueFunc_t func)
{
    // 安全判断
    if (array == NULL || func == NULL)
        return;

    // 循环给数组赋值
    for (uint16_t i = 0; i < len; i++)
    {
        array[i] =array[i]+func(); // 调用函数获取值，赋值给数组
			SYSTEM_INFO("%x ",array[i]);
    }
}




#include "./DataConvert/data_random.h"

void testsetInt16Array(void){

int16_t adv[10];

//Weight_RANDOM_INIT
	Weight_RANDOM_INIT();
	
setInt16Array(adv,10,RNG_SelectOpByWeight);
	
}




















