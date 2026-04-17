

#include "./DataConvert/data_random.h"
//#include "./usart/bsp_usart.h"
 #include "./sys/sysio.h"
//#include <time.h>



//int UpDownIncrease[]={-10,-5,-2,-1,0,1,2,5,10};


//void dd(){
//int c;
//c=UpDownIncrease[CreateFalseRandNum(0,sizeof(UpDownIncrease))];

//}
// 全局RNG句柄（必须全局，供HAL_RNG_GenerateRandomNumber调用）

// 全局RNG句柄（只需初始化一次）
static RNG_HandleTypeDef hrng = {0};

void MX_RNG_Init(void)
{
  // 1. 配置RNG句柄的核心参数（Instance=外设地址，是唯一需要根据芯片调整的部分）
  hrng.Instance = RNG;  // 注意：部分型号（如H7）可能是RNG1/RNG2，需改为RNG1
  
  // 2. 使能RNG外设时钟（关键！时钟未使能会导致初始化失败）
  __HAL_RCC_RNG_CLK_ENABLE();

  // 3. 复位RNG外设（清除之前的异常状态，确保初始化干净）
  __HAL_RNG_RESET_HANDLE_STATE(&hrng);

  // 4. 初始化RNG外设（HAL库底层配置，无需修改）
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();  // 初始化失败时调用错误处理函数（用户工程需实现）
  }
}


HAL_StatusTypeDef  random_generate(uint32_t *pBuffer, uint32_t uwBufferLenght)
{

	RNG_HandleTypeDef hand_random;
	/*使能RNG时钟*/
    __RNG_CLK_ENABLE();
	/*初始化RNG模块产生随机数*/
    hand_random.Instance = RNG;
    HAL_RNG_Init(&hand_random);

   for(uint32_t count=0;count<uwBufferLenght;count++)
    {
          HAL_StatusTypeDef status = HAL_RNG_GenerateRandomNumber(&hrng, &pBuffer[count]);
        if (status != HAL_OK)
        {
            return status; // 单个随机数生成失败，返回错误码
        }

    }    
		return HAL_OK; // 全部生成成功
}



uint16_t CreateFalseRandNum(uint16_t BeginNum,uint16_t EndNum){
	
	if(EndNum>BeginNum){
return (rand() % (EndNum+1-BeginNum))+BeginNum;
	}else{
		
	return 0;
	
	}

}

// 6. 生成0~9的随机数（基于STM32硬件RNG，无硬件则用软件随机数 fallback）
static uint8_t RNG_GetRandomNum(uint16_t BeginNum,uint16_t EndNum)
{
	
  uint32_t random_val = 0;

  // 方法1：使用STM32硬件RNG（推荐，随机数更均匀）
  if (HAL_RNG_GenerateRandomNumber(&hrng, &random_val) == HAL_OK)
  {
    return (uint8_t)(random_val % (EndNum+1-BeginNum))+BeginNum;  // 取模10，得到0~9
  }

  // 方法2：软件随机数 fallback（无硬件RNG时使用，简单实现）
//  static uint32_t lfsr = 0x12345678;  // 初始种子（可替换为定时器值提升随机性）
//  lfsr = (lfsr << 1) ^ ((lfsr >> 31) & 0x07);  // 线性反馈移位寄存器
//		if(EndNum>BeginNum){
//return (lfsr % (EndNum+1-BeginNum))+BeginNum;
//		}
		return 0;
		//  return (uint8_t)(lfsr % 10);
}



/**
 * @brief 将data数组填充为随机字符
 * @param data：待填充的字符数组
 * @param max_len：数组最大长度（避免越界，留1字节存字符串结束符）最后一个字符默认为'\0'
 */
