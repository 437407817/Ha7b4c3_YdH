/* drivers/input/touchscreen/gt9xx.h
 * 
 * 2010 - 2013 Goodix Technology.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be a reference 
 * to you, when you are integrating the GOODiX's CTP IC into your system, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 * 
 */

#ifndef _FT5206_CT_IIC_H
#define _FT5206_CT_IIC_H

#include "stm32_FH_xxx.h"
#include <stdbool.h> 
#ifndef NULL
  #define NULL        0
#endif




//==================== STM32H743 通用 IO 口地址映射 ====================
// 输出数据寄存器 ODR 基地址 + 0x14 = 20
#define GPIOA_ODR_Addr    (GPIOA_BASE+20)
#define GPIOB_ODR_Addr    (GPIOB_BASE+20)
#define GPIOC_ODR_Addr    (GPIOC_BASE+20)
#define GPIOD_ODR_Addr    (GPIOD_BASE+20)
#define GPIOE_ODR_Addr    (GPIOE_BASE+20)
#define GPIOF_ODR_Addr    (GPIOF_BASE+20)
#define GPIOG_ODR_Addr    (GPIOG_BASE+20)
#define GPIOH_ODR_Addr    (GPIOH_BASE+20)
#define GPIOI_ODR_Addr    (GPIOI_BASE+20)
#define GPIOJ_ODR_Addr    (GPIOJ_BASE+20)  // 修正笔误：ADDr → Addr
#define GPIOK_ODR_Addr    (GPIOK_BASE+20)  // 修正笔误：ADDr → Addr

// 输入数据寄存器 IDR 基地址 + 0x10 = 16
#define GPIOA_IDR_Addr    (GPIOA_BASE+16)
#define GPIOB_IDR_Addr    (GPIOB_BASE+16)
#define GPIOC_IDR_Addr    (GPIOC_BASE+16)
#define GPIOD_IDR_Addr    (GPIOD_BASE+16)
#define GPIOE_IDR_Addr    (GPIOE_BASE+16)
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)
#define GPIOG_IDR_Addr    (GPIOG_BASE+16)
#define GPIOH_IDR_Addr    (GPIOH_BASE+16)
#define GPIOI_IDR_Addr    (GPIOI_BASE+16)
#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16)
#define GPIOK_IDR_Addr    (GPIOK_BASE+16)

//==================== 位带操作核心宏 (H7/F4 通用) ====================
// 位带地址转换公式
#define BITBAND(addr, bitnum)  ((addr & 0xF0000000)+0x02000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
#define BIT_ADDR(addr, bitnum) (*(volatile unsigned long *)BITBAND(addr,bitnum))

//==================== IO 口操作宏 (单引脚操作, n<16) ====================
// GPIOA
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)
// GPIOB
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)
// GPIOC
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)
// GPIOD
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)
// GPIOE
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)
// GPIOF
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)
// GPIOG
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)
// GPIOH
#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)
// GPIOI
#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)
// GPIOJ
#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)
#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)
// GPIOK
#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)
#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)







//==================== STM32H743 I2C GPIO 配置 ====================
// GPIO 模式配置 (H7 MODER 寄存器：00=输入 01=通用输出 10=复用 11=模拟)
#define CT_SDA_IN()    {GPIOI->MODER &= ~(3U << (3*2)); GPIOI->MODER |= 0U << (3*2);}  // PI3 输入模式
#define CT_SDA_OUT()   {GPIOI->MODER &= ~(3U << (3*2)); GPIOI->MODER |= 1U << (3*2);}  // PI3 输出模式

// IO 操作函数 (H7 标准位带操作 / 寄存器操作)
#define CT_IIC_SCL     PHout(6)    // SCL  PH6
#define CT_IIC_SDA     PIout(3)    // SDA  PI3
#define CT_READ_SDA    PIin(3)     // 读取 PI3 输入电平



//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口				 
void CT_IIC_Start(void);				//发送IIC开始信号
void CT_IIC_Stop(void);	  				//发送IIC停止信号
void CT_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t CT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CT_IIC_Ack(void);					//IIC发送ACK信号
void CT_IIC_NAck(void);					//IIC不发送ACK信号
























#endif /* _GOODIX_GT9XX_H_ */
