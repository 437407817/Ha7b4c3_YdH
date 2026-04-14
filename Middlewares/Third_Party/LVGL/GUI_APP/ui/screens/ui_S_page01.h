
#ifndef UI_S_PAGE01_H
#define UI_S_PAGE01_H

#ifdef __cplusplus
extern "C" {
#endif

 #include "../ui.h"


extern void ui_S_page01_screen_init(void);
extern void ui_S_page01_screen_destroy(void);
extern void ui_S_page01_screen_relocalize(void);
extern void ui_S_page01_update_values(uint16_t values[64], uint64_t color_flags);
extern void ui_S_page01_update_error_values( uint8_t error_flags);
void ui_S_page01_update_initial_setting_values(
    uint8_t auto_flag,uint16_t stop_vol,uint16_t dif_vol,uint16_t alarm_vol);
extern lv_obj_t * ui_S_page01_screen;
extern lv_obj_t * scroll_view ;
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif


