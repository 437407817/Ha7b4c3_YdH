/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./io/bsp_io_input.h"
#include "./io/bsp_io_output.h"  

 /**
  * @brief  配置 PA0 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void IO_EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    /*开启按键GPIO口的时钟*/
    KEY1_INT_GPIO_CLK_ENABLE();
    KEY2_INT_GPIO_CLK_ENABLE();

    /* 选择按键1的引脚 */ 
    GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;
    /* 设置引脚为输入模式 */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
    /* 设置引脚不上拉也不下拉 */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* 使用上面的结构体初始化按键 */
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* 配置 EXTI 中断源 到key1 引脚、配置中断优先级*/
    HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ, 0, 0);
    /* 使能中断 */
    HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);

    /* 选择按键2的引脚 */ 
    GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;  
    /* 其他配置与上面相同 */
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* 配置 EXTI 中断源 到key1 引脚、配置中断优先级*/
    HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ, 0, 0);
    /* 使能中断 */
    HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
}







void IN_Io_ALL_GPIO_Config(void){
//Key_GPIO_Config();
IO_EXTI_Key_Config();



}


// 外部中断回调函数
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if (GPIO_Pin == GPIO_PIN_0)
//    {
//        // 这里可以添加处理 PA0 中断的具体代码
//        // 例如，通过串口打印信息
//        // 为了简化示例，这里只是简单添加一个无限循环作为示意
//        // 实际使用时需要根据需求修改
//			printf("xcdy========");
//LED1_TOGGLE;
//    }
//}


void KEY1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  //确保是否产生了EXTI Line中断
//	if(__HAL_GPIO_EXTI_GET_IT(KEY1_INT_GPIO_PIN) != RESET) 
//	{
//		// LED1 取反		
//		LED1_TOGGLE;
//    //清除中断标志位
//		__HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);     
//	}  
}

//void KEY2_IRQHandler(void)
//{
//  //确保是否产生了EXTI Line中断
//	if(__HAL_GPIO_EXTI_GET_IT(KEY2_INT_GPIO_PIN) != RESET) 
//	{
//		// LED2 取反		
//		LED2_TOGGLE;
//    //清除中断标志位
//		__HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);     
//	}  
//}




/*********************************************END OF FILE**********************/
