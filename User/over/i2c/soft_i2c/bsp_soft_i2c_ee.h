#ifndef _BSP_SOFT_I2C_EE_H
#define	_BSP_SOFT_I2C_EE_H


#include "stm32f10x.h"

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16A每页有16个字节 
 */
	

//  #define 	SIM_sEE_M24C02 
 #define 	SIM_sEE_M24C256 
 
 #if defined (SIM_sEE_M24C08)  || defined (SIM_sEE_M24C02)

#define 	SIM_SINGLE_BYTE_ADDRESS 

#elif   defined (SIM_sEE_M24C64_32)  || defined (SIM_sEE_M24C256)

#define 	SIM_DOUBLE_BYTE_ADDRESS 
#endif




#define EEPROM_DEV_ADDR			0xA0		/* 24xx02的设备地址 */

#define EEPROM_SIZE				  256			  /* 24xx02总容量 */


#if defined (SIM_sEE_M24C02)
 #define EEPROM_PAGE_SIZE           8       /* 24xx02的页面大小 */
#elif defined (SIM_sEE_M24C08)
 #define EEPROM_PAGE_SIZE           16
#elif defined (SIM_sEE_M24C64_32)
 #define EEPROM_PAGE_SIZE           32
#elif defined (SIM_sEE_M24C256)
 #define EEPROM_PAGE_SIZE           32 
#endif



uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_CheckDevice(uint8_t _Address);
void ee_Erase(void);
uint8_t ee_WaitStandby(void);






#endif /* _BSP_SIM_I2C_EE_H */



