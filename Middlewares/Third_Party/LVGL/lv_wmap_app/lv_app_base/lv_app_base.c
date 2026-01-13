#include "lv_app_base.h"
#include "lvgl/lvgl.h"







#include "lvgl/lvgl.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*************************************************************************
 * 1. 基础配置
 *************************************************************************/
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define BAT_GROUPS    4
#define BATS_PER_GRP  12

// 颜色定义
#define COLOR_BG           lv_color_hex(0xF2F2F7)
#define COLOR_WHITE        lv_color_hex(0xFFFFFF)
#define COLOR_TEXT_MAIN    lv_color_hex(0x1C1C1E)
#define COLOR_TEXT_SUB     lv_color_hex(0x6e6e73)
#define COLOR_SHADOW       lv_color_hex(0xA0A0A5)
#define COLOR_METAL        lv_color_hex(0x8E8E93)

// 状态色
#define COLOR_ST_IDLE      lv_color_hex(0x9E9EA5)
#define COLOR_ST_CHG       lv_color_hex(0x34C759)
#define COLOR_ST_DSG       lv_color_hex(0xFF9500)
#define COLOR_ST_FULL      lv_color_hex(0x007AFF)
#define COLOR_ST_ERR       lv_color_hex(0xFF3B30) // 鲜艳的红
#define COLOR_ST_WARN      lv_color_hex(0xFFCC00)

/*************************************************************************
 * 2. 数据结构
 *************************************************************************/

typedef enum {
    CELL_IDLE = 0,
    CELL_CHG,
    CELL_DSG,
    CELL_FULL,
    CELL_ERR_GEN, // 通用故障
    CELL_ERR_OV,  // 过压故障
    CELL_ERR_UV   // 欠压故障
} cell_status_t;

// 组状态
typedef enum {
    GRP_ST_STANDBY = 0,
    GRP_ST_CHARGING,
    GRP_ST_DISCHARGING,
    GRP_ST_CHG_DONE,
    GRP_ST_DSG_DONE,
    GRP_ST_FAULT
} group_state_t;

typedef struct {
    int id;
    cell_status_t status;
    int percent;
    float voltage;
    float current;
} cell_data_t;

static cell_data_t all_cells[BAT_GROUPS][BATS_PER_GRP];
static group_state_t group_states[BAT_GROUPS];
static int current_view_group = 0;

// UI 句柄
static lv_obj_t * grid_container;
static lv_obj_t * label_sys_time;
static lv_obj_t * lbl_stat_max;
static lv_obj_t * lbl_stat_min;
static lv_obj_t * lbl_stat_sum;
static lv_obj_t * lbl_stat_diff;
static lv_obj_t * lbl_stat_state;
static lv_obj_t * group_indicators[BAT_GROUPS];

// 样式
static lv_style_t style_card;
static lv_style_t style_cell_bg;
static lv_style_t style_cell_liquid;
static lv_style_t style_btn_base;
static lv_style_t style_err_badge; // 新增：故障徽章样式

/*************************************************************************
 * 3. 模拟数据 (故意制造一些故障)
 *************************************************************************/
void init_bms_data(void) {
    // G1: 充电
    group_states[0] = GRP_ST_CHARGING;
    for(int i=0; i<BATS_PER_GRP; i++) {
        all_cells[0][i].id = i + 1;
        all_cells[0][i].status = CELL_CHG;
        all_cells[0][i].voltage = 4.0;
        all_cells[0][i].percent = 70;
    }

    // G2: 放电
    group_states[1] = GRP_ST_DISCHARGING;
    for(int i=0; i<BATS_PER_GRP; i++) {
        all_cells[1][i].id = 13 + i;
        all_cells[1][i].status = CELL_DSG;
        all_cells[1][i].voltage = 3.6;
        all_cells[1][i].percent = 40;
    }

    // G3: 系统故障 - 多节电池出问题
    group_states[2] = GRP_ST_FAULT;
    for(int i=0; i<BATS_PER_GRP; i++) {
        all_cells[2][i].id = 25 + i;
        all_cells[2][i].status = CELL_IDLE;
        all_cells[2][i].voltage = 4.15;
        all_cells[2][i].percent = 90;
    }
    // #26: 过压故障
    all_cells[2][1].status = CELL_ERR_OV; all_cells[2][1].voltage = 4.55;
    // #29: 断线/通用故障
    all_cells[2][4].status = CELL_ERR_GEN; all_cells[2][4].voltage = 0.00; all_cells[2][4].percent = 0;
    // #36: 欠压故障
    all_cells[2][11].status = CELL_ERR_UV; all_cells[2][11].voltage = 2.10; all_cells[2][11].percent = 0;

    // G4: 满电
    group_states[3] = GRP_ST_CHG_DONE;
    for(int i=0; i<BATS_PER_GRP; i++) {
        all_cells[3][i].id = 37 + i;
        all_cells[3][i].status = CELL_FULL;
        all_cells[3][i].voltage = 4.2;
        all_cells[3][i].percent = 100;
    }
}

