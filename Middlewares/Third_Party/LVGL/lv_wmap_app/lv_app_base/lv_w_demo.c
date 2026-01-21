
#include "lv_app_base/lv_w_demo.h"







#include "lvgl/lvgl.h"
// 1. 先定义选中/未选中样式（保留）
static lv_style_t style_unchecked; // 未选中样式
static lv_style_t style_checked;   // 选中样式

void init_checkable_style(void) {
    // 初始化未选中样式（默认灰色）
    lv_style_init(&style_unchecked);
    lv_style_set_bg_color(&style_unchecked, lv_palette_lighten(LV_PALETTE_GREY, 2));
    lv_style_set_text_color(&style_unchecked, lv_color_black());

    // 初始化选中样式（红色背景+白色文字）
    lv_style_init(&style_checked);
    lv_style_set_bg_color(&style_checked, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_text_color(&style_checked, lv_color_white());
}

// 2. 定义C语言风格的静态回调函数（替代Lambda）
static void check_state_change_cb(lv_event_t * e) {
    // 获取触发事件的按钮和内部标签
    lv_obj_t * btn = lv_event_get_target_obj(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0); // 获取按钮的第一个子控件（标签）
    
    // 判断当前选中状态并更新标签文本
    if(lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_label_set_text(label, "checked");
    } else {
        lv_label_set_text(label, "unchecked");
    }
}

// 3. 创建可勾选按钮（修正事件绑定）
void create_checkable_btn(void) {
    init_checkable_style(); // 初始化样式

    // 创建基础按钮
    lv_obj_t * btn = lv_btn_create(lv_screen_active());
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    // 核心：开启可勾选标志
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    // 绑定状态样式（未选中/选中）
    lv_obj_add_style(btn, &style_unchecked, LV_STATE_DEFAULT);  // 未选中时应用
    lv_obj_add_style(btn, &style_checked, LV_STATE_CHECKED);    // 选中时应用

    // 添加标签（显示选中状态）
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "unchecked");
    lv_obj_center(label);

    // 修正：C语言风格绑定事件（关键！）
    // 参数1：目标控件  参数2：静态回调函数  参数3：事件类型  参数4：用户数据（无则传NULL）
    lv_obj_add_event_cb(btn, check_state_change_cb, LV_EVENT_VALUE_CHANGED, NULL);
}
LV_FONT_DECLARE(lv_font_semibold_14)
// 创建开关和标签，并绑定事件
void lv_switch_label_demo(void)
{
    // 获取当前屏幕
    create_checkable_btn();
}



