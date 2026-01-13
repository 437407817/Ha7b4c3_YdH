


#include "FreeRTOS.h"
#include "nr_micro_shell.h"
#include "semphr.h"

//#include "cmsis_os.h"

 #include "./usart/que.h"
 
 #include "nr_micro_shell.h"
#include <string.h>
#include <ctype.h>
 #include "./DataConvert/w_string.h"  //±Ø±¸
 
 #include <stdbool.h>
 #define LOG_TAG "USER"
#include "elog.h"



 
 
int nr_log(uint8_t argc, char **argv)
{
    log_a("log a EasyLogger! aa");
    log_e("log e EasyLogger! ee");
    log_w("log w EasyLogger! ww");
    log_i("log i EasyLogger! ii");
    log_d("log d EasyLogger! dd");
    log_v("log v EasyLogger! vv");
}

//NR_SHELL_CMD_EXPORT(testlog, nr_log);



 
int nr_print(uint8_t argc, char **argv)
{
 printf("NRS is short for nr'Shell!!\r\n");
}

//NR_SHELL_CMD_EXPORT(testprint,nr_print);



//------------------------------











