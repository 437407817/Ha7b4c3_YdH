#ifndef __BSP_IIC_H
#define	__BSP_IIC_H




#include "stm32f10x.h"
#include "stdlib.h"	    
#include "./sys/sysio.h"
/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define IIC1_OWN_ADDRESS7      0X0A   







/* STM32 I2C 快速模式 */
#define IIC1_Speed              100000

#define  OLED_ADDRESS           0x78

/*I2C接口*/
#define IIC1                          I2C1
#define IIC1_CLK                      RCC_APB1Periph_I2C1

#define IIC1_CLK_INIT								RCC_APB1PeriphClockCmd

#define IIC_STM32F10X

#ifdef IIC_STM32F10X

  #define IIC1_CLK_INIT								RCC_APB1PeriphClockCmd
#else  //STM32F4配置
  #define IIC1_CLK_INIT								RCC_APB1PeriphClockCmd
#endif

#define IIC1_SCL_PIN                  GPIO_Pin_6                 
#define IIC1_SCL_GPIO_PORT            GPIOB                       
#define IIC1_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
#define IIC1_SCL_SOURCE               GPIO_PinSource6
#define IIC1_SCL_AF                   GPIO_AF_I2C1

#define IIC1_SDA_PIN                  GPIO_Pin_7                  
#define IIC1_SDA_GPIO_PORT            GPIOB                       
#define IIC1_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
#define IIC1_SDA_SOURCE               GPIO_PinSource7
#define IIC1_SDA_AF                   GPIO_AF_I2C1

#define IIC1_WORD_ADDR_SIZE           8






#define IIC_RCC_Periph RCC_APB2Periph_GPIOB
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    		






/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))



/*信息输出*/
#define IIC_DEBUG_ON         0

#define IIC_INFO(fmt,arg...)           SYSTEM_INFO("<<-IIC-INFO->> "fmt"\n",##arg)
#define IIC_ERROR(fmt,arg...)          SYSTEM_ERROR("<<-IIC-ERROR->> "fmt"\n",##arg)
#define IIC_DEBUG(fmt,arg...)          do{\
                                          if(IIC_DEBUG_ON)\
                                          SYSTEM_DEBUG("<<-IIC-DEBUG->> "fmt"\n",##arg);\
                                          }while(0)

																					
																					
uint32_t I2C_START(void);
uint32_t I2C_ByteToWriteInsert(u8 WriteAddr);
uint32_t I2C_AddressToWriteInsert(u8 SlaverAddress);
void I2C_STOP(void);

void IIC_ConfigInit(void);

uint32_t I2C_ByteToWriteOver(u8 SlaverAddress, u16 InsideAddrOrCMD,u8 Data);
uint32_t I2C_BytesToWriteOver(u8 SlaverAddress, u16 InsideAddrOrCMD,u8 *pData);













#endif /* __BSP_IIC_H */


