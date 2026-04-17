

#include "./factory_iic.h"
//#include "./usart/bsp_usart.h"

#include "./i2c/hard_i2c/bsp_i2c_ee.h"
#include "./DataConvert/data_compare.h"
#include <stdio.h>
#include <string.h>
#include "./sys/bsp_systime.h"   


const IIC_FactoryInterface IIC_HARD_EEPROM_Driver = {
    .Init = I2C_EE_Init,
		.DeInit	= NULL,
    .IIC_BufferWrite = I2C_EE_BufferWrite,
    .IIC_BufferRead = I2C_EE_BufferRead
};


const  IIC_FactoryInterface* IIC_EE_currentDriver ;

void IIC_EE_DriverFactory_Register(DriverFactory* factory, const IIC_FactoryInterface* driver) {
    if(factory->count < MAX_IIC_EE_DRIVERS) {
        factory->drivers[factory->count++] = driver;
    }
}

const IIC_FactoryInterface* IIC_EE_DriverFactory_Create(DriverFactory* factory, uint8_t index) {
    return (index < factory->count) ? factory->drivers[index] : NULL;
}





void IIC_EE_Factory_Init(void) {
    DriverFactory factory = {0};
    
    // ???????
    IIC_EE_DriverFactory_Register(&factory, &IIC_HARD_EEPROM_Driver);
//    DriverFactory_Register(&factory, &DHT11_Sensor);
    
    // ???????BMP180_Sensor
//   const  IIC_FactoryInterface* 
		 IIC_EE_currentDriver = IIC_EE_DriverFactory_Create(&factory, 0);
    
    if(IIC_EE_currentDriver) {
        IIC_EE_currentDriver->Init();
//        float data = currentDriver->Read();
        // ????...
    }
    
//    // ???????DHT11_Sensor
//    SensorInterface* currentSensor = DriverFactory_Create(&factory, 1);
//    
//    if(currentSensor) {
//        currentSensor->Init();
//        float data = currentSensor->Read();
//        // ????...
//    }
}



uint8_t SaveEEpromDataFun(uint8_t** TMP_data ,uint16_t sizeof_ee, uint16_t ee_start_addr){


//uint16_t size_ee = sizeof(TMP_EEpromSettingState);
//uint16_t size_ee = 208;		
uint8_t I2c_Buf_Write[sizeof_ee];
uint8_t I2c_Buf_Read[sizeof_ee];

//memcpy(&I2c_Buf_Write,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,sizeof_ee);//Heap_Size       EQU     0x00000400  ·ñÔò´æ´¢Ê§°Ü
//			SYSTEM_DEBUG("\n ********** SaveEEpromData**********%d\n",sizeof_ee);
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,(uint8_t *)*TMP_data,sizeof_ee,"SaveEEpromDataFun \n");

	memcpy(&I2c_Buf_Write,*TMP_data,sizeof_ee);//

	SYSTEM_DEBUG_ARRAY_MESSAGE(I2c_Buf_Write,sizeof_ee,"  I2c_Buf_Write  =????? ");
	
	IIC_EE_currentDriver->IIC_BufferWrite( I2c_Buf_Write, ee_start_addr, sizeof_ee);


	delay_ms(1);
	

IIC_EE_currentDriver->IIC_BufferRead(I2c_Buf_Read, ee_start_addr, sizeof_ee);	
	
	
	SYSTEM_DEBUG_ARRAY_MESSAGE(I2c_Buf_Read,sizeof_ee,"  I2c_Buf_Read  =????? ");
	
	uint8_t i=Buffercmp(I2c_Buf_Read,I2c_Buf_Write,sizeof_ee);
	
	if(i==1){
	SYSTEM_DEBUG("\n EEpromSave error, start_addr= %04x \n",ee_start_addr);
		return 1;
	}else{
	SYSTEM_DEBUG("\n EEpromSave success, start_addr= %04x \n",ee_start_addr);
		return 0;
	}
	
//	for (uint8_t i=0; i<sizeof_ee; i++)
//	{	
//		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
//		{
//			
//			SYSTEM_ERROR("\n **********EEprom error**********\n");
//		return 1;
//		}
//   
//	}

//		return 0;
}

uint8_t ReadEEpromDataFun(uint8_t** TMP_data ,uint16_t sizeof_ee, uint16_t ee_start_addr){

	volatile uint8_t size_ee = sizeof_ee;

	
uint8_t 	I2c_Buf_Read[size_ee];
//sEE_ReadBuffer(I2c_Buf_Read, EEP_PageAddress_02, (uint16_t *)(&size_ee02));
//EEPROM_ReadBuffer(I2c_Buf_Read, EEP_PageAddress_02, size_ee02);
	
	IIC_EE_currentDriver->IIC_BufferRead(I2c_Buf_Read, ee_start_addr, size_ee);	
	
memcpy(*TMP_data,&I2c_Buf_Read,size_ee);	

//	GV_HardwareState.BatteryChargerParallel_R1A_S2A=(uint8_t)GV_EEprom.Battery_SET_R1A_S2A;
	
	SYSTEM_DEBUG("\n EEprom read, start_addr= %04x\n",ee_start_addr);
//	pp=&GV_EEprom.Battery_SET_SumlimitValue;
	SYSTEM_DEBUG_ARRAY_MESSAGE(I2c_Buf_Read,size_ee,"  EEprom read=== ");

	
	
	



}



