

#include "./USB/usb_user_hid.h"
//#include "./usart/bsp_usart.h"








/**
  * @brief  HID application Init
  * @param  None
  * @retval None
  */
void HID_InitApplication(void)
{
  /* Configure KEY Button */
//  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  
  /* Configure Joystick in EXTI mode */
//  BSP_JOY_Init(JOY_MODE_EXTI);

  /* Initialize the LCD */
//  BSP_LCD_Init();
  
  /* Init the LCD Log module */
//  LCD_LOG_Init();
  
#ifdef USE_USB_HS 
  LCD_LOG_SetHeader((uint8_t *)" USB OTG HS HID Host");
#else
  LCD_LOG_SetHeader(" USB OTG FS HID Host");
#endif
  
  LCD_UsrLog("USB Host library started.\n"); 
  
  /* Start HID Interface */
  HID_MenuInit();
}











/**************************END OF FILE************************************/











