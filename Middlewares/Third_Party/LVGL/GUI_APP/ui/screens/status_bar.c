#include "../ui.h"

// 🔥 必须加宏定义
#define STATUSBAR_H       30      // 状态栏高度
#define CTRL_CENTER_H     300     // 控制中心高度

lv_obj_t *g_status_bar;
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
// 手势回调（🔥方向已修复）
// ==============================
static void status_bar_gesture_cb(lv_event_t *e) 
{ 
     lv_dir_t dir = lv_indev_get_gesture_dir(lv_event_get_indev(e)); 
    // 获取当前触发事件的对象
    lv_obj_t *current_target = lv_event_get_current_target(e);
    
    // 判断触发源是否为 g_ctrl_center
    if (current_target == g_ctrl_center) {
        // 触发源是控制中心时的处理逻辑
        // v_printf("Gesture triggered from control center\n");
        if (dir == LV_DIR_TOP) 
    { 
        ctrl_center_close(); 
    } 

    }
    
   

    // 🔥 向下滑 = LV_DIR_BOTTOM（正确触发） 
    if (dir == LV_DIR_BOTTOM) 
    { 
        ctrl_center_open(); 
    } 
    // 向上滑关闭 
    else if (dir == LV_DIR_TOP) 
    { 
        ctrl_center_close(); 
    } 
}

// ==============================
// 创建状态栏（🔥触摸属性全开）
// ==============================
static void status_bar_create(lv_obj_t *page)
{
    lv_obj_t *layer = lv_layer_top();
    
    g_status_bar = lv_obj_create(page);
 lv_obj_set_parent(g_status_bar, page);


    lv_obj_set_size(g_status_bar, LV_HOR_RES, STATUSBAR_H);
    lv_obj_align(g_status_bar, LV_ALIGN_TOP_MID, 0, 0);

    // 🔥 关键：允许点击 + 允许手势传递
    lv_obj_clear_flag(g_status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(g_status_bar, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);

    // 样式
    lv_obj_set_style_bg_color(g_status_bar, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(g_status_bar, 0, 0);





    // 时间
    lv_obj_t *time_lab = lv_label_create(g_status_bar);
    lv_label_set_text(time_lab, "00:00");
    lv_obj_set_style_text_color(time_lab, lv_color_white(), 0);
    lv_obj_align(time_lab, LV_ALIGN_LEFT_MID, 10, 0);

    // 状态
    lv_obj_t *info_lab = lv_label_create(g_status_bar);
    lv_label_set_text(info_lab, "WiFi 🔋");
    lv_obj_set_style_text_color(info_lab, lv_color_white(), 0);
    lv_obj_align(info_lab, LV_ALIGN_RIGHT_MID, -10, 0);

    // 🔥 绑定手势
   // lv_obj_add_event_cb(g_status_bar, status_bar_gesture_cb, LV_EVENT_GESTURE, NULL);
    // 置于顶层
// lv_obj_move_foreground(g_status_bar);
    
}

// ==============================
// 创建控制中心
// ==============================
static void ctrl_center_create(lv_obj_t *page)
{
    lv_obj_t *layer = lv_layer_top();

    g_ctrl_center = lv_obj_create(layer);
    lv_obj_remove_flag(g_ctrl_center, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(g_ctrl_center, LV_HOR_RES, CTRL_CENTER_H);
    lv_obj_align(g_ctrl_center, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(g_ctrl_center, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_radius(g_ctrl_center, 20, 0);
    lv_obj_add_flag(g_ctrl_center, LV_OBJ_FLAG_HIDDEN);
//设置g_ctrl_center显示在悬浮层
    // lv_obj_set_parent(g_ctrl_center, layer);
 
    


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
        // 禁止控制中心拦截手势
     lv_obj_add_flag(g_ctrl_center, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(layer, status_bar_gesture_cb, LV_EVENT_GESTURE, NULL);
}

// ==============================
// 初始化
// ==============================
void sys_top_bar_init(lv_obj_t *page)
{
    status_bar_create(page);
    ctrl_center_create(page);
}

// 给页面绑定下滑（整个屏幕顶部都能滑）
void sys_top_bar_attach_all_swipe(lv_obj_t *page)
{
        sys_top_bar_init(page);   
      lv_obj_add_event_cb(page, status_bar_gesture_cb, LV_EVENT_GESTURE, NULL);
        // 绑定到状态栏
    //  lv_obj_add_event_cb(g_status_bar, status_bar_gesture_cb, LV_EVENT_GESTURE, NULL);
}