/*************************************************************************
 * 4. 辅助函数
 *************************************************************************/
lv_color_t get_cell_color(cell_status_t st) {
    switch(st) {
        case CELL_CHG: return COLOR_ST_CHG;
        case CELL_DSG: return COLOR_ST_DSG;
        case CELL_FULL: return COLOR_ST_FULL;
        case CELL_ERR_GEN:
        case CELL_ERR_OV:
        case CELL_ERR_UV: return COLOR_ST_ERR; // 故障统一红色液体基调，靠图标区分
        default: return COLOR_ST_IDLE;
    }
}

lv_color_t get_group_color(group_state_t st) {
    if(st == GRP_ST_FAULT) return COLOR_ST_ERR;
    if(st == GRP_ST_CHARGING) return COLOR_ST_CHG;
    if(st == GRP_ST_DISCHARGING) return COLOR_ST_DSG;
    if(st == GRP_ST_CHG_DONE) return COLOR_ST_FULL;
    return COLOR_ST_IDLE;
}

const char* get_group_str(group_state_t st) {
    if(st == GRP_ST_FAULT) return "FAULT!";
    if(st == GRP_ST_CHARGING) return "Charging";
    if(st == GRP_ST_DISCHARGING) return "Discharging";
    if(st == GRP_ST_CHG_DONE) return "Full";
    return "Standby";
}

bool is_cell_fault(cell_status_t st) {
    return (st >= CELL_ERR_GEN);
}

/*************************************************************************
 * 5. 样式初始化
 *************************************************************************/
void init_styles_v9(void) {
    lv_style_init(&style_card);
    lv_style_set_bg_color(&style_card, COLOR_WHITE);
    lv_style_set_radius(&style_card, 0);
    lv_style_set_shadow_width(&style_card, 15);
    lv_style_set_shadow_color(&style_card, COLOR_SHADOW);
    lv_style_set_shadow_opa(&style_card, LV_OPA_20);

    lv_style_init(&style_cell_bg);
    lv_style_set_bg_color(&style_cell_bg, COLOR_WHITE);
    lv_style_set_radius(&style_cell_bg, 14);
    lv_style_set_shadow_width(&style_cell_bg, 15);
    lv_style_set_shadow_color(&style_cell_bg, COLOR_SHADOW);
    lv_style_set_shadow_opa(&style_cell_bg, LV_OPA_30);
//    lv_style_set_shadow_offset_y(&style_cell_bg, 5);
lv_obj_set_style_shadow_ofs_y(style_cell_bg, 5, LV_STATE_DEFAULT);
    lv_style_init(&style_cell_liquid);
    lv_style_set_radius(&style_cell_liquid, 10);
    lv_style_set_bg_grad_dir(&style_cell_liquid, LV_GRAD_DIR_HOR);

    // 新增：故障徽章样式 (圆形，红色背景，白色边框)
    lv_style_init(&style_err_badge);
    lv_style_set_radius(&style_err_badge, LV_RADIUS_CIRCLE);
    lv_style_set_bg_color(&style_err_badge, COLOR_ST_ERR);
    lv_style_set_border_width(&style_err_badge, 2);
    lv_style_set_border_color(&style_err_badge, COLOR_WHITE);
    lv_style_set_shadow_width(&style_err_badge, 10);
    lv_style_set_shadow_color(&style_err_badge, lv_color_hex(0x800000));
    lv_style_set_shadow_opa(&style_err_badge, LV_OPA_50);

    lv_style_init(&style_btn_base);
    lv_style_set_radius(&style_btn_base, 8);
    lv_style_set_bg_color(&style_btn_base, COLOR_WHITE);
    lv_style_set_shadow_width(&style_btn_base, 10);
    lv_style_set_shadow_color(&style_btn_base, COLOR_SHADOW);
    lv_style_set_shadow_opa(&style_btn_base, LV_OPA_30);
}

/*************************************************************************
 * 6. 核心绘制逻辑 (含故障图标)
 *************************************************************************/
