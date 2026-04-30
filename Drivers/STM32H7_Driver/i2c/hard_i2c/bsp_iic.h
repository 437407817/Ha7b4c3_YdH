#ifndef __BSP_IIC_H
#define	__BSP_IIC_H




#include "./stm32_FH_xxx_hal.h"
//#include "stdlib.h"	    
#include "./sys/sysio.h"







/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS7      0X0A   

#define I2Cx_a                             I2C1
#define I2Cx_a_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_a_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_a_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_a_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_a_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx_a Pins */
#define I2Cx_a_SCL_PIN                    GPIO_PIN_6
#define I2Cx_a_SCL_GPIO_PORT              GPIOB
#define I2Cx_a_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_a_SDA_PIN                    GPIO_PIN_7
#define I2Cx_a_SDA_GPIO_PORT              GPIOB
#define I2Cx_a_SDA_AF                     GPIO_AF4_I2C1

//--------------

/* AT24C01/02每页有8个字节 */
//#define EEPROM_PAGESIZE           8
#define EEPROM_PAGESIZE 	   8
/* AT24C04/08A/16A每页有16个字节 */
//#define EEPROM_PAGESIZE           16			

#define EEPROM_I2C_FORCE_RESET()      __HAL_RCC_I2C4_FORCE_RESET()
#define EEPROM_I2C_RELEASE_RESET()    __HAL_RCC_I2C4_RELEASE_RESET()
/* I2C interrupt requests */
#define EEPROM_I2C_EV_IRQn            I2C4_EV_IRQn
#define EEPROM_I2C_ER_IRQn            I2C4_ER_IRQn
#define EEPROM_I2C_EV_IRQHandler	    I2C4_EV_IRQHandler
#define EEPROM_I2C_ER_IRQHandler	    I2C4_ER_IRQHandler

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS7      0X0A   

/*I2C接口*/
#define EEPROM_I2C                          I2C4
#define EEPROM_I2C_CLK_ENABLE()             __HAL_RCC_I2C4_CLK_ENABLE()
//#define RCC_PERIPHCLK_I2Cx               	  RCC_PERIPHCLK_I2C4

#define EEPROM_I2C_SCL_PIN                  GPIO_PIN_8                 
#define EEPROM_I2C_SCL_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SCL_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()
#define EEPROM_I2C_SCL_AF                   GPIO_AF6_I2C4

#define EEPROM_I2C_SDA_PIN                  GPIO_PIN_9                 
#define EEPROM_I2C_SDA_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SDA_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()
#define EEPROM_I2C_SDA_AF                   GPIO_AF6_I2C4






/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0 /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

#define EEPROM_ADDRESS        0xA0


extern I2C_HandleTypeDef  I2C_A_Handle; 



void I2C_EE_Init(void);









//----------------------------------------
/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
//#define IIC1_OWN_ADDRESS7      0X0A   







///* STM32 I2C 快速模式 */
//#define IIC1_Speed              100000

//#define  OLED_ADDRESS           0x78

///*I2C接口*/
//#define IIC1                          I2C1
//#define IIC1_CLK                      RCC_APB1Periph_I2C1

//#define IIC1_CLK_INIT								RCC_APB1PeriphClockCmd

//#define IIC_STM32F10X

//#ifdef IIC_STM32F10X

//  #define IIC1_CLK_INIT								RCC_APB1PeriphClockCmd
//#else  //STM32F4配置
//  #define IIC1_CLK_INIT								RCC_APB1PeriphClockCmd
//#endif

//#define IIC1_SCL_PIN                  GPIO_Pin_6                 
//#define IIC1_SCL_GPIO_PORT            GPIOB                       
//#define IIC1_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
//#define IIC1_SCL_SOURCE               GPIO_PinSource6
//#define IIC1_SCL_AF                   GPIO_AF_I2C1

//#define IIC1_SDA_PIN                  GPIO_Pin_7                  
//#define IIC1_SDA_GPIO_PORT            GPIOB                       
//#define IIC1_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
//#define IIC1_SDA_SOURCE               GPIO_PinSource7
//#define IIC1_SDA_AF                   GPIO_AF_I2C1

//#define IIC1_WORD_ADDR_SIZE           8






    		






///*等待超时时间*/
//#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
//#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))



///*信息输出*/
//#define IIC_DEBUG_ON         0

//#define IIC_INFO(fmt,arg...)           SYSTEM_INFO("<<-IIC-INFO->> "fmt"\n",##arg)
//#define IIC_ERROR(fmt,arg...)          SYSTEM_ERROR("<<-IIC-ERROR->> "fmt"\n",##arg)
//#define IIC_DEBUG(fmt,arg...)          do{\
//                                          if(IIC_DEBUG_ON)\
//                                          SYSTEM_DEBUG("<<-IIC-DEBUG->> "fmt"\n",##arg);\
//                                          }while(0)

																					
																					
//uint32_t I2C_START(void);
//uint32_t I2C_ByteToWriteInsert(u8 WriteAddr);
//uint32_t I2C_AddressToWriteInsert(u8 SlaverAddress);
//void I2C_STOP(void);

//void IIC_ConfigInit(void);

//uint32_t I2C_ByteToWriteOver(u8 SlaverAddress, u16 InsideAddrOrCMD,u8 Data);
//uint32_t I2C_BytesToWriteOver(u8 SlaverAddress, u16 InsideAddrOrCMD,u8 *pData);













#endif /* __BSP_IIC_H */


