/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESmodes/bsp_crypt_aes.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 0 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "./crypt/bsp_encode.h"
#include "./sys/sysio.h"
#include "./sys/bsp_systime.h"  
#include "./GV_variable.h" 
#include "./crypt/crypt_aes.h" 

#include "./DataConvert/data_compare.h"
#include "./i2c/factory_iic.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CRYP_AESmodes
  * @{
  */ 




#define ChipUniqueID1   *(__IO uint32_t *)(0x1FFF7A10) // 低字节
#define ChipUniqueID2   *(__IO uint32_t *)(0x1FFF7A14)
#define ChipUniqueID3   *(__IO uint32_t *)(0x1FFF7A18) // 高字节
#define ChipSumFlashSize   *(__IO uint16_t *)(0X1FFFF7E0) //flash大小


#define STRUCT_SIZE 15

//extern str_ChipEncode GV_EEpromChipEncode;
//extern str_ChipEncode GV_SaveEEpChipEncode;


void showChipID(void){
//FLASH_Unlock();
		SYSTEM_INFO("\r\n=     ChipUniqueID: 0x%08X-%08X-%08X",ChipUniqueID3,ChipUniqueID2,ChipUniqueID1);
		SYSTEM_INFO("\r\n=     SumFlashSize: %dK\r\n=",ChipSumFlashSize);

}

#define cryptsize 20

void saveEEpromChip(void) {
    // 初始化结构体成员（这里只是示例，你可以根据实际情况修改）
	static int c=0;
	c++;
    for (int i = 0; i < 4; i++) {
        GV_SaveEEpChipEncode.chipdata.ChipID1[i] = *((__IO uint8_t *)(0x1FFF7A10) + i);
        GV_SaveEEpChipEncode.chipdata.ChipID2[i] = *((__IO uint8_t *)(0x1FFF7A14) + i);
  
    }
		    for (int i = 0; i < 3; i++) {

        GV_SaveEEpChipEncode.chipdata.ChipID3[i] = *((__IO uint8_t *)(0x1FFF7A18) + i);
    }
		
		
//		memcpy(GV_SaveEEpChipEncode.ChipID1, "ID01", 4);
//    memcpy(GV_SaveEEpChipEncode.ChipID2, "ID02", 4);
//    memcpy(GV_SaveEEpChipEncode.ChipID3, "ID03", 3);
		
		
		
    GV_SaveEEpChipEncode.chipdata.ChipisActive = c;
    GV_SaveEEpChipEncode.chipdata.overtime_day = 15;
    GV_SaveEEpChipEncode.chipdata.overtime_month = 8;
    GV_SaveEEpChipEncode.chipdata.overtime_year = 25;
//		SYSTEM_DEBUG("\n sizeof(GV_SaveEEpChipEncode)==%d\n",sizeof(GV_SaveEEpChipEncode));
// 创建临时指针并转换类型
//uint8_t* tmpPtr = (uint8_t*)&GV_SaveEEpChipEncode;
		
//		SaveEEpromDataFun(&tmpPtr ,11 ,  0x0008);
//		tmpPtr = (uint8_t*)&GV_EEpromChipEncode;
//		ReadEEpromDataFun(&tmpPtr ,11 ,  0x0008);
		
//		tmpPtr=NULL;
		
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,(uint8_t *)GV_EEpromChipEncode.chipdata.ChipID1,11,"GV_EEpromChipEncode--- \n");
//    strcpy(GV_EEpromChipEncode.code, "Sample Activation Code");

    // 定义存储结构体内容的数组
    unsigned char input[STRUCT_SIZE];
unsigned char decode_output[STRUCT_SIZE];
		
		
		
    // 将结构体内容复制到数组中
    memcpy(input, &GV_SaveEEpChipEncode, STRUCT_SIZE);

//    // 打印数组内容（用于验证）
//    for (int i = 0; i < STRUCT_SIZE; i++) {
//        SYSTEM_INFO("%02X ", input[i]);
//        if ((i + 1) % 20 == 0) {
//            SYSTEM_INFO("\r\n");
//        }
//    }
				SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,input,STRUCT_SIZE,"HEX_enCODE \n");

//		 SYSTEM_INFO("  \r\n");
		unsigned char encrypted_output[cryptsize];
		unsigned char decrypted_output[cryptsize];
		
		size_t encrypted_olen = 0;
		size_t decrypted_olen = 0;
		uint8_t cmpflage=0;
mbedtls_aes_ecb_encrypt16(input,STRUCT_SIZE,encrypted_output,&encrypted_olen);
		
//		SYSTEM_DEBUG_ARRAY_MESSAGE(encrypted_output,encrypted_olen,"encrypted_output==  length %d",encrypted_olen);
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,encrypted_output,encrypted_olen,"encrypted_output \n");
		
		memcpy(&GV_SaveEEpChipEncode.code,encrypted_output,encrypted_olen);
		
