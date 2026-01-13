


#include "FreeRTOS.h"

#include "semphr.h"

#include "cmsis_os.h"

 #include "./buffer/que.h"
 

#include <string.h>
#include <ctype.h>
 #include "./DataConvert/w_string.h"  //必备
 
 #include <stdbool.h>
//#include <elog.h>
#include "shell.h"
#define LOG_TAG "SYSTEM"
#include "elog.h"
#include "stdlib.h"




//const char* get_task_state_string(eTaskState state) {
//    switch (state) {
//        case eRunning:    return "eRunning";
//        case eReady:      return "eReady";
//        case eBlocked:    return "eBlocked";
//        case eSuspended:  return "eSuspended";
//        case eDeleted:    return "eDeleted";
//        case eInvalid:    return "eInvalid";
//        default:          return "Unknown state";
//    }
//}
// 字符串数组：顺序与eTaskState枚举成员一一对应
const char* task_state_strings[] = {
    "eRunning",    // 对应 eRunning (0)
    "eReady",      // 对应 eReady (1)
    "eBlocked",    // 对应 eBlocked (2)
    "eSuspended",  // 对应 eSuspended (3)
    "eDeleted",    // 对应 eDeleted (4)
    "eInvalid"     // 对应 eInvalid (5)
};




int l_pti(uint8_t argc, char **argv)
{
 TaskHandle_t tmp_handle;
 TaskStatus_t xTaskDetails;
	tmp_handle = xTaskGetHandle(argv[1]);
	if(NULL==tmp_handle){
	printf("task name is wrong!\r\n");
		return 0;
	}
// tmp_handle = xTaskGetHandle("LED1_Task"); 
	//xTaskGetCurrentTaskHandle();
 vTaskGetInfo(tmp_handle,&xTaskDetails,1,eInvalid);
	
 taskENTER_CRITICAL(); 
	 
// printf("TaskName : %s , TaskNumber = %d , CurrentState = %d , CurrentPriority = %d , BasePriority =  %d , RunTimeCounter = %d ,StackHighWaterMark = %d , TickCount = %d\r\n\r\n",
//	       xTaskDetails.pcTaskName,(uint16_t)xTaskDetails.xTaskNumber,xTaskDetails.eCurrentState,(uint16_t)xTaskDetails.uxCurrentPriority,
//	(uint16_t)xTaskDetails.uxBasePriority,xTaskDetails.ulRunTimeCounter,xTaskDetails.usStackHighWaterMark,xTaskGetTickCount());
  printf("TaskName : %s , TaskNumber = %d , CurrentState = %s , CurrentPriority = %d , BasePriority =  %d , RunTimeCounter = %d ,StackHighWaterMark = %d , TickCount = %d\r\n\r\n",
	       xTaskDetails.pcTaskName,(uint16_t)xTaskDetails.xTaskNumber,task_state_strings[xTaskDetails.eCurrentState],(uint16_t)xTaskDetails.uxCurrentPriority,
	(uint16_t)xTaskDetails.uxBasePriority,xTaskDetails.ulRunTimeCounter,xTaskDetails.usStackHighWaterMark,xTaskGetTickCount());
 taskEXIT_CRITICAL();  
	return 0;
}

//NR_SHELL_CMD_EXPORT(pti,nr_pti);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), pti, l_pti, show x Task Info);


int l_ptl(uint8_t argc, char **argv)
{
	
 char str[200];
 vTaskList(str);

 taskENTER_CRITICAL();  
 printf("TaskName  State Priority  Stack  Number \r\n%s\r\n",str);
 taskEXIT_CRITICAL(); 
		return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), ptl, l_ptl, show All Task State);

//NR_SHELL_CMD_EXPORT(ptl, nr_ptl);



int l_mem(uint8_t argc, char **argv)
{
	
 taskENTER_CRITICAL(); 
 printf("FreeHeapSize = %d   EverFreeHeapSize = %d\r\n",xPortGetFreeHeapSize(),xPortGetMinimumEverFreeHeapSize());
 taskEXIT_CRITICAL();
		return 0;
}

