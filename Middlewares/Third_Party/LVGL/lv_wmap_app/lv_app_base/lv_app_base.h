#ifndef _LV_APP_BASE_H

#define _LV_APP_BASE_H

#include "lvgl/lvgl.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

/**
 * @brief Initialize the Smart Charger UI Demo
 * Call this function after lv_init() and display driver setup.
 */
//void lv_smart_charger_init(void);
void ui_init(void) ;
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_SMART_CHARGER_H