void fill_data_False_random(char *data, uint16_t max_len) {
    if (max_len <= 1) return;  // 至少留1字节存'\0'，避免越界

    // 初始化随机数种子（仅需初始化一次，建议在main函数中调用）
//    static uint8_t seed_init = 0;
//    if (!seed_init) {
//        srand((unsigned int)time(NULL));  // 以系统时间为种子，保证每次运行随机结果不同
//        seed_init = 1;
//    }

    // 定义随机字符池（可按需调整，例如只保留字母：'A'-'Z','a'-'z'）
    const char char_pool[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:',.<>?";
    uint16_t pool_len = sizeof(char_pool) - 1;  // 排除字符串结束符'\0'

    // 填充随机字符（最多填充max_len-1个，留1字节存'\0'）
    for (uint16_t i = 0; i < max_len - 1; i++) {
        uint16_t rand_idx = rand() % pool_len;  // 生成0~pool_len-1的随机索引
        data[i] = char_pool[rand_idx];         // 从字符池取随机字符
    }

    data[max_len - 1] = '\0';  // 确保字符串结束符，兼容C语言字符串处理
}




///-------------------------------------------------------------------




// 4. 初始化权重结构体（权重总和=10，可根据需求调整各字段权重）
static void RNG_InitWeight(RNG_UpDownIncreaseTypeDef *weight)
{
  // 权重分配示例（总和=1+1+1+2+3+1+1+0+0=10）
  // 逻辑：小幅度调整（+1/0/-1）权重更高，大幅度调整（+10/-10）权重更低
  weight->up_ten    = 1;  // +10：权重1
  weight->up_five   = 1;  // +5：权重1
  weight->up_two    = 1;  // +2：权重1
  weight->up_one    = 1;  // +1：权重2
  weight->zero      = 1;  // +0：权重3（最高，最可能执行）
  weight->down_one  = 2;  // -1：权重1
  weight->down_two  = 1;  // -2：权重1
  weight->down_five = 1;  // -5：权重0（不执行）
  weight->down_ten  = 1;  // -10：权重0（不执行）
}

// 5. 构建权重映射表（根据权重结构体生成区间，用于随机选择）
static void RNG_BuildWeightMap(RNG_UpDownIncreaseTypeDef *weight)
{
	WeightMapTypeDef          map[OP_MAX];  // 权重映射表
  uint8_t current = 0;  // 当前区间起始位置

  // 逐个映射操作：权重>0才参与随机选择
  map[OP_UP_TEN] = (WeightMapTypeDef){OP_UP_TEN, +10, weight->up_ten, current, current + weight->up_ten - 1};
  current += weight->up_ten;

  map[OP_UP_FIVE] = (WeightMapTypeDef){OP_UP_FIVE, +5, weight->up_five, current, current + weight->up_five - 1};
  current += weight->up_five;

  map[OP_UP_TWO] = (WeightMapTypeDef){OP_UP_TWO, +2, weight->up_two, current, current + weight->up_two - 1};
  current += weight->up_two;

  map[OP_UP_ONE] = (WeightMapTypeDef){OP_UP_ONE, +1, weight->up_one, current, current + weight->up_one - 1};
  current += weight->up_one;

  map[OP_ZERO] = (WeightMapTypeDef){OP_ZERO, 0, weight->zero, current, current + weight->zero - 1};
  current += weight->zero;

  map[OP_DOWN_ONE] = (WeightMapTypeDef){OP_DOWN_ONE, -1, weight->down_one, current, current + weight->down_one - 1};
  current += weight->down_one;

  map[OP_DOWN_TWO] = (WeightMapTypeDef){OP_DOWN_TWO, -2, weight->down_two, current, current + weight->down_two - 1};
  current += weight->down_two;

  map[OP_DOWN_FIVE] = (WeightMapTypeDef){OP_DOWN_FIVE, -5, weight->down_five, current, current + weight->down_five - 1};
  current += weight->down_five;

  map[OP_DOWN_TEN] = (WeightMapTypeDef){OP_DOWN_TEN, -10, weight->down_ten, current, current + weight->down_ten - 1};

}



// 7. 根据权重选择操作，返回数值变化量delta
 int8_t RNG_SelectOpByWeight(void)
{
	 WeightMapTypeDef          weight_map[OP_MAX];  // 权重映射表
	
  uint8_t random_num = RNG_GetRandomNum(0,9);  // 生成0~9随机数

  // 遍历映射表，找到随机数所在的权重区间
  for (uint8_t i = 0; i < OP_MAX; i++)
  {
    if (weight_map[i].weight == 0) continue;  // 权重为0的操作跳过

    // 随机数在当前操作的区间内，返回对应的delta
    if (random_num >= weight_map[i].start && random_num <= weight_map[i].end)
    {
#if 0
      // （可选）打印选中的操作（调试用）
      switch (weight_map[i].op)
      {
        case OP_UP_TEN:    printf("+10 "); break;
        case OP_UP_FIVE:   printf("+5  "); break;
        case OP_UP_TWO:    printf("+2  "); break;
        case OP_UP_ONE:    printf("+1  "); break;
        case OP_ZERO:      printf("0   "); break;
        case OP_DOWN_ONE:  printf("-1  "); break;
        case OP_DOWN_TWO:  printf("-2  "); break;
        case OP_DOWN_FIVE: printf("-5  "); break;
        case OP_DOWN_TEN:  printf("-10 "); break;
        default: break;
      }
#endif
			
      return weight_map[i].delta;
    }
  }

  return 0;  // 异常默认返回0
}

void Weight_RANDOM_INIT(void){

RNG_UpDownIncreaseTypeDef weight;
  RNG_InitWeight(&weight);
  RNG_BuildWeightMap(&weight);
}



// 8. 核心测试函数：初始值100，运行10次，输出每次结果
void RNG_RunTest(int c_val,int r_count)
{
  RNG_UpDownIncreaseTypeDef weight;
//  WeightMapTypeDef          weight_map[OP_MAX];  // 权重映射表
  uint16_t                  current_val = c_val;   // 初始值
  const uint16_t             run_count = r_count;      // 运行次数

  // 步骤1：初始化权重和映射表
  RNG_InitWeight(&weight);
  RNG_BuildWeightMap(&weight);

  // 步骤2：运行10次，迭代调整数值
  printf("=== weight test(start :%d) ===\r\n", current_val);
  for (uint16_t i = 0; i < run_count; i++)
  {
    int8_t delta = RNG_SelectOpByWeight();  // 随机选择操作
    current_val += delta;                             // 调整数值
    printf("NO:%d: CV =%d , NV =%d\r\n", i+1, delta, current_val);
  }
  printf("===test over ===\r\n");
}



//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), rrt, RNG_RunTest, test ramdom);







/**************************END OF FILE************************************/











