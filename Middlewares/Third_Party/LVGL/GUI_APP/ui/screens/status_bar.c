// #include "status_bar.h"
#include "../ui.h"

// static lv_obj_t *status_bar;
// static lv_obj_t *time_label;
// static lv_obj_t *info_label;

// // 创建全局状态栏（顶层显示，所有页面共用）
// void status_bar_create(void)
// {
//     // ======================
//     // 关键：创建在 顶层屏幕 layer
//     // 所有页面都无法覆盖它
//     // ======================
//     lv_obj_t *layer = lv_layer_top();  // LVGL V9 全局顶层层

//     // 状态栏背景
//     status_bar = lv_obj_create(layer);
//     lv_obj_set_size(status_bar, LV_HOR_RES, 30);
//     lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
//     //改成蓝色
//     lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x007AFF), 0);
//     lv_obj_set_style_border_width(status_bar, 0, 0);
//  lv_obj_remove_flag(status_bar, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); 
//     // 时间
//     time_label = lv_label_create(status_bar);
//     lv_label_set_text(time_label, "12:30");
//     lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
//     lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);

//     // 状态信息
//     info_label = lv_label_create(status_bar);
//     lv_label_set_text(info_label, "5G WiFi 🔋95%");
//     lv_obj_set_style_text_color(info_label, lv_color_white(), 0);
//     lv_obj_align(info_label, LV_ALIGN_RIGHT_MID, -10, 0);
// }

// // 可选：动态更新时间
// void status_bar_update_time(const char *time_str)
// {
//     if (time_label) {
//         lv_label_set_text(time_label, time_str);
//     }
// }



// #include "sys_top_bar.h"





static lv_obj_t *g_status_bar;
static lv_obj_t *g_ctrl_center;

// 动画结束隐藏
static void anim_hide_cb(lv_anim_t *a)
{
    lv_obj_add_flag(g_ctrl_center, LV_OBJ_FLAG_HIDDEN);
}

// 关闭控制中心
static void ctrl_center_close(void)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, g_ctrl_center);
    lv_anim_set_time(&a, 300);
    lv_anim_set_values(&a, 0, -CTRL_CENTER_H);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_ready_cb(&a, anim_hide_cb);
    lv_anim_start(&a);
}

// 打开控制中心
static void ctrl_center_open(void)
{
    if (!lv_obj_has_flag(g_ctrl_center, LV_OBJ_FLAG_HIDDEN)) return;

    lv_obj_set_y(g_ctrl_center, -CTRL_CENTER_H);
    lv_obj_remove_flag(g_ctrl_center, LV_OBJ_FLAG_HIDDEN);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, g_ctrl_center);
    lv_anim_set_time(&a, 300);
    lv_anim_set_values(&a, -CTRL_CENTER_H, 0);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_start(&a);
}

// ==============================
// 手势：状态栏滑动触发
// ==============================
static void status_bar_gesture_cb(lv_event_t *e)
{
    printf("status_bar_gesture_cb\n"); // 先打印确认回调是否触发
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_event_get_indev(e));

    // 🔥 修正：向下滑（手指从上→下）对应 LV_DIR_TOP
    if (dir == LV_DIR_TOP) 
    {
        ctrl_center_open();
    }
    // 可选：向上滑关闭控制中心
    else if (dir == LV_DIR_BOTTOM)
    {
        ctrl_center_close();
    }
}

// ==============================
// 创建状态栏
// ==============================
static void status_bar_create(void)
{
    lv_obj_t *layer = lv_layer_top();

    g_status_bar = lv_obj_create(layer);
 lv_obj_add_flag(g_status_bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(g_status_bar, LV_OBJ_FLAG_SCROLLABLE); 
 

    lv_obj_set_size(g_status_bar, LV_HOR_RES, STATUSBAR_H);
    lv_obj_align(g_status_bar, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_set_style_bg_color(g_status_bar, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(g_status_bar, 0, 0);

    // 时间
    lv_obj_t *time_lab = lv_label_create(g_status_bar);
    lv_label_set_text(time_lab, "12:30");
    lv_obj_set_style_text_color(time_lab, lv_color_white(), 0);

    
    lv_obj_align(time_lab, LV_ALIGN_LEFT_MID, 10, 0);

    // 状态
    lv_obj_t *info_lab = lv_label_create(g_status_bar);
    lv_label_set_text(info_lab, "WiFi 🔋");
    lv_obj_set_style_text_color(info_lab, lv_color_white(), 0);
    lv_obj_align(info_lab, LV_ALIGN_RIGHT_MID, -10, 0);

    // ====================================
    // 🔥 核心：给状态栏绑定下滑手势
    // ====================================
    lv_obj_add_event_cb(g_status_bar, status_bar_gesture_cb, LV_EVENT_GESTURE, NULL);
    
}

// ==============================
// 创建控制中心
// ==============================
static void ctrl_center_create(void)
{
    lv_obj_t *layer = lv_layer_top();

    g_ctrl_center = lv_obj_create(layer);
    lv_obj_set_size(g_ctrl_center, LV_HOR_RES, CTRL_CENTER_H);
    lv_obj_align(g_ctrl_center, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(g_ctrl_center, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_radius(g_ctrl_center, 20, 0);
    lv_obj_add_flag(g_ctrl_center, LV_OBJ_FLAG_HIDDEN);

    // 亮度条
    lv_obj_t *slider = lv_slider_create(g_ctrl_center);
    lv_obj_set_size(slider, LV_HOR_RES - 40, 6);
    lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 40);
    lv_slider_set_value(slider, 70, LV_ANIM_OFF);

    // 按钮
    const char *txts[] = {"WiFi", "aa", "bb", "cc", "dd", "ee"};
    for (int i = 0; i < 6; i++)
    {
        lv_obj_t *btn = lv_button_create(g_ctrl_center);
        lv_obj_set_size(btn, 100, 60);
        lv_obj_set_pos(btn, 20 + (i % 3) * 110, 80 + (i / 3) * 80);

        lv_obj_t *lab = lv_label_create(btn);
        lv_label_set_text(lab, txts[i]);
        lv_obj_center(lab);
    }
}

// ==============================
// 初始化
// ==============================
void sys_top_bar_init(void)
{
    status_bar_create();
    ctrl_center_create();
}

// 绑定手势：页面 + 状态栏都要绑
void sys_top_bar_attach_all_swipe(lv_obj_t *page)
{
    lv_obj_add_event_cb(page, status_bar_gesture_cb, LV_EVENT_GESTURE, NULL);
}
