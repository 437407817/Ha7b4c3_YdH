#ifndef __FACTORY_IIC_H
#define	__FACTORY_IIC_H




#include "stm32_FH_xxx.h"
//#include "stdlib.h"	    
#include "./sys/sysio.h"









#define MAX_IIC_EE_DRIVERS	1



typedef struct {
    void (*Init)(void);
	void (* DeInit)(void);
  void (* IIC_BufferWrite)(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
  uint32_t (* IIC_BufferRead)(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
} IIC_FactoryInterface;


typedef struct {
   const  IIC_FactoryInterface* drivers[MAX_IIC_EE_DRIVERS];
    uint8_t count;
} DriverFactory;


void IIC_EE_DriverFactory_Register(DriverFactory* factory, const IIC_FactoryInterface* driver);

const IIC_FactoryInterface* IIC_EE_DriverFactory_Create(DriverFactory* factory, uint8_t index);

void IIC_EE_Factory_Init(void);


uint8_t SaveEEpromDataFun(uint8_t** TMP_data ,uint16_t sizeof_ee, uint16_t ee_start_addr);

uint8_t ReadEEpromDataFun(uint8_t** TMP_data ,uint16_t sizeof_ee, uint16_t ee_start_addr);


extern const  IIC_FactoryInterface* IIC_EE_currentDriver;






#endif /* __BSP_IIC_H */


