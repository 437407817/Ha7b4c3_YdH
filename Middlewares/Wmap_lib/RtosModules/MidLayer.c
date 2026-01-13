#include "midlayer.h"
#include "cmsis_os.h"
#include "business.h"

#include "mmc.h"
#include "common.h"

#define LOG_TAG "BUSI_MODULE"
#include "elog.h"

mmc_ctrl_stru midlayer_sample_tempraure_mmc_ctrl;
mmc_ctrl_stru midlayer_sample_bright_mmc_ctrl;
mmc_ctrl_stru midlayer_switch_light_mmc_ctrl;
mmc_ctrl_stru midlayer_switch_fan_mmc_ctrl;

midlayer_sample_temprature_stru midlayer_sample_temprature_values;
midlayer_sample_bright_stru     midlayer_sample_bright_values;

midlayer_switch_light_stru      midlayer_switch_light_sw;
midlayer_switch_fan_stru        midlayer_switch_fan_sw;

TaskHandle_t MidLayer_Sample_Process_Task_Handler;//任务句柄
TaskHandle_t MidLayer_Switch_Process_Task_Handler;//任务句柄

void midlayer_sample_process_task_entry(void *p)
{
 log_i("midlayer_sample_process_task is running ...");
	
 mmc_ctrl_init(&midlayer_sample_tempraure_mmc_ctrl,&midlayer_sample_temprature_values,sizeof(midlayer_sample_temprature_values),"midlayer_sample_temprature_values");
 mmc_ctrl_init(&midlayer_sample_bright_mmc_ctrl,&midlayer_sample_bright_values,sizeof(midlayer_sample_bright_values),"midlayer_sample_bright_values");

 while(1)
 {
	log_d("wait for busi_sample_sem_req"); 
	 
	xSemaphoreTake(busi_sample_mmc_ctrl.sem_req,portMAX_DELAY);	//等待来自业务的采集请求
	 
	if(mmc_ctrl_require(&midlayer_sample_tempraure_mmc_ctrl,MMC_DELAY_TIME) == pdPASS) //中间层请求温度
	{
	 log_i("sample temprature requre SUCCESS!");
		
	 
   ((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->temprature = ((midlayer_sample_temprature_stru *)(midlayer_sample_tempraure_mmc_ctrl.p_shared_mem))->temprature;
	 log_i("temprature : %d", ((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->temprature);	//显示接收到的温度值

	}
	 
	if(mmc_ctrl_require(&midlayer_sample_bright_mmc_ctrl,MMC_DELAY_TIME) == pdPASS) //中间层请求亮度
	{
	 log_i("sample bright requre SUCCESS!");
		
	 
   ((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->bright = ((midlayer_sample_bright_stru *)(midlayer_sample_bright_mmc_ctrl.p_shared_mem))->bright;
	 log_i("bright : %d", ((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->bright);	//显示接收到的亮度值

	}
	
	//对其它传感器采集进行请求处理
	
	//对预留字段进行装填
	((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->reserved[0] = 1;
	((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->reserved[1] = 2;
	((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->reserved[2] = 3;
	((busi_sample_stru *)(busi_sample_mmc_ctrl.p_shared_mem))->reserved[3] = 4;
	
	xSemaphoreGive(busi_sample_mmc_ctrl.sem_ack); //回应顶层业务
	log_i("release busi_sample_sem_ack");
 }
}

void midlayer_switch_process_task_entry(void *p)
{
 log_i("midlayer_switch_process_task is running ...");
	
 mmc_ctrl_init(&midlayer_switch_light_mmc_ctrl,&midlayer_switch_light_sw,sizeof(midlayer_sample_temprature_values),"midlayer_sample_temprature_values");
 mmc_ctrl_init(&midlayer_switch_fan_mmc_ctrl,&midlayer_switch_fan_sw,sizeof(midlayer_sample_bright_values),"midlayer_sample_bright_values");

 while(1)
 {
	log_d("wait for busi_switch_sem_req"); 
	 
	xSemaphoreTake(busi_switch_mmc_ctrl.sem_req,portMAX_DELAY);	//等待来自业务的控制请求

	((midlayer_switch_light_stru *)(midlayer_switch_light_mmc_ctrl.p_shared_mem))->light_sw = ((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->light_status;
	 
	if(mmc_ctrl_require(&midlayer_switch_light_mmc_ctrl,MMC_DELAY_TIME) == pdPASS) //中间层请求灯光开关
	{
	 log_i("switch light requre SUCCESS!");
		
   ((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->light_status = ((midlayer_switch_light_stru *)(midlayer_switch_light_mmc_ctrl.p_shared_mem))->light_sw;
	 log_i("switch light feedback status : %d", ((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->light_status);	//显示接收到的灯光开关反馈状态

	}
	
	((midlayer_switch_fan_stru *)(midlayer_switch_fan_mmc_ctrl.p_shared_mem))->fan_sw = ((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->fan_status;

	if(mmc_ctrl_require(&midlayer_switch_fan_mmc_ctrl,MMC_DELAY_TIME) == pdPASS) //中间层请求风扇开关
	{
	 log_i("switch fan requre SUCCESS!");
		
	 
   ((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->fan_status = ((midlayer_switch_fan_stru *)(midlayer_switch_fan_mmc_ctrl.p_shared_mem))->fan_sw;
	 log_i("switch fan feedback status : %d", ((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->fan_status);	//显示接收到的风扇开关反馈状态

	}
	
	//对其它开关控制进行请求处理
	
	//对预留字段进行装填（象征更多的关于反馈状态）
	((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->reserved[0] = 1;
	((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->reserved[1] = 2;
	((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->reserved[2] = 3;
	((busi_switch_stru *)(busi_switch_mmc_ctrl.p_shared_mem))->reserved[3] = 4;
	
	xSemaphoreGive(busi_switch_mmc_ctrl.sem_ack); //回应顶层业务
	log_i("release busi_switch_sem_ack");
 }
}

__weak void SwitchDrv_Init();
__weak void SampleDrv_Init();

void midlayer_init()
{
 SwitchDrv_Init();
 SampleDrv_Init();
	
 //task create tasks
	
 xTaskCreate((TaskFunction_t )midlayer_sample_process_task_entry,"midlayer_sample_process_task",128,NULL, 2,(TaskHandle_t*  )&MidLayer_Sample_Process_Task_Handler);  
 xTaskCreate((TaskFunction_t )midlayer_switch_process_task_entry,"midlayer_switch_process_task",128,NULL, 2,(TaskHandle_t*  )&MidLayer_Switch_Process_Task_Handler);  

}
