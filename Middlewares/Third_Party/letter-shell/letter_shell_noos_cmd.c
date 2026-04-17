


#include "FreeRTOS.h"

#include "semphr.h"

//#include "cmsis_os.h"

 #include "./buffer/que.h"
 
#include "shell.h"
#include <string.h>
#include <ctype.h>
 #include "./DataConvert/w_string.h"  //±Ø±¸
 
 #include <stdbool.h>
 #define LOG_TAG "USER"
#include "elog.h"



 
 
int l_log(uint8_t argc, char **argv)
{
    log_a("log a EasyLogger! aa");
    log_e("log e EasyLogger! ee");
    log_w("log w EasyLogger! ww");
    log_i("log i EasyLogger! ii");
    log_d("log d EasyLogger! dd");
    log_v("log v EasyLogger! vv");
}

//NR_SHELL_CMD_EXPORT(testlog, nr_log);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_log, l_log, test log);

 
int l_print(uint8_t argc, char **argv)
{
 printf("letter 'Shell!!\r\n");
}

//NR_SHELL_CMD_EXPORT(testprint,nr_print);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_print, l_print, test print);


//------------------------------











