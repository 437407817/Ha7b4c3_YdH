#ifndef __SYSIO_H
#define __SYSIO_H	
//#include "stm32fxxx_hal.h"
#include "./SEGGER_RTT.h"
//#include "./sys/bsp_systime.h" 

#include "./SEGGER_TOOLKIT.h"
#include <stdio.h>


//#define PRINT_ALL_ON   0
//#define SEGGER_ALL_ON   1


//#include <inttypes.h> // 包含必要的头文件
///**
//  * @brief  错误信息输出.
//  */
//#if(SEGGER_ALL_ON)
//#define SEGGER__ERROR_ON                  1	
//#define SEGGER__INFO_ON                  1	
//#define SEGGER__DEBUG_ON                  1		
//#else
//#define SEGGER_SET
//#endif

//#if(PRINT_ALL_ON)
//#define PRINT_ERROR_ON                  1	
//#define PRINT_INFO_ON                  1	
//#define PRINT_DEBUG_ON                  1		
//#else
//#define PRINT_SET
//#endif


#define  SEGGER_RTT_ALL_ON    1
#define   PRINT_ALL_ON   0


enum E_PRINT_MOD{
EM_NO_PRINT=0x00,	
EM_SEGGER_PRINT=0x01,
EM_SERIAL_PRINT=0x02,
EM_EASYLOG_PRINT=0x04,
};

//#define  SYSTEM_print(fmt,arg...)  do{printf}while(0)


//SEGGER打印
#define SEGGER_RTT_ERROR_ON             SEGGER_RTT_ALL_ON
//串口打印
#define PRINT_ERROR_ON                  PRINT_ALL_ON			


#define SEGGER_RTT_ADDRESS_ON     1