//		mbedtls_aes_ecb_dencrypt16(encrypted_output,encrypted_olen,decrypted_output,&decrypted_olen);//***
		
		
		
		uint8_t* tmpPtr = (uint8_t*)&GV_SaveEEpChipEncode.code;
		SaveEEpromDataFun(&tmpPtr ,16 ,  0x0008);
				tmpPtr = (uint8_t*)&GV_EEpromChipEncode.code;
		ReadEEpromDataFun(&tmpPtr ,16 ,  0x0008);
		
		tmpPtr=NULL;
		
		
		
		   cmpflage=Buffercmp((uint8_t*)&GV_SaveEEpChipEncode.code,(uint8_t*)&GV_EEpromChipEncode.code,16);
	 SYSTEM_DEBUG("cmp GV_SaveEEpChipEncode.code %d   encrypted_olen=%d",cmpflage,encrypted_olen);
		
		mbedtls_aes_ecb_dencrypt16((unsigned char *)GV_EEpromChipEncode.code,encrypted_olen,decrypted_output,&decrypted_olen);
		
		memcpy(&GV_EEpromChipEncode.chipdata,decrypted_output,decrypted_olen);
		
//		snprintf(GV_SaveEEpChipEncode.code, sizeof(GV_SaveEEpChipEncode.code), "PROD-2025-ABC123");//***
//		SYSTEM_DEBUG_STRING(GV_SaveEEpChipEncode.code,sizeof(GV_SaveEEpChipEncode.code));//***
		
		
		
//		SYSTEM_DEBUG_STRING_2(EM_SEGGER_PRINT,decrypted_output,decrypted_olen);
//		SYSTEM_INFO("Decrypted length: %d\r\n", (int)decrypted_olen);
//		memcpy(decrypted_output, &GV_SaveEEpChipEncode, STRUCT_SIZE);
		
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,decrypted_output,decrypted_olen,"HEX_DECODE 1");
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,(uint8_t *)&GV_EEpromChipEncode.chipdata,decrypted_olen,"HEX_DECODE 2");
//		SYSTEM_DEBUG("cmpflage %d",cmpflage);
//		SYSTEM_DEBUG_STRING_2(EM_SEGGER_PRINT,decrypted_output,decrypted_olen);
   cmpflage=Buffercmp(input,decrypted_output,decrypted_olen);
	 SYSTEM_DEBUG("cmpflage %d   decrypted_olen=%d",cmpflage,decrypted_olen);
	 
}



SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), saveEEpromChip, saveEEpromChip, saveEEpromChip);


void cmpAndSaveChipData(void) {

uint8_t cmpflage=0;
   cmpflage=Buffercmp((uint8_t *)&GV_SaveEEpChipEncode.chipdata,(uint8_t *)&GV_EEpromChipEncode.chipdata,11);
	 SYSTEM_DEBUG("cmpflage %d   ",cmpflage);
if(cmpflage==0){

		uint8_t* tmpPtr = (uint8_t*)&GV_SaveEEpChipEncode.code;
		SaveEEpromDataFun(&tmpPtr ,16 ,  0x0008);
tmpPtr=NULL;

}
}



void FirstSaveChipData(void) {

uint8_t cmpflage=0;
//		static int c=0;
	unsigned char decrypted_output[cryptsize];
	unsigned char encrypted_output[cryptsize];
	unsigned char input[STRUCT_SIZE];
			size_t decrypted_olen = 0;
		size_t encrypted_olen = 0;
//	c++;
    for (int i = 0; i < 4; i++) {
        GV_SaveEEpChipEncode.chipdata.ChipID1[i] = *((__IO uint8_t *)(0x1FFF7A10) + i);
        GV_SaveEEpChipEncode.chipdata.ChipID2[i] = *((__IO uint8_t *)(0x1FFF7A14) + i);
 
    }
		
		    for (int i = 0; i < 3; i++) {
        GV_SaveEEpChipEncode.chipdata.ChipID3[i] = *((__IO uint8_t *)(0x1FFF7A18) + i);
    }

    GV_SaveEEpChipEncode.chipdata.ChipisActive = 0;
    GV_SaveEEpChipEncode.chipdata.overtime_day = 15;
    GV_SaveEEpChipEncode.chipdata.overtime_month = 8;
    GV_SaveEEpChipEncode.chipdata.overtime_year = 25;
	
		uint8_t* tmpPtr = (uint8_t*)&GV_EEpromChipEncode.code;
		
		ReadEEpromDataFun(&tmpPtr ,16 ,  0x0008);
		
		mbedtls_aes_ecb_dencrypt16((unsigned char *)GV_EEpromChipEncode.code,16,decrypted_output,&decrypted_olen);
		
		memcpy(&GV_EEpromChipEncode.chipdata,decrypted_output,decrypted_olen);
		
   cmpflage=Buffercmp((uint8_t *)&GV_SaveEEpChipEncode.chipdata,(uint8_t *)&GV_EEpromChipEncode.chipdata,11);
		
	 SYSTEM_DEBUG("cmpflage %d   ",cmpflage);
		
if(cmpflage!=0){
	
	memcpy(input, &GV_SaveEEpChipEncode, STRUCT_SIZE);
mbedtls_aes_ecb_encrypt16(input,STRUCT_SIZE,encrypted_output,&encrypted_olen);
	memcpy(&GV_SaveEEpChipEncode.code,encrypted_output,encrypted_olen);
		uint8_t* tmpPtr = (uint8_t*)&GV_SaveEEpChipEncode.code;
		SaveEEpromDataFun(&tmpPtr ,16 ,  0x0008);
tmpPtr=NULL;
SYSTEM_DEBUG("first save chipcode data  ");
}

}


























/*********************************************END OF FILE**********************/
