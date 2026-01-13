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

volatile uint32_t lv_tick_cnt = 0;


void lv_init_all(void){
#if !(USE_LVGL_OS)
     // H743 1ms定时中断最优配置

//	btim_timx_int_init(100 - 1, 2000 - 1);    /* 200 000 000 / 200 00 = 10 000KHz 10KHz的计数频率，计数次为1ms */
	btim_timx_int_init(100 - 1, 2400 - 1);    /* 200 000 000 / 200 00 = 10 000KHz 10KHz的计数频率，计数次为1ms */
#endif
    lv_init(); 
    lv_port_disp_init(); 
    lv_port_indev_init();

}








void Handle_lv(void){


        // 处理LVGL心跳（移到主线程）
        if(lv_tick_cnt > 0)
        {
            lv_tick_inc(lv_tick_cnt);
            lv_tick_cnt = 0;
        }
	lv_timer_handler();
}



#include "lv_demo_stress.h"
//#include "lv_demo_music.h"
#include "../lv_examples.h"
void lv_test(void){

//    lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
//    lv_obj_set_size(switch_obj, 120, 60);
//    lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
	
#if 0		
	lv_demo_stress();
#endif
	
#if 1	
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
#endif
}














































/*********************************************END OF FILE**********************/
