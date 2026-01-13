#include "busi_test.h"
#include "cmsis_os.h"
#include "business.h"
#include "common.h"

//#include "shell.h"

#define LOG_TAG "BUSI_TEST"

#include "elog.h"

TaskHandle_t mst_handle;
//接收到bussiness的指令，进行采集，执行，然后返回给bussiness
void mock_sample_task_entry(void *p)
{
 log_d("mock sample task is running...");
	
 while(1)
 {
	log_d("wait for sample_sem_req forever"); 
	xSemaphoreTake(busi_sample_mmc_ctrl.sem_req,portMAX_DELAY);
  //传感器采集赋值
  busi_sample_values.temprature = 25;
	busi_sample_values.bright = 2;
	 
	busi_sample_values.reserved[0] = 1;
  busi_sample_values.reserved[1] = 2;
	busi_sample_values.reserved[2] = 3;
  busi_sample_values.reserved[3] = 4;
	 
	log_d("current temp = %d bright = %d",busi_sample_values.temprature,busi_sample_values.bright);
	log_d("current reserved = [0]%d [1]%d [2]%d [3]%d",busi_sample_values.reserved[0],busi_sample_values.reserved[1],busi_sample_values.reserved[2],busi_sample_values.reserved[3]);

	xSemaphoreGive(busi_sample_mmc_ctrl.sem_ack);
	log_d("release sem ack");
 }
}

void mock_switch_task_entry(void *p)
{
 log_d("mock switch task is running ....");
	
 while(1)
 {
	log_d("wait for switch_sem_req forever");
	 
	xSemaphoreTake(busi_switch_mmc_ctrl.sem_req,portMAX_DELAY);
	 
  busi_switch_values.fan_status = FAN_CTRL_OPEN_STATUS;
	busi_switch_values.light_status = LIGHT_CTRL_OPEN_STATUS;
	 
	busi_switch_values.reserved[0]  = 1;
	busi_switch_values.reserved[1]  = 2;
  busi_switch_values.reserved[2]  = 3;
  busi_switch_values.reserved[3]  = 4;		
	//可通过信息执行
	log_d("current fan_status = %d light_status = %d",busi_switch_values.fan_status,busi_switch_values.light_status);
	log_d("current reserved = [0]%d [1]%d [2]%d [3]%d",busi_switch_values.reserved[0],busi_switch_values.reserved[1],busi_switch_values.reserved[2],busi_switch_values.reserved[3]);
	   	 
  xSemaphoreGive(busi_switch_mmc_ctrl.sem_ack);
	 
	log_d("release sem ack");
 }
}

int smst(uint8_t argc,char **argv)
{
 log_d("start mock_sample_task_entry sample task");
	
 xTaskCreate(mock_sample_task_entry,"mock_sample_task",128,(void *)0,7,&mst_handle);
 xTaskCreate(mock_switch_task_entry,"mock_switch_task",128,(void *)0,7,&mst_handle);
 
}
//ZNS_CMD_EXPORT(smst,start mock sample task)




