#ifndef _W_LV_SCREEN_H

#define _W_LV_SCREEN_H

#include "lvgl/lvgl.h"


#ifdef __cplusplus
extern "C" {
#endif



// #define W_LV_SCREEN_1_9 (lv_obj_get_width(lv_screen_active()) *-4/ 5)
// #define W_LV_SCREEN_2_9 (lv_obj_get_width(lv_screen_active())*-3 / 5)
// #define W_LV_SCREEN_3_9 (lv_obj_get_width(lv_screen_active()) * -2 / 5)
// #define W_LV_SCREEN_4_9 (lv_obj_get_width(lv_screen_active())  * -1 / 5)
// #define W_LV_SCREEN_5_9 (0)

// 通用宏定义：计算屏幕宽度的 分子/分母 倍，例如 lv_obj_align(sw, LV_ALIGN_CENTER, LV_SCREEN_WIDTH_RATIO(-5, 10)    , 20);
// 参数说明：
//   num：分子（如-4、-3、0等）
//   den：分母（如5、10等，需确保非0）
#define W_LV_SCREEN_WIDTH_RATIO(num, den)  ((lv_obj_get_width(lv_screen_active())) * (num) / (den))
#define W_LV_SCREEN_HEIGHT_RATIO(num, den)  ((lv_obj_get_height(lv_screen_active())) * (num) / (den))




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_SMART_CHARGER_H

