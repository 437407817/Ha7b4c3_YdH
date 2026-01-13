/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Src/usbd_desc.c
  * @author  MCD Application Team
  * @brief   This file provides the USBD descriptors and string formatting method.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usbd_init.h"

#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"
#include "./sys/sysio.h"
extern USBD_HandleTypeDef USBD_Device;

extern volatile uint8_t g_device_state;             /* USB连接 情况 */


void BSP_usbd_init(void)
{
  /* STM32F446xx HAL library initialization */
//  HAL_Init();
//  
//  /* Configure the system clock to 180 MHz */
//  SystemClock_Config();
  
  /* Init Device Library */
  USBD_Init(&USBD_Device, &VCP_Desc, 0);
  
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
  
  /* Add CDC Interface Class */
  USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
  
  /* Start Device Process */
  USBD_Start(&USBD_Device);
  
  /* Run Application (Interrupt mode) */
//  while (1)
//  {
//  }
}




void BSP_usbd_test(void)
{
    uint16_t len;
	    uint16_t times = 0;
	    uint8_t usbstatus = 0;
	
	    while (1)
    {
        if (usbstatus != g_device_state)                                          /* USB连接状态发生了改变 */
        {
            usbstatus = g_device_state;                                           /* 记录新的状态 */

            if (usbstatus == 1)
            {
//                lcd_show_string(30, 110, 200, 16, 16, "USB Connected    ", BLUE); /* 提示USB连接成功 */
//                LED1(0);                                                          /* 绿灯亮 */
							SYSTEM_DEBUG("USB Connected");
            }
            else
            {
							SYSTEM_DEBUG("USB disConnected");
//                lcd_show_string(30, 110, 200, 16, 16, "USB disConnected ", BLUE); /* 提示USB断开 */
//                LED1(1);                                                          /* 绿灯灭 */
            }
        }

        if (g_usb_usart_rx_sta & 0x8000)
        {
            len = g_usb_usart_rx_sta & 0x3FFF;                                    /* 得到此次接收到的数据长度 */
            usb_printf("\r\n您发送的消息长度为:%d\r\n\r\n", len);
            cdc_vcp_data_tx(g_usb_usart_rx_buffer, len);
            usb_printf("\r\n\r\n");                                               /* 插入换行 */
            g_usb_usart_rx_sta = 0;
        }
        else
        {
            times++;

            if (times % 5000 == 0)
            {
                usb_printf("\r\nSTM32开发板USB虚拟串口实验\r\n");
                usb_printf("正点原子@ALIENTEK\r\n\r\n");
            }

            if (times % 200 == 0)
            {
                usb_printf("请输入数据,以回车键结束\r\n");
            }

            if (times % 30 == 0)
            {
//                LED0_TOGGLE();         /* 闪烁LED,提示系统正在运行 */
            }

            delay_ms(10);
        }
    }
	
	


}



