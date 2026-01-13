/**
  ******************************************************************************
  * @file    bsp_test-all.c
  * @author  fire
  * @version V1.0
  * @date    2023-05-15
  * @brief   应用函数接口
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "./WindowCmd/exe_cmd.h"   

#include "ff.h"
#include "diskio.h"	
 #include "./sys/sysio.h"
#include "./usart/bsp_usart.h"
//******************************
//#include "nr_micro_shell.h"


/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs            (C)ChaN, 2018    */
/*----------------------------------------------------------------------*/

#include <string.h>
//#include "STM32F100.h"
//#include "uart_stm32f1.h"
#include "./sys/sysio.h"
//#include "nr_micro_shell.h"
#include "./usart/que.h"

  #if 0
#define MENU_SIZE 4
const char* menu[MENU_SIZE] = {"Option 1", "Option 2", "Option 3", "Option 4"};
//int current_selection = 0;



//uint8_t esc_seq[3];
//uint8_t esc_index = 0;




typedef enum {
    ESC_IDLE, // 等待ESC字符
    ESC_RECEIVED, // 已接收到ESC字符
    BRACKET_RECEIVED // 已接收到'['字符
} esc_state_t;

//esc_state_t esc_state = ESC_IDLE; // 初始状态

typedef struct
{
  uint16_t current_selection;            
  esc_state_t esc_state;                                   

	uint16_t is_choice_enter;
	
}MENU_TypeDef;

MENU_TypeDef menu_a={.current_selection=0,.esc_state=ESC_IDLE};





void display_menu();



creat_que(rx_que2, 10);

 uint8_t ch2;



#if 1

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        //uint8_t data = USART_ReceiveData(USART1);
			 InQue(rx_que2, USART_ReceiveData(USARTx_A));
//shell(USART_ReceiveData(USART1));
			//USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
			//SYSTEM_INFO("xxx");
    }
    
}

#endif




//        if (ansi_get_char(c, &nr_ansi) == NR_SHELL_END_CHAR) \
//        {                                                    \
//            shell_parser(&nr_shell, nr_ansi.current_line);   \
//            ansi_clear_current_line(&nr_ansi);               \
//        }    


void dispose_menu() {
    //unsigned char ch;
unsigned char  data;  
	shell(data);
	SYSTEM_INFO("dispose_menu");
	display_menu();
    while(1)
    {
			  
        while(!QueIsEmpty(rx_que2))
        {
					OutQue(rx_que2, &data);
					//SYSTEM_INFO("%c",data);
					if(data == '\x1b'){
					printf("\033[H\033[J");
						menu_a.esc_state = ESC_IDLE;
						
					}
					if((menu_a.is_choice_enter==1)&&(data == '\x0d')){

						menu_a.is_choice_enter=0;
						printf("\r\nselect option:%s",menu[menu_a.current_selection]);	
             USART_ITConfig(USARTx_A, USART_IT_RXNE, DISABLE);
             printf("\r\n");

							return;
							}
					
					
					shell(data);
					
					//SYSTEM_INFO("state=%d\n",menu_a.esc_state);//xxxxa
					
					        switch (menu_a.esc_state) {
            case ESC_IDLE:
                if (data == '\x1b') { // 检测到ESC字符
                    menu_a.esc_state = ESC_RECEIVED;
                } else {

									if((menu_a.is_choice_enter==1)&&(menu_a.esc_state==ESC_IDLE)){
									display_menu();				
										SYSTEM_INFO("select=clear\n");//xxxx					
									}
									
                }
                break;

            case ESC_RECEIVED:
                if (data == '[') { // 检测到'['字符
                    menu_a.esc_state = BRACKET_RECEIVED;
                } else {
                    // 如果不是'['，则重置状态机
                    menu_a.esc_state = ESC_IDLE;


                }
                break;

            case BRACKET_RECEIVED:
                menu_a.esc_state = ESC_IDLE; // 无论是否匹配，都重置状态机
                switch (data) {
                    case 'C': // 右箭头
                        menu_a.current_selection = (menu_a.current_selection + 1) % MENU_SIZE;
	
                        display_menu();
                        break;
                    case 'D': // 左箭头
                        menu_a.current_selection = (menu_a.current_selection - 1 + MENU_SIZE) % MENU_SIZE;

                        display_menu();
                        break;
                    // 可以根据需要添加对其他方向键的支持
                    default:
                        // 对于其他字符，不做任何操作或执行其他处理

                        break;
                }
                break;
        }

        }	
    }

}


void display_menu() {
    printf("\033[H\033[J"); // 清屏
    for(int i=0; i<MENU_SIZE; ++i) {
        if(i == menu_a.current_selection) {
            printf("> %s", menu[i]); // 当前选中的菜单项前加上'>'
					menu_a.is_choice_enter=1;
        } else {
            printf("  %s", menu[i]);
        }
    }
}


void shell_cmd_menu(char argc, char *argv) {
    (void)argv; // 忽略参数
	USART_ITConfig(USARTx_A, USART_IT_RXNE, ENABLE);
    dispose_menu(); // 显示初始菜单
}

// 注册命令
//NR_SHELL_CMD_EXPORT(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), menu, shell_cmd_menu, "Enter menu mode");

NR_SHELL_CMD_EXPORT(menu, shell_cmd_menu);




void exe_window_cmd(void)
{
	
	
	
	
	
}

#endif








/*********************************************END OF FILE**********************/