void create_battery_cell(lv_obj_t * parent, int cell_idx) {
    // 变量声明 (C89)
    cell_data_t * data;
    lv_color_t color;
    bool is_fault;
    lv_obj_t * cont;
    lv_obj_t * tip;
    lv_obj_t * shell;
    lv_obj_t * fluid;
    lv_obj_t * lbl_id;
    lv_obj_t * lbl_v;
    lv_obj_t * lbl_info; // 用于显示状态或电流
    lv_obj_t * lbl_p;

    // 数据获取
    data = &all_cells[current_view_group][cell_idx];
    color = get_cell_color(data->status);
    is_fault = is_cell_fault(data->status);

    // 容器
    cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 110, 190);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    // 正极头
    tip = lv_obj_create(cont);
    lv_obj_set_size(tip, 40, 15);
    lv_obj_align(tip, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(tip, COLOR_METAL, 0);
    lv_obj_set_style_bg_opa(tip, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(tip, 4, 0);

    // 外壳
    shell = lv_obj_create(cont);
    lv_obj_add_style(shell, &style_cell_bg, 0);
    lv_obj_set_size(shell, 110, 175);
    lv_obj_align(shell, LV_ALIGN_TOP_MID, 0, 15);

    // 如果是故障，给外壳增加粗红框，进一步强调
    if(is_fault) {
        lv_obj_set_style_border_width(shell, 3, 0);
        lv_obj_set_style_border_color(shell, COLOR_ST_ERR, 0);
    }

    // 液体条
    fluid = lv_bar_create(shell);
    lv_obj_add_style(fluid, &style_cell_liquid, LV_PART_INDICATOR);

    // 如果是故障，液体用纯色或淡红色；正常用亮色渐变
    lv_obj_set_style_bg_color(fluid, color, LV_PART_INDICATOR);
    if(is_fault) {
        lv_obj_set_style_bg_grad_color(fluid, lv_color_mix(lv_color_black(), color, 20), LV_PART_INDICATOR); // 暗淡一点
    } else {
        lv_obj_set_style_bg_grad_color(fluid, lv_color_mix(COLOR_WHITE, color, 80), LV_PART_INDICATOR); // 亮泽
    }

    lv_obj_set_style_bg_opa(fluid, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(fluid, 94, 159);
    lv_obj_center(fluid);
    lv_bar_set_range(fluid, 0, 100);
    lv_bar_set_value(fluid, data->percent, LV_ANIM_OFF);

    // ============= 信息层 =============

    // 编号 #01
    lbl_id = lv_label_create(shell);
    lv_label_set_text_fmt(lbl_id, "#%02d", data->id);
    lv_obj_set_style_text_font(lbl_id, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lbl_id, COLOR_TEXT_SUB, 0);
    lv_obj_align(lbl_id, LV_ALIGN_TOP_LEFT, 5, 5);

    // 电压 (如果断线/Generic Fault显示 0.00V)
    lbl_v = lv_label_create(shell);
    lv_label_set_text_fmt(lbl_v, "%.2fV", data->voltage);
    lv_obj_set_style_text_font(lbl_v, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(lbl_v, COLOR_TEXT_MAIN, 0);
    lv_obj_align(lbl_v, LV_ALIGN_TOP_MID, 0, 30);

    // 中部图标或信息
    // 逻辑：如果是故障，显示大的红色图标；如果正常，显示状态文本或电流
    if(is_fault) {
        // --- 绘制故障徽章 ---
        lv_obj_t * badge = lv_obj_create(shell);
        lv_obj_add_style(badge, &style_err_badge, 0);
        lv_obj_set_size(badge, 40, 40);
        lv_obj_align(badge, LV_ALIGN_CENTER, 0, -5); // 居中微上

        lv_obj_t * icon = lv_label_create(badge);
        // 使用 "X" 符号 (LV_SYMBOL_CLOSE) 或者 "!" (LV_SYMBOL_WARNING)
        lv_label_set_text(icon, LV_SYMBOL_CLOSE);
        lv_obj_set_style_text_color(icon, COLOR_WHITE, 0);
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_20, 0); // 符号大小
        lv_obj_center(icon);

        // 故障描述 (放到下面去，代替电流)
        lbl_info = lv_label_create(shell);
        if(data->status == CELL_ERR_OV) lv_label_set_text(lbl_info, "OVER-V");
        else if(data->status == CELL_ERR_UV) lv_label_set_text(lbl_info, "UNDER-V");
        else lv_label_set_text(lbl_info, "ERROR");

        lv_obj_set_style_text_color(lbl_info, COLOR_ST_ERR, 0);
        lv_obj_set_style_text_font(lbl_info, &lv_font_montserrat_14, 0); // 加粗或大点
        lv_obj_align(lbl_info, LV_ALIGN_BOTTOM_MID, 0, -35); // 放下面

    } else {
        // --- 正常显示电流/状态 ---
        lbl_info = lv_label_create(shell);
        if(data->status == CELL_CHG) {
             lv_label_set_text(lbl_info, "Chg...");
             lv_obj_set_style_text_color(lbl_info, COLOR_ST_CHG, 0);
        } else if(data->status == CELL_DSG) {
             lv_label_set_text(lbl_info, "Dsg...");
             lv_obj_set_style_text_color(lbl_info, COLOR_ST_DSG, 0);
        } else {
             lv_label_set_text(lbl_info, "Idle");
             lv_obj_set_style_text_color(lbl_info, COLOR_TEXT_SUB, 0);
        }
        lv_obj_set_style_text_font(lbl_info, &lv_font_montserrat_12, 0);
        lv_obj_align(lbl_info, LV_ALIGN_CENTER, 0, 5);
    }

    // 百分比 (Bottom)
    lbl_p = lv_label_create(shell);
    lv_label_set_text_fmt(lbl_p, "%d%%", data->percent);
    lv_obj_set_style_text_font(lbl_p, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(lbl_p, COLOR_TEXT_MAIN, 0);
    lv_obj_align(lbl_p, LV_ALIGN_BOTTOM_MID, 0, -10);
}

/*************************************************************************
 * 7. 更新逻辑
 *************************************************************************/
void update_header_leds(void) {
    int i;
    for(i=0; i<BAT_GROUPS; i++) {
        lv_color_t color = get_group_color(group_states[i]);
        lv_obj_set_style_bg_color(group_indicators[i], color, 0);

        // 选中组高亮边框
        if(i == current_view_group) {
            lv_obj_set_style_border_width(group_indicators[i], 3, 0);
            lv_obj_set_style_border_color(group_indicators[i], COLOR_TEXT_MAIN, 0);
        } else {
            lv_obj_set_style_border_width(group_indicators[i], 0, 0);
        }
    }
}

void update_stats(void) {
    float min_v = 100.0f;
    float max_v = 0.0f;
    float sum_v = 0.0f;
    int i;

    for(i=0; i<BATS_PER_GRP; i++) {
        float v = all_cells[current_view_group][i].voltage;
        if(v < min_v) min_v = v;
        if(v > max_v) max_v = v;
        sum_v += v;
    }

    lv_label_set_text_fmt(lbl_stat_max, "Max: %.3fV", max_v);
    lv_label_set_text_fmt(lbl_stat_min, "Min: %.3fV", min_v);
    lv_label_set_text_fmt(lbl_stat_sum, "Total: %.1fV", sum_v);
    lv_label_set_text_fmt(lbl_stat_diff,"Diff: %.3fV", max_v - min_v);

    group_state_t st = group_states[current_view_group];
    lv_label_set_text(lbl_stat_state, get_group_str(st));
    lv_obj_set_style_text_color(lbl_stat_state, get_group_color(st), 0);
}

void refresh_view(void) {
    int i;
    lv_obj_clean(grid_container);
    for(i=0; i<BATS_PER_GRP; i++) {
        create_battery_cell(grid_container, i);
    }
    update_stats();
    update_header_leds();
}

static void nav_click_cb(lv_event_t * e) {
    int id = (int)(intptr_t)lv_event_get_user_data(e);
    if(id != current_view_group) {
        current_view_group = id;
        refresh_view();
    }
}

static void prev_next_cb(lv_event_t * e) {
    int dir = (int)(intptr_t)lv_event_get_user_data(e);
    int next = current_view_group + dir;
    if(next < 0) next = BAT_GROUPS - 1;
    if(next >= BAT_GROUPS) next = 0;
    current_view_group = next;
    refresh_view();
}

static void timer_update_cb(lv_timer_t * t) {
    time_t now;
    struct tm * ti;
    time(&now);
    ti = localtime(&now);
    if(label_sys_time) {
        lv_label_set_text_fmt(label_sys_time, "%04d-%02d-%02d %02d:%02d:%02d",
            ti->tm_year+1900, ti->tm_mon+1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
    }
}

/*************************************************************************
 * 8. UI 初始化
 *************************************************************************/
void ui_create(void) {
    // 变量声明
    lv_obj_t * scr = lv_scr_act();
    lv_obj_t * header;
    lv_obj_t * lbl_main;
    lv_obj_t * leds_area;
    lv_obj_t * led_btn;
    lv_obj_t * led_lbl;
    lv_obj_t * info_bar;
    lv_obj_t * footer;
    lv_obj_t * btn_p;
    lv_obj_t * btn_n;
    lv_obj_t * btn_lbl;
    int i;

    init_bms_data();
    init_styles_v9();
    lv_obj_set_style_bg_color(scr, COLOR_BG, 0);

    // Header
    header = lv_obj_create(scr);
    lv_obj_add_style(header, &style_card, 0);
    lv_obj_set_size(header, 800, 70);
    lv_obj_set_pos(header, 0, 0);

    lbl_main = lv_label_create(header);
    lv_label_set_text(lbl_main, "BMS Monitor System");
    lv_obj_set_style_text_font(lbl_main, &lv_font_montserrat_20, 0);
    lv_obj_align(lbl_main, LV_ALIGN_LEFT_MID, 20, 0);

    label_sys_time = lv_label_create(header);
    lv_label_set_text(label_sys_time, "0000-00-00 00:00:00");
    lv_obj_set_style_text_font(label_sys_time, &lv_font_montserrat_16, 0);
    lv_obj_align(label_sys_time, LV_ALIGN_CENTER, -30, 0);

    // G1-G4 灯区域
    leds_area = lv_obj_create(header);
    lv_obj_remove_style_all(leds_area);
    lv_obj_set_size(leds_area, 260, 60);
    lv_obj_align(leds_area, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_flex_flow(leds_area, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(leds_area, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(leds_area, 10, 0);

    for(i=0; i<BAT_GROUPS; i++) {
        led_btn = lv_btn_create(leds_area);
        lv_obj_set_size(led_btn, 50, 40);
        lv_obj_set_style_radius(led_btn, 6, 0);
        lv_obj_add_event_cb(led_btn, nav_click_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);

        led_lbl = lv_label_create(led_btn);
        lv_label_set_text_fmt(led_lbl, "G%d", i+1);
        lv_obj_center(led_lbl);

        group_indicators[i] = led_btn;
    }

    // Info Bar
    info_bar = lv_obj_create(scr);
    lv_obj_remove_style_all(info_bar);
    lv_obj_set_size(info_bar, 800, 40);
    lv_obj_set_pos(info_bar, 0, 75);
    lv_obj_set_flex_flow(info_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(info_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lbl_stat_max = lv_label_create(info_bar);
    lbl_stat_min = lv_label_create(info_bar);
    lbl_stat_sum = lv_label_create(info_bar);
    lbl_stat_diff = lv_label_create(info_bar);
    lbl_stat_state = lv_label_create(info_bar);
    lv_obj_set_style_text_font(lbl_stat_state, &lv_font_montserrat_16, 0);

    // Grid Area
    grid_container = lv_obj_create(scr);
    lv_obj_remove_style_all(grid_container);
    lv_obj_set_size(grid_container, 800, 390);
    lv_obj_set_pos(grid_container, 0, 115);
    lv_obj_set_flex_flow(grid_container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(grid_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(grid_container, 15, 0);

    // Footer
    footer = lv_obj_create(scr);
    lv_obj_add_style(footer, &style_card, 0);
    lv_obj_set_size(footer, 800, 90);
    lv_obj_set_pos(footer, 0, 510);
    lv_obj_set_flex_flow(footer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(footer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    btn_p = lv_btn_create(footer);
    lv_obj_add_style(btn_p, &style_btn_base, 0);
    lv_obj_set_size(btn_p, 120, 50);
    btn_lbl = lv_label_create(btn_p);
    lv_label_set_text(btn_lbl, "< PREV GRP");
    lv_obj_center(btn_lbl);
    lv_obj_add_event_cb(btn_p, prev_next_cb, LV_EVENT_CLICKED, (void*)(intptr_t)-1);

    btn_n = lv_btn_create(footer);
    lv_obj_add_style(btn_n, &style_btn_base, 0);
    lv_obj_set_size(btn_n, 120, 50);
    btn_lbl = lv_label_create(btn_n);
    lv_label_set_text(btn_lbl, "NEXT GRP >");
    lv_obj_center(btn_lbl);
    lv_obj_add_event_cb(btn_n, prev_next_cb, LV_EVENT_CLICKED, (void*)(intptr_t)1);

    lv_timer_create(timer_update_cb, 500, NULL);

    // First Refresh
    refresh_view();
}

















// 主入口
void ui_init(void) {
ui_create();
}















