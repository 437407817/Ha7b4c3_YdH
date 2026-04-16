/**
  ******************************************************************************
  * @file    
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./lvgl_port.h"
//#include "./touch/bsp_touch_gtxx.h"
#include "./touch/bsp_i2c_touch.h"


#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
//#include "lv_demo_stress.h"
//#include "lv_demo_music.h"
#include "timer/btim.h"
#include "./user_config.h"
#include "./global/GV_variable.h" 

#if (USE_LVGL_OS) && 0
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "freerotspro.h" 
#endif
volatile uint32_t lv_tick_cnt = 0;


void lv_init_all(void){
#if !(USE_LVGL_OS)
     // H743 1ms定时中断最优配置

//	btim_timx_int_init(100 - 1, 2000 - 1);    /* 200 000 000 / 200 00 = 10 000KHz 10KHz的计数频率，计数次为1ms */
	btim_timx_int_init(100 - 1, 2400 - 1);    /* 200 000 000 / 200 00 = 10 000KHz 10KHz的计数频率，计数次为1ms */
#endif
    lv_init(); 
#if 0
#if (USE_LVGL_OS)
	// 替换原来的 lv_tick_set_cb(HAL_GetTick());
		lv_tick_set_cb((lv_tick_get_cb_t)xTaskGetTickCount);
#else
		lv_tick_set_cb(HAL_GetTick);
#endif	
#endif
    lv_port_disp_init(); 
    lv_port_indev_init();

}

#include "ui.h"
#include "./global/GV_variable.h" 

extern lv_obj_t * scroll_view ;
extern STR_GET_VOL_Data_t GV_get_vol_real_data;

extern STR_SEND_SETTING_DATA_t GV_send_setting_return_data;

extern uint8_t update_lvgl_flag;
extern uint8_t update_lvgl_02_flag;
bool ui_initialized = false; // 新增标志位

//void update_lvgl_data(void){
//    // 增加对核心 UI 组件的检查
//    // 假设 display_64_values 内部会访问特定的 label，确保那些 label 已经存在
//    if(scroll_view != NULL && lv_obj_is_valid(scroll_view)) { 
//        display_64_values(scroll_view, 
//                          GV_get_vol_real_data.Bat_Vol, 
//                          GV_get_vol_real_data.Bat_WorkStatus, 
//                          BAT_MAX_NUM);
//    } else {
//        // 如果 UI 还没准备好，先不刷新，防止报 NULL 错误
//         log_d("UI not ready yet..."); 
//    }
//}
//#include <string.h>
// lvgl_port.c
//uint64_t Bat_temp_WorkStatus;
void update_lvgl_data(void){
    // 检查 UI 是否已经加载到当前屏幕，防止在后台页面刷新报错
    if(lv_screen_active() == ui_S_page01_screen) {
//			memcpy(&Bat_temp_WorkStatus, &GV_get_vol_real_data.Bat_WorkStatus,8);
        // 直接调用高效的更新函数
        ui_S_page01_update_values(GV_get_vol_real_data.Bat_Vol, 
                                  GV_get_vol_real_data.Bat_WorkStatus);
			ui_S_page01_update_error_values(GV_get_vol_real_data.ErrorStatus);
    }
}

void update_lvgl_02data(void){
    // 检查 UI 是否已经加载到当前屏幕，防止在后台页面刷新报错
    if(lv_screen_active() == ui_S_page01_screen) {
        // 直接调用高效的更新函数
ui_S_page01_update_initial_setting_values(GV_send_setting_return_data.IsAutoRun,GV_send_setting_return_data.StopVoltage,
			GV_send_setting_return_data.VoltageDifference,GV_send_setting_return_data.AlarmHighVoltage);
    }
}


// ui_manager.c
void sensor_update_timer_cb(lv_timer_t * timer) {
    // 检查是否有新数据（或者直接强制刷新）
if (ui_initialized && update_lvgl_flag) { 
        update_lvgl_data();
        update_lvgl_flag = 0;
    }
if (ui_initialized && update_lvgl_02_flag) { 
        update_lvgl_02data();
        update_lvgl_02_flag = 0;
    }
}

#if !(USE_LVGL_OS)&&(USE_LVGL)
void Handle_lv(void){


        // 处理LVGL心跳（移到主线程）
        if(lv_tick_cnt > 0)
        {
            lv_tick_inc(lv_tick_cnt);
            lv_tick_cnt = 0;
        }
	lv_timer_handler();
}


#endif







#include "ui.h"
#include "lv_demo_stress.h"
//#include "lv_demo_music.h"
#include "../lv_examples.h"
void lv_pro_run(void){

//    lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
//    lv_obj_set_size(switch_obj, 120, 60);
//    lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
	ui_init();
	// 在 lv_init() 和 界面创建代码 之后
lv_timer_t * timer = lv_timer_create(sensor_update_timer_cb, 200, NULL);
	ui_initialized = true; // 标记初始化完成
#if 0		
	lv_demo_stress();
#endif
	//ui_create();
	//lv_switch_label_demo();
#if 0	
//	lv_obj_t * scr = lv_scr_act(); // 获取默认屏幕
	  lv_obj_t * label;
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);
    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
#endif

#if 1	
//lv_demo_music();
//lv_example_get_started_2();
//lv_demo_stress();
#endif


}














































/*********************************************END OF FILE**********************/