#define SYSTEM_ERROR(fmt,arg...)          do{\
                                          if(SEGGER_RTT_ERROR_ON){\
																					SEGGER_RTT_SetTerminal(2);\
                                          SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_RED"<<-SYSTEM_ERROR->> [%s][%d]"fmt"\n" ,__FILE__,__LINE__, ##arg);}\
										                      if(PRINT_ERROR_ON)\
										                      printf("<<-SYSTEM_ERROR->> [%s] [%d]"fmt"\n",__FILE__,__LINE__, ##arg);\
                                          }while(0)
//RTT_CTRL_RESET,RTT_CTRL_BG_BRIGHT_RED,RTT_CTRL_TEXT_BRIGHT_WHITE,
/**
  * @brief  提示信息输出.
  */

#define SEGGER_RTT_INFO_ON             SEGGER_RTT_ALL_ON

																					
																				
#define PRINT_INFO_ON                 PRINT_ALL_ON


//overtime																					
//#define SYSTEM_INFO(fmt,arg...)          do{\
//                                          if(SEGGER_RTT_INFO_ON){\
//																					SEGGER_RTT_SetTerminal(1);\
//																								if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,"[%s]",__FILE__);};\
//                                          SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_BLUE"<<-SYSTEM-INFO->> [%d]"fmt"\n" ,__LINE__, ##arg);}\
//										                      if(PRINT_INFO_ON)\
//										                      printf("<<-SYSTEM-INFO->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                          }while(0)		

//#define SYSTEM_INFO(fmt,arg...)          do{\
//                                          if(SEGGER_RTT_INFO_ON){\
//																					SEGGER_RTT_SetTerminal(1);\
//                                          SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_BLUE""fmt"" ,##arg);}\
//										                      if(PRINT_INFO_ON)\
//										                      printf(""fmt"", ##arg);\
//                                          }while(0)	


#define SYSTEM_INFO_MOD                 EM_SEGGER_PRINT                                    //(EM_NO_PRINT|EM_SEGGER_PRINT)

#define SYSTEM_INFO(fmt,arg...)        SYSTEM_INFO_2(SYSTEM_INFO_MOD,fmt,##arg) 




#define SYSTEM_INFO_2(EV_MOD,fmt,arg...)          do{\
                                          if(EM_SEGGER_PRINT==(EM_SEGGER_PRINT&(EV_MOD))){\
																					SEGGER_RTT_SetTerminal(1);\
                                          SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN fmt  ,##arg);}\
										                      if(EM_SERIAL_PRINT==(EM_SERIAL_PRINT&(EV_MOD)))\
										                      printf(fmt, ##arg);\
                                          }while(0)				

																					
																					
/**
  * @brief  调试信息输出.
  */
																		
#define SEGGER_RTT_DEBUG_ON             SEGGER_RTT_ALL_ON
																		

#define PRINT_DEBUG_ON                  PRINT_ALL_ON


//#define SYSTEM_DEBUG3(fmt,arg...)    SYSTEM_DEBUG(fmt,##arg) 
																					
//#define SYSTEM_DEBUG(fmt,arg...)          do{\
//                                          if(SEGGER_RTT_DEBUG_ON){\
//																					SEGGER_RTT_SetTerminal(0);\
//																									if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,"[%s]",__FILE__);};\
//                                          SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_MAGENTA" [%d] <<-SYSTEM-DEBUG->>"fmt"\n" ,__LINE__, ##arg);}\
//										                      if(PRINT_DEBUG_ON)\
//										                      printf("<<-SYSTEM-DEBUG->> [%s][%d]   "fmt"\n",__FILE__,__LINE__, ##arg);\
//                                          }while(0)								

//无法打印大数，
#define SYSTEM_DEBUG(fmt,arg...)         SYSTEM_DEBUG_2(EM_SEGGER_PRINT,fmt,##arg)


#define SYSTEM_DEBUG_2(EV_MOD,fmt,arg...)          do{\
                                          if(EM_SEGGER_PRINT==(EM_SEGGER_PRINT&(EV_MOD))){\
																					SEGGER_RTT_SetTerminal(0);\
																					if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG->>",__FILE__,__LINE__);};\
                                          SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt"\n" ,##arg);}\
										                      if(EM_SERIAL_PRINT==(EM_SERIAL_PRINT&(EV_MOD)))\
										                      printf("<<-SYSTEM-DEBUG->> [%s][%d]   "fmt"\n",__FILE__,__LINE__, ##arg);\
                                          }while(0)		

																					
#define SYSTEM_BIGNUM_HEX(EV_MOD,big,fmt,arg...)      if(EV_MOD==1){SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_BLACK" "fmt" " ,##arg);}\
																					SEGGER_RTT_printf(0, "0x%08X_%08X\n", (uint32_t)(big >> 32), (uint32_t)big)																			
#define SYSTEM_BIGNUM_DEC(EV_MOD,big,fmt,arg...)         if(EV_MOD==1){SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_BLACK" "fmt" ",##arg);}\
																					SEGGER_RTT_printf(0, "%u%09u\n",(uint32_t)(big / 1000000000ULL),(uint32_t)(big % 1000000000ULL))

																					
																					
															
//#define SYSTEM_DEBUG_BINARY(num,fmt,arg...)          do{\
//																					char binary_str[37];binary_str[36] = '\0';uint8_t index = 35;uint32_t num2=num;\
//																					for (int i = 0; i < 32; i++) {\
//																					binary_str[index--] = (num2 & 1)? '1' : '0';\
//																					if ((i + 1) % 8 == 0 && i!= 31) {\
//																					binary_str[index--] = ' ';\
//																					};\
//																					(num2=(num2)>>(1));\
//																					}\
//																					if(SEGGER_RTT_DEBUG_ON){\
//																						SEGGER_RTT_SetTerminal(0);\
//																						if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,"[%s]",__FILE__);};\
//																						SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_MAGENTA" [%d] <<-SYSTEM-DEBUG->>"fmt" " ,__LINE__, ##arg);\
//																						SEGGER_RTT_printf(0,"0b %s\n", &binary_str[index + 1]);\
//																					}\
//																					if(PRINT_DEBUG_ON)\
//										                      printf("<<-SYSTEM-DEBUG->> [%s][%d] "fmt"0b %s\\n",__FILE__,__LINE__, ##arg,&binary_str[index + 1]);\
//                                          }while(0)	



#define SYSTEM_DEBUG_BINARY(num,fmt,arg...)        SYSTEM_DEBUG_BINARY_2(EM_SEGGER_PRINT,num,fmt,##arg)



#define SYSTEM_DEBUG_BINARY_2(EV_MOD,num,fmt,arg...)          do{\
																					char binary_str[37];binary_str[36] = '\0';uint8_t index = 35;uint32_t num2=num;\
																					for (int i = 0; i < 32; i++) {\
																					binary_str[index--] = (num2 & 1)? '1' : '0';\
																					if ((i + 1) % 8 == 0 && i!= 31) {\
																					binary_str[index--] = ' ';\
																					};\
																					(num2=(num2)>>(1));\
																					}\
																					if(EM_SEGGER_PRINT==(EM_SEGGER_PRINT&(EV_MOD))){\
																						SEGGER_RTT_SetTerminal(0);\
																						if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_BINARY->>",__FILE__,__LINE__);};\
																						SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt" 0b:", ##arg);\
																						SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN" %s\n", &binary_str[index + 1]);\
																					}\
																					if(EM_SERIAL_PRINT==(EM_SERIAL_PRINT&(EV_MOD)))\
										                      printf("<<-SYSTEM-DEBUG->> [%s][%d] "fmt"0b %s\\n",__FILE__,__LINE__, ##arg,&binary_str[index + 1]);\
                                          }while(0)	
																					
																					
																					

#define PRINT_HEX                   1																					
#define PRINT_ASCII                 0				



////SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)&GV_get_vol_485_232_Bigdata3.Bat_WorkStatus,sizeof(GV_get_vol_real_data),"xxxx");

#define SYSTEM_DEBUG_ARRAY_MESSAGE(array, num,fmt,arg...)      	SYSTEM_DEBUG_ARRAY_MESSAGE_2(EM_SEGGER_PRINT,array, num,fmt,##arg)												
																					

#define SYSTEM_DEBUG_ARRAY_MESSAGE_2(EV_MOD,array, num,fmt,arg...)      do{\
																									 int32_t i;\
																									 uint8_t* a = array;\
																										if(EM_SERIAL_PRINT==(EM_SERIAL_PRINT&(EV_MOD)))\
																									 {\
																											printf("\r\n[%s]<<-PRINT_DEBUG-ARRAY->> [%d] "fmt"\n",__FILE__,__LINE__, ##arg);\
printf("\r\n 01-02-03-04-05-06-07-08-09-10---11-12-13-14-15-16-17-18-19-20---21-22-23-24-25-26-27-28-29-30  \r\n ");\
																											for (i = 0; i < (num); i++)\
																											{\
																													printf("%02x ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																													if ((i + 1 ) %30 == 0)\
																													{\
																															printf("\r\n ");\
																													}else{\
																													printf("  ");\
																													}\
																													}\
																											}\
																											printf("\r\n");\
																									};\
																										if(EM_SEGGER_PRINT==(EM_SEGGER_PRINT&(EV_MOD)))\
																									 {\
																									  if(PRINT_HEX)\
																									 {\
																										 SEGGER_RTT_SetTerminal(0);\
																										 if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_ARRAY_HEX->>",__FILE__,__LINE__);};\
																										 SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt"", ##arg);\
SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_RED"\n 01-02-03-04-05-06-07-08-09-10---11-12-13-14-15-16-17-18-19-20---21-22-23-24-25-26-27-28-29-30---31-32-33-34-35-36-37-38-39-40 \n ");\
																										SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN);\
																											for (i = 0; i < (num); i++)\
																											{\
																													SEGGER_RTT_printf(0,"%02x ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																													if ((i + 1 ) %40 == 0)\
																													{\
																															SEGGER_RTT_printf(0,"\n ");\
																													}else{\
																													SEGGER_RTT_printf(0,"  ");\
																													}\
																													}\
																									 }\
																									 	SEGGER_RTT_printf(0,"\n");\
																									 }\
																									 if(PRINT_ASCII)\
																									 {\
																										 SEGGER_RTT_SetTerminal(0);\
																										 			if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_ARRAY_ASCII->>",__FILE__,__LINE__);};\
																										 SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt"", ##arg);\
SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_RED"\n 01-02-03-04-05-06-07-08-09-10---11-12-13-14-15-16-17-18-19-20---21-22-23-24-25-26-27-28-29-30---31-32-33-34-35-36-37-38-39-40 \n ");\
																										SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN);\
																											for (i = 0; i < (num); i++)\
																											{\
																													SEGGER_RTT_printf(0,"%c ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																													if ((i + 1 ) %40 == 0)\
																													{\
																															SEGGER_RTT_printf(0,"\n ");\
																													}else{\
																													SEGGER_RTT_printf(0,"  ");\
																													}\
																													}\
																									 }\
																									 	SEGGER_RTT_printf(0,"\n");\
																											};\
																									 };\
																								     a =NULL; }while(0);    																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 
																										 

//0间断HEX16字符串，1间断ASIIC，2连续HEX16字符串																										 
#define SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(HEX_or_ASCII_i,array, num,fmt,arg...)      do{\
																									 int32_t i;\
																									 uint8_t* a = array;\
																										if(PRINT_DEBUG_ON)\
																									 {\
																											printf("\n <<-PRINT_DEBUG-ARRAY->> [%d] "fmt"\n",__LINE__, ##arg);\
																											for (i = 0; i < (num); i++)\
																											{\
																													printf("%02x  ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																															printf("\n");\
																													}\
																											}\
																											printf("\n");\
																									};\
																										if(SEGGER_RTT_DEBUG_ON)\
																									 {\
																									  if(0==HEX_or_ASCII_i)\
																									 {\
																										 SEGGER_RTT_SetTerminal(0);\
																										 			if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_ARRAY_HEX->>",__FILE__,__LINE__);};\
																										 SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt"", ##arg);\
SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_RED"\n 01-02-03-04-05-06-07-08-09-10---11-12-13-14-15-16-17-18-19-20---21-22-23-24-25-26-27-28-29-30---31-32-33-34-35-36-37-38-39-40 \n ");\
																										SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN);\
																											for (i = 0; i < (num); i++)\
																											{\
																													SEGGER_RTT_printf(0,"%02x ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																													if ((i + 1 ) %40 == 0)\
																													{\
																															SEGGER_RTT_printf(0,"\n ");\
																													}else{\
																													SEGGER_RTT_printf(0,"  ");\
																													}\
																													}\
																									 }\
																									 	SEGGER_RTT_printf(0,"\n");\
																									 }\
																									 else if(1==HEX_or_ASCII_i)\
																									 {\
																										 SEGGER_RTT_SetTerminal(0);\
																										 			if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_ARRAY_ASCII->>",__FILE__,__LINE__);};\
																										 SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt"", ##arg);\
SEGGER_RTT_printf(0,RTT_CTRL_BG_BRIGHT_WHITE""RTT_CTRL_TEXT_BRIGHT_RED"\n 01-02-03-04-05-06-07-08-09-10---11-12-13-14-15-16-17-18-19-20---21-22-23-24-25-26-27-28-29-30---31-32-33-34-35-36-37-38-39-40 \n ");\
																										SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN);\
																											for (i = 0; i < (num); i++)\
																											{\
																													SEGGER_RTT_printf(0,"%c  ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																													if ((i + 1 ) %40 == 0)\
																													{\
																															SEGGER_RTT_printf(0,"\n ");\
																													}else{\
																													SEGGER_RTT_printf(0,"  ");\
																													}\
																													}\
																									 }\
																									 	SEGGER_RTT_printf(0,"\n");\
																											}\
																									 else	if(2==HEX_or_ASCII_i)\
																									 {\
																										 SEGGER_RTT_SetTerminal(0);\
																										 			if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_ARRAY_HEX->>",__FILE__,__LINE__);};\
																										 SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED" "fmt" ", ##arg);\
																										SEGGER_RTT_printf(0,RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN);\
																											for (i = 0; i < (num); i++)\
																											{\
																													SEGGER_RTT_printf(0,"%02x", (a)[i]);\
																													if ((i + 1 ) %40 == 0)\
																													{\
																															SEGGER_RTT_printf(0,"\n ");\
																													}\
																									 }\
																									 	SEGGER_RTT_printf(0,"\n");\
																									 }\
																									 };\
																								     a =NULL; }while(0);         				









//#define  print_hex(label_t, data_t, len)          do{\
//																											 char* label = (char* )label_t;char* data = (char* )data_t;\
//																												printf("%s (length %zu): ", label, len);\
//																												for (size_t i = 0; i < (len); i++) {\
//																													printf("%02X ", data[i]);\
//																													}\
//																													printf("\r\n");\
//																											}while(0);

																								
																											
																											
																											
																											
																											
																											
																											
																											

//#define  SYSTEM_DEBUG_STRING(str, length)          do{\
//																										char temp_str[256];\
//																										for (int i = 0; i < length; i++) {\
//																										temp_str[i] = str[i];\
//																										}\
//																										temp_str[length] = '\0';\
//																										SEGGER_RTT_printf(0, "The decrypted output first %d characters: %s\n", length, temp_str);\
//																										}while(0);
																											
																											
#define  SYSTEM_DEBUG_STRING(str, length) 				SYSTEM_DEBUG_STRING_2(EM_SEGGER_PRINT,str, length) 																							

#define  SYSTEM_DEBUG_STRING_2(EV_MOD,str, length)          do{\
																											if(EM_SEGGER_PRINT==(EM_SEGGER_PRINT&(EV_MOD)))\
																									 {\
																										char temp_str[256];\
																										for (int i = 0; i < length; i++) {\
																										temp_str[i] = str[i];\
																										}\
																										temp_str[length] = '\0';\
																										SEGGER_RTT_SetTerminal(0);\
																										if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-DEBUG_STRING->>",__FILE__,__LINE__);};\
																										SEGGER_RTT_printf(0, RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_RED"str chars:");\
																										SEGGER_RTT_printf(0, RTT_CTRL_BG_BLACK RTT_CTRL_TEXT_BRIGHT_GREEN" %s\r\n", temp_str);\
																									};\
																									 	if(EM_SERIAL_PRINT==(EM_SERIAL_PRINT&(EV_MOD)))\
																									 {\
																										 printf("str chars: %.*s\r\n", (int)length, str);\
																									 }}while(0);


#define Error_Handler()															do{\
																									 SEGGER_RTT_SetTerminal(2);\
																										if(SEGGER_RTT_ADDRESS_ON){SEGGER_RTT_printf(0,RTT_CTRL_BG_WHITE RTT_CTRL_TEXT_BRIGHT_BLACK"[%s] [%d] <<-Error_Handler->>",__FILE__,__LINE__);};\
																										}while(0);





																									
//#define 	SYSTEM_DEBUG_SEGGER_CANDataToHost(CanTxMsg* ptr_cantxmsg)						SYSTEM_DEBUG_ARRAY_MESSAGE(ptr_cantxmsg->Data,8,"CAN_TX send message : StdId = %03X  ExtId = %08X ",ptr_cantxmsg->StdId,ptr_cantxmsg->ExtId);						
																					
																					
#define 	VOFA_Chart_OneCurve_d(x)    								  printf("Chart01:%d\n", x); 																																			
#define 	VOFA_Chart_OneCurve_f(x)    								  printf("Chart01:%f\n", x); 																			
#define 	VOFA_Chart_TwoCurve_f(x,y)    								printf("Chart02:%f, %f\n", x, y); 			
																											
																									
#define 	SYSTEM_DEBUG_SEGGER_CANDataToHost(ptr_cantxmsg)						SYSTEM_DEBUG_ARRAY_MESSAGE(ptr_cantxmsg->Data,8,"CAN_TX send message : StdId = %03X  ExtId = %08X ",ptr_cantxmsg->StdId,ptr_cantxmsg->ExtId);						
																					
																		
																					

																					
																					
																					
																					
																					
																					
																					
#endif