//NR_SHELL_CMD_EXPORT(mem, nr_mem);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), mem, l_mem, Heap Size);


int l_suspend(uint8_t argc, char **argv)
{
	 TaskHandle_t tmp_handle;
	printf("argc:%d\r\n",argc);

	printf("suspend task:%s\r\n",argv[1]);
	tmp_handle=xTaskGetHandle(argv[1]);
	if(NULL==tmp_handle){
	printf("task name is wrong!\r\n");
		return 0;
	}
	vTaskSuspend(tmp_handle);
	return 0;
}

//NR_SHELL_CMD_EXPORT(suspend, nr_suspend);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), suspend, l_suspend, suspend x Task);
//vTaskResume

int l_resume(uint8_t argc, char **argv)
{
	TaskHandle_t tmp_handle;
	tmp_handle=xTaskGetHandle(argv[1]);
	printf("argc:%d\r\n",argc);
	printf("resume task:%s\r\n",argv[1]);
		if(NULL==tmp_handle){
	printf("task name is wrong!\r\n");
		return 0;
	}
	vTaskResume(tmp_handle);
		return 0;
}

//NR_SHELL_CMD_EXPORT(resume, nr_resume);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), resume, l_resume, resume x Task);

int l_delete(uint8_t argc, char **argv)
{
		TaskHandle_t tmp_handle;
	tmp_handle=xTaskGetHandle(argv[1]);
	printf("delete task:%s \r\n",(argv[1]));
	if(NULL==tmp_handle){
	printf("task name is wrong!\r\n");
		return 0;
	}
	vTaskDelete(tmp_handle);
		return 0;
}

//NR_SHELL_CMD_EXPORT(delete, nr_delete);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), delete, l_delete, delete x Task);


int l_setpriority(uint8_t argc, char **argv)
{
	TaskHandle_t tmp_handle;
//	configASSERT(isNumericString((&argv[argv[2]])));
	tmp_handle=xTaskGetHandle(argv[1]);
	printf("argc:%d\r\n",argc);
	printf("set task:%s priority:%s\r\n",(argv[1]),(argv[2]));
		if(NULL==tmp_handle){
	printf("task name is wrong!\r\n");
		return 0;
	}
				if(0==isNumericString(argv[2])){
	printf("second param is not num\r\n");
		return 0;
	}
//	vTaskPrioritySet(xTaskGetHandle((argv[1])),(UBaseType_t)stringToInt((argv[2])));
	vTaskPrioritySet(xTaskGetHandle((argv[1])),(UBaseType_t)atoi((argv[2])));
		return 0;
}

//NR_SHELL_CMD_EXPORT(setpty, nr_setpriority);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), setpriority, l_setpriority, set x Task priority);

 


//修改elo过滤等级fll 3/fll 2
int l_filterElogLever(uint8_t argc, char **argv)
{
    int level = 0;
				if(0==isNumericString(argv[1])){
	printf("first param is not num\r\n");
		return 0;
	}
    level = atoi(argv[1]);
    printf("set level :%d\r\n",level);
    elog_set_filter_lvl(level);
	return 0;
}

//NR_SHELL_CMD_EXPORT(fll, filterElogLever);
 SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), fel, l_filterElogLever, filter Elog Lever);


//fllt 过滤tag标签//取消fllk 空
int l_fllt(uint8_t argc, char **argv)
{
	
    elog_set_filter_tag(argv[1]);
    printf("set filter tag: %s",argv[1]);
		return 0;
}

//NR_SHELL_CMD_EXPORT(fllt, fllt);
 SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), fet, l_fllt, filter Elog Tag);


//fllt keyword filter//取消fllk 空
int l_fllk(uint8_t argc, char **argv)
{
    elog_set_filter_kw(argv[1]);
    printf("set filter keyword: %s\r\n",argv[1]);
		return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), fek, l_fllk, filter Elog Key Word);

//NR_SHELL_CMD_EXPORT(fllk, fllk);


