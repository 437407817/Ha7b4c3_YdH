#ifndef STATUS_BAR_H
#define STATUS_BAR_H

// #include "lvgl.h"
 #include "../ui.h"

// // 全局初始化状态栏（整个程序只调用一次）
// void status_bar_create(void);

// // 更新状态栏时间（可选）
// void status_bar_update_time(const char *time_str);

#define STATUSBAR_H     30
#define CTRL_CENTER_H   260


extern lv_obj_t *g_status_bar;

void sys_top_bar_init(lv_obj_t *page);
void sys_top_bar_set_time(const char* str);
// 绑定手势：页面 + 状态栏都要绑
void sys_top_bar_attach_all_swipe(lv_obj_t* page);





#endif