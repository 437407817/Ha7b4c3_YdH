#include "./SEGGER_TOOLKIT.h"
#include "./SEGGER_RTT.h"
#include "./sys/bsp_systime.h"   


#include "./SEGGER_dao.h"

//#include "./io/OUT_io/bsp_gpio_out.h" 



#include "./DataConvert/data_convert.h"
 #include "./sys/sysio.h"
#include "string.h"
#include "./sys/sys_interrupt.h"  

/**
  * @brief  切换采集通道channel for collection
  */




static
const char SeggerHelpMsg[] =
	"[Disk contorls]\n"
" ' ''?':--help menu \n"
" ' '' '+<ASCII serial datas>:-- sRTT send ASCII data to pc com_a {:space space ABCDEFG123456}\n"
" FF 03 +<HEX serial datas>:-- sRTT send HEX data to pc com_a (show HEX){:FF 03 A5 5A 05 00 FF 01 00 02}\n"
	" . - exit\n"
	"\n";


/**
  * @brief  切换采集通道channel for collection
  * @param  *i 当前通道第几个的状态地址  *openstate 1开启轮训转换采集 0 关闭所有通道
  * @retval 无
  */



void SEGGER_HostDataToUsart(void){
uint16_t asiic_NBytes = SEGGER_PCsendDataCount;
uint8_t * Hex_tmps;
//uint8_t  Hex_tmps02[50];	
	uint8_t *asiic_tmps = NULL;
	uint16_t hex_lengths=0;
	
		asiic_tmps = (uint8_t *)malloc(asiic_NBytes);
	
asiic_NBytes = SEGGER_RTT_Read(0, asiic_tmps, asiic_NBytes);//注意是&asiic_tmps[0]或asiic_tmps，不能是&asiic_tmps
	
if (asiic_NBytes!=0) {

	if(asiic_NBytes > 1){
	

	if(asiic_tmps[0] ==' '){
	
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,(uint8_t *)asiic_tmps+2,asiic_NBytes-2,"segger pc get0=");

					switch(asiic_tmps[1]){
		case ' ':
						if (HAL_UART_Transmit(&SEGGER_USART_A, (uint8_t *)asiic_tmps+2, asiic_NBytes-2, 1000) != HAL_OK) {
            // 处理发送错误的情况，比如记录日志
							SYSTEM_DEBUG("ERROR");
        };
						
				break;
		case '?':
					SEGGER_RTT_WriteString(0, SeggerHelpMsg);
		break;				
		case 'G':	
					SEGGER_PcDao_McuToUsartTransform(&SEGGER_USART_A, asiic_tmps+2, asiic_NBytes-2);break;   //与“ ”“ ”等效，向单片机内部发送，发送给串口ASIIC 12 34 56 78 90 AB CD EF
		case 'H':
					SEGGER_PcDao_UsartToMcuTransform(&SEGGER_USART_A, asiic_tmps+2, asiic_NBytes-2);break;   //02:通过模拟串口发送给mcu	
		default:break;
				}
		
	}else{
	
		SYSTEM_DEBUG("SECOND");
		
		
			}
		
	
		#if 1
		
	 //带去空格发送，包括其他非0-e字符
	AsciiRemoveSpace02(asiic_tmps,asiic_NBytes,&hex_lengths);
		


		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,(uint8_t *)asiic_tmps,hex_lengths,"segger pc get1=");//
		//SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)asiic_tmps,hex_lengths,"segger pc get0=");//
		Hex_tmps = (uint8_t *)malloc(hex_lengths);//在里面创建必须在里面释放
		AsciiToHex(asiic_tmps, Hex_tmps, hex_lengths);
		//AsciiToHex03(asiic_tmps, Hex_tmps, hex_lengths);
		//AsciiToHex02(asiic_tmps,Hex_tmps02,hex_lengths);		
		if(Hex_tmps[0]==0xFF){
		switch(Hex_tmps[1]){
			case 01:SEGGER_RTT_WriteString(0, "blocking mode (Application waits if necessary, no data lost)\r\n");
			SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);break;
			case 02:SEGGER_RTT_WriteString(0, "non-blocking mode (Application does not wait, data lost if fifo full)\r\n");
			SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);break;
//			case 01:SEGGER_PcDao_McuToUsartTransform(USARTx_A, Hex_tmps+2, hex_lengths/2-2);break;   //01:向单片机内部发送，发送给串口
			case 03:SEGGER_PcDao_McuToUsartTransform(&SEGGER_USART_A, Hex_tmps+2, hex_lengths/2-2);break;   //01:向单片机内部发送，发送给串口HEX 12 34 56 78 90 AB CD EF
		  case 04:SEGGER_PcDao_UsartToMcuTransform(&SEGGER_USART_A, Hex_tmps+2, hex_lengths/2-2);break;   //02:通过模拟串口发送给mcu
			case 05:SEGGER_PcDao_McuToUsartTransform(&SEGGER_USART_A, Hex_tmps+2, hex_lengths/2-2);break;  
		 // case 03:SEGGER_PcDao_CanToMcuTransform(Hex_tmps+2, hex_lengths/2-2);break;   //03:通过CAN发送给mcu
		 // case 04:SEGGER_PcDao_McuToCanTransform(Hex_tmps+2, hex_lengths/2-2);break;   //04:通过向单片机内部发送，发送给CAN
			case 0x09:Software_Reset();break; 
			 case 0x10:SEGGER_PcDao_Test256DataPrint();break; 


			//case 07:SEGGER_PcDao_McuToUsartTransform(USART_485_B, Hex_tmps+2, hex_lengths/2-2);break; 

			
			
			default:break;
		}
		
		}else if(Hex_tmps[0]==0xFE){
			
			
			
			segger_outfun(Hex_tmps,hex_lengths/2-2);
			

		}
	
		#endif
	
		
	  free(Hex_tmps);
    Hex_tmps = NULL;
		
	}
	 	

}
//		free(Hex_tmps);//不能放这里，会出问题
//    Hex_tmps = NULL;
free(asiic_tmps);
asiic_tmps = NULL;

}

__weak void segger_outfun(uint8_t * tmp,uint16_t retlen){

SYSTEM_INFO("You need to implement this function ( void segger_outfun(uint8_t * tmp) )./n");
SEGGER_UNUSED(tmp);

}



//void SYSTEM_DEBUG_SEGGER_CANDataToHost(CanTxMsg* ptr_cantxmsg){
//	SYSTEM_DEBUG_ARRAY_MESSAGE(ptr_cantxmsg->Data,8,"CAN_TX send message : StdId = %03X  ExtId = %08X ",ptr_cantxmsg->StdId,ptr_cantxmsg->ExtId);
//}







/*********************************************END OF FILE**********************/
