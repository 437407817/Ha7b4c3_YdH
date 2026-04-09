


#include "../ui.h"

// #include "ui_S_page01.h"



lv_obj_t * ui_S_page01_screen = NULL;
lv_obj_t * scroll_view = NULL;


// --- 新增：静态变量定义 ---
static lv_obj_t * ui_input_stop_vol = NULL;
static lv_obj_t * ui_input_vol_dif = NULL;
static lv_obj_t * ui_input_vol_h_alarm = NULL;

static lv_obj_t * ui_auto_checkbox = NULL;
static lv_obj_t * ui_global_kb = NULL;






// ui_S_page01.c 顶部
lv_obj_t * ui_battery_labels[64] = {0}; // 用于存放 64 个电压 Label 的指针
lv_obj_t * ui_battery_cells[64] = {0};  // 用于存放 64 个单元格容器的指针（改颜色用）

// 修改后的创建函数（只在初始化时运行一次）
//void display_64_values_init(lv_obj_t *parent) {
//    lv_obj_t *container = lv_obj_create(parent);
//    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
//    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
//    lv_obj_set_style_pad_all(container, 5, 0);

//    for (int i = 0; i < 64; i++) {
//        // 1. 创建单元格
//        lv_obj_t *cell = lv_obj_create(container);
//        ui_battery_cells[i] = cell;
//        lv_obj_set_size(cell, 70, 60);
//        lv_obj_set_style_border_width(cell, 1, 0);
//        
//        // 2. 创建编号（不需要更新，所以不用存指针）
//        lv_obj_t *index_label = lv_label_create(cell);
//        lv_label_set_text_fmt(index_label, "%d", i + 1);
//        lv_obj_align(index_label, LV_ALIGN_TOP_LEFT, 2, 2);
//        
//        // 3. 创建电压 Label（关键：存入数组供后续更新）
//        ui_battery_labels[i] = lv_label_create(cell);
//        lv_label_set_text(ui_battery_labels[i], "0.000V"); // 初始值
//        lv_obj_align(ui_battery_labels[i], LV_ALIGN_CENTER, 0, 0);
//    }
//}


void display_64_values_init(lv_obj_t *parent) {
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_all(container, 5, 0);
	
//	lv_obj_set_style_bg_color(container, lv_color_hex(0x000CCC), LV_PART_MAIN | LV_STATE_DEFAULT);
//  lv_obj_set_style_bg_opa(container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    for (int i = 0; i < 64; i++) {
        // 1. 创建单元格
        lv_obj_t *cell = lv_obj_create(container);
        ui_battery_cells[i] = cell;
        lv_obj_set_size(cell, 70, 60);
        lv_obj_set_style_border_width(cell, 1, 0);
        // 关键：消除 cell 默认的内边距，否则对齐会偏移
        lv_obj_set_style_pad_all(cell, 0, 0); 
        
        // 2. 创建编号（左上角）
        lv_obj_t *index_label = lv_label_create(cell);
        lv_label_set_text_fmt(index_label, "%d", i + 1);
        lv_obj_set_style_text_font(index_label, &lv_font_montserrat_12, 0);
        // 明确对齐到左上角，微调坐标 (2, 2)
        lv_obj_align(index_label, LV_ALIGN_TOP_LEFT, 2, 2);
        
        // 3. 创建电压 Label（居中靠下）
        ui_battery_labels[i] = lv_label_create(cell);
        lv_label_set_text(ui_battery_labels[i], "0.000V");
        lv_obj_set_style_text_font(ui_battery_labels[i], &lv_font_montserrat_14, 0);
        
        /* * 解决重合的关键：
         * 使用 LV_ALIGN_CENTER 会让它处于正中心。
         * 如果想避开左上角的标号，可以：
         * 1. 使用 LV_ALIGN_BOTTOM_MID (底部居中)
         * 2. 或者使用 LV_ALIGN_CENTER 配合 y 轴偏移
         */
        lv_obj_align(ui_battery_labels[i], LV_ALIGN_CENTER, 0, 5); // 向下偏移 5 像素
    }
}

// 新增：专门用于刷新的函数（由 LVGL Timer 调用）
//void ui_S_page01_update_values(uint16_t values[64], uint64_t color_flags) {
//    for (int i = 0; i < 64; i++) {
//        if (ui_battery_labels[i] == NULL) continue; // 安全检查

//        // 只更新文本，不创建任何新对象！
//        lv_label_set_text_fmt(ui_battery_labels[i], "%d.%03dV", values[i] / 1000, values[i] % 1000);

//        // 更新背景颜色逻辑
//        uint64_t bit_mask = (uint64_t)1 << i;
//        if (color_flags & bit_mask) {
//            lv_obj_set_style_bg_color(ui_battery_cells[i], lv_color_hex(0x0000FF), 0);
//            lv_obj_set_style_text_color(ui_battery_labels[i], lv_color_hex(0xFFFFFF), 0);
//        } else {
//            lv_obj_set_style_bg_color(ui_battery_cells[i], lv_color_hex(0xFFFFFF), 0);
//            lv_obj_set_style_text_color(ui_battery_labels[i], lv_color_hex(0x000000), 0);
//        }
//    }
//}


// ui_S_page01.c

void ui_S_page01_update_values(uint16_t values[64], uint64_t color_flags) {
    int max_idx = -1;
    int min_idx = -1;
    uint16_t max_val = 0;
    uint16_t min_val = 0xFFFF;

    // 1. 第一遍遍历：更新所有 64 个 Label 的文字和背景色
    for (int i = 0; i < 64; i++) {
        if (ui_battery_labels[i] == NULL || ui_battery_cells[i] == NULL) continue;

        // 更新电压文字（所有 64 路均显示）
        lv_label_set_text_fmt(ui_battery_labels[i], "%d.%03dV", values[i] / 1000, values[i] % 1000);

        // 更新背景颜色（根据 color_flags 位图）
        uint64_t bit_mask = (uint64_t)1 << i;
        if (color_flags & bit_mask) {
            lv_obj_set_style_bg_color(ui_battery_cells[i], lv_color_hex(0x0000FF), 0);
            lv_obj_set_style_text_color(ui_battery_labels[i], lv_color_hex(0xFFFFFF), 0);
        } else {
            lv_obj_set_style_bg_color(ui_battery_cells[i], lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_text_color(ui_battery_labels[i], lv_color_hex(0x000000), 0);
        }

        // 默认恢复普通边框（清除上一轮标记）
        lv_obj_set_style_border_color(ui_battery_cells[i], lv_color_hex(0xCCCCCC), 0);
        lv_obj_set_style_border_width(ui_battery_cells[i], 1, 0);

        // --- 核心修改：仅在前 BATNUM (52) 个中寻找最值 ---
        if (i < lvgl_BATNUM) {
            if (values[i] > max_val) {
                max_val = values[i];
                max_idx = i;
            }
            if (values[i] < min_val) {
                min_val = values[i];
                min_idx = i;
            }
        }
    }

    // 2. 第二步：高亮前 52 路中的最值边框
    if (max_idx != -1) {
        lv_obj_set_style_border_width(ui_battery_cells[max_idx], 2, 0);
        lv_obj_set_style_border_color(ui_battery_cells[max_idx], lv_color_hex(0xFF0000), 0); // 红色最高
				lv_obj_set_style_text_color(ui_battery_labels[max_idx], lv_color_hex(0xFF0000), 0);
    }
    if (min_idx != -1 && min_idx != max_idx) {
        lv_obj_set_style_border_width(ui_battery_cells[min_idx], 2, 0);
        lv_obj_set_style_border_color(ui_battery_cells[min_idx], lv_color_hex(0xFFD700) , 0); // 黄色最低
			lv_obj_set_style_text_color(ui_battery_labels[min_idx], lv_color_hex(0xFFD700), 0);
    }
}










// 键盘事件回调函数
//static void keyboard_event_cb(lv_event_t *e) {
//    lv_obj_t *ta = lv_event_get_target(e);
//    lv_obj_t *kb = lv_event_get_user_data(e);
//    lv_keyboard_set_textarea(kb, ta);
//    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
//}

//static void keyboard_event_cb(lv_event_t * e) {
//    lv_obj_t * ta = lv_event_get_target(e);
//    lv_obj_t * kb = lv_event_get_user_data(e);

//    if(lv_event_get_code(e) == LV_EVENT_FOCUSED) {
//        lv_keyboard_set_textarea(kb, ta);
//        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
//        lv_obj_move_foreground(kb); // 确保键盘在最上层
//    }
//    
//    if(lv_event_get_code(e) == LV_EVENT_DEFOCUSED) {
//        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
//    }
//}



//static void keyboard_event_cb(lv_event_t * e) {
//    lv_obj_t * obj = lv_event_get_target(e); // 触发事件的对象（可能是ta也可能是kb）
//    lv_obj_t * kb = lv_event_get_user_data(e);
//    lv_event_code_t code = lv_event_get_code(e);

//    if(code == LV_EVENT_FOCUSED) {
//        lv_keyboard_set_textarea(kb, obj);
//        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
//        lv_obj_move_foreground(kb); // 确保键盘不被其他控件遮挡
//    }
//    
//    // 当点击键盘上的“对勾”或“取消”时隐藏
//    if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
//        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
//        lv_indev_reset(NULL, obj); // 让输入框失去焦点，防止无法再次触发 FOCUSED
//    }
//}

static void keyboard_event_cb(lv_event_t * e) {
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e); // 获取回调关联的键盘对象
    lv_obj_t * ta = lv_event_get_target(e);               // 获取当前触发事件的对象（通常是输入框）
    lv_event_code_t code = lv_event_get_code(e);

    // --- 1. 处理输入框获得焦点：弹出键盘 ---
    if(code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(kb, ta);           // 将键盘与当前点击的输入框绑定
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);  // 移除隐藏标志，显示键盘
        
        /* LVGL 9.4 置顶写法：将对象移动到子列表最后，即显示在最前端 */
        lv_obj_move_to_index(kb, -1); 
    }

    // --- 2. 处理点击键盘上的“确定(对勾)”或“取消(叉号)”按钮：隐藏键盘 ---
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);    // 添加隐藏标志，关掉键盘
        
        /* * 关键逻辑：重置输入设备的焦点状态。
         * 如果不重置，输入框会一直处于 FOCUSED 状态，
         * 下次再点击同一个输入框时，将不会再次触发 LV_EVENT_FOCUSED，导致键盘无法弹出。
         */
			
        lv_indev_t * indev = lv_indev_active();
			
        if(indev) {
            lv_indev_reset(indev, ta); 
        }
    }
}


static void ui_screen_click_event_cb(lv_event_t * e) {
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_obj_t * target = lv_event_get_target(e); // 获取当前点击的真实对象

    // 如果键盘当前是显示的
    if(!lv_obj_has_flag(kb, LV_OBJ_FLAG_HIDDEN)) {
        // 判断点击的对象：
        // 1. 不是键盘本身
        // 2. 不是那两个输入框
        if(target != kb && target != ui_input_stop_vol && target != ui_input_vol_dif) {
            lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN); // 隐藏键盘
            
            // 手动清除所有输入框的焦点状态，确保下次点击能正常弹出
            lv_indev_t * indev = lv_indev_active();
            if(indev) lv_indev_reset(indev, NULL);
        }
    }
}



// 点击外部事件回调函数
static void click_outside_event_cb(lv_event_t *e) {
    lv_obj_t *kb = lv_event_get_user_data(e);
    if(!lv_obj_has_flag(kb, LV_OBJ_FLAG_HIDDEN)) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

// 按钮1点击回调函数
static void btn1_event_cb(lv_event_t *e) {
    v_printf("Button 1 clicked\n");
	#if !USE_LVGL_PC_SIMULATION
	GV_send_run_state_data.Machine_RUN=1;
	xSemaphoreGive(g_btn_task_sem);
	#endif
}

// 按钮2点击回调函数
static void btn2_event_cb(lv_event_t *e) {
    v_printf("Button 2 clicked\n");
	#if !USE_LVGL_PC_SIMULATION
	GV_send_run_state_data.Machine_RUN=0;
	xSemaphoreGive(g_btn_task_sem);
		#endif
}



// 字符串转 3位小数放大整数（1.5 → 1500，0.01 → 10）
int str_to_int_3dec(const char *str)
{
    int num = 0;
    int dec = 0;
    int point_found = 0;

    if (str == NULL) return 0;

    while (*str != '\0')
    {
        if (*str >= '0' && *str <= '9')
        {
            num = num * 10 + (*str - '0');
            if (point_found)
            {
                dec++;
                if (dec >= 3) break; // 只取3位小数
            }
        }
        else if (*str == '.')
        {
            point_found = 1; // 遇到小数点
        }
        str++;
    }

    // 补足 3 位小数
    while (dec < 3)
    {
        num *= 10;
        dec++;
    }

    return num;
}


// 按钮3点击回调函数

static void btn3_event_cb(lv_event_t *e) {
    // 获取输入框字符串
    const char * str_stop = lv_textarea_get_text(ui_input_stop_vol);
    const char * str_dif = lv_textarea_get_text(ui_input_vol_dif);
    const char * str_alarm_vol = lv_textarea_get_text(ui_input_vol_h_alarm);    
    // 转换为浮点数
    float val_stop = atof(str_stop);
    float val_dif = atof(str_dif);
    float val_alarm_vol = atof(str_alarm_vol);
    
	int val_int_stop = str_to_int_3dec(str_stop);
	int val_int_dif = str_to_int_3dec(str_dif);
	int val_int_alarm_vol = str_to_int_3dec(str_alarm_vol);
	
    // 正确代码（LVGL 9.0 ~ 9.5 通用）
		bool is_auto = lv_obj_has_state(ui_auto_checkbox, LV_STATE_CHECKED);

    // 校验范围 0.1 - 5.0
    if(val_stop < 0.1f || val_stop > 5.0f ) {
        v_printf("Error Stop=%.3f: val_stop Range must be 0.1 ~ 5.0V\n",val_stop);
        // 视觉反馈：边框变红
        lv_obj_set_style_border_color(ui_input_stop_vol, lv_color_hex(0xFF0000), 0);
        return;
    }
		    if( val_dif < 0.001f || val_dif > 3.0f) {
        v_printf("Error Dif=%.3f: val_dif Range must be 0.001 ~ 3.0V\n",val_dif);
        // 视觉反馈：边框变红
        lv_obj_set_style_border_color(ui_input_vol_dif, lv_color_hex(0xFF0000), 0);
        return;
    }

    if(val_alarm_vol < val_stop) {
        v_printf("Error Alarm Vol=%.3f: val_alarm_vol must be greater than val_stop\n",val_alarm_vol);
        // 视觉反馈：边框变红
        lv_obj_set_style_border_color(ui_input_vol_h_alarm, lv_color_hex(0xFF0000), 0);
        return;
    }


    // 发送数据（假设你已经创建了对应的 Queue）
    // ctrl_data_msg_t msg = {val_stop, val_dif, is_auto};
    // xQueueSend(g_ctrl_data_queue, &msg, 0);
    
    v_printf("Data Sent: Stop=%.3f, Dif=%.3f, Alarm Vol=%.3f, Auto=%d\n", val_stop, val_dif, val_alarm_vol, is_auto);
		v_printf("Data Sent: Stop=%d, Dif=%d, Auto=%d\n", val_int_stop, val_int_dif, val_int_alarm_vol, is_auto);
		
		
    lv_obj_set_style_border_color(ui_input_stop_vol, lv_color_hex(0xCCCCCC), 0);
		lv_obj_set_style_border_color(ui_input_vol_dif, lv_color_hex(0xCCCCCC), 0);
		lv_obj_set_style_border_color(ui_input_vol_h_alarm, lv_color_hex(0xCCCCCC), 0);
		
#if USE_LVGL_PC_SIMULATION

#else
		
		STR_SEND_SETTING_DATA_t setting_msg={is_auto,0,val_int_stop,val_int_dif,val_int_alarm_vol};
		
//		g_setting_data_queue
		// 3. 发送到队列
    if(g_setting_data_queue != NULL) {
        // 参数1: 队列句柄
        // 参数2: 要发送的数据指针
        // 参数3: 等待时间。在 UI 任务中建议设为 0，即如果队列满了立刻跳过，不阻塞 UI 刷新
        BaseType_t xStatus = xQueueSend(g_setting_data_queue, &setting_msg, 0);

        if(xStatus == pdPASS) {
            v_printf("UI: Data sent to queue success\n");
        } else {
            v_printf("UI: Queue full, send failed\n");
        }
    }
		
#endif		
}


// 页面切换回调函数
static void switch_page_event_cb(lv_event_t *e) {
    lv_disp_load_scr(ui_S_page02_screen);
}





#if 0

// 显示电池电压的函数
void display_64_values(lv_obj_t *parent, uint16_t values[64], uint64_t color_flags, int count) {
    // 网格布局参数
    const int grid_size = 8; // 8x8网格
    const int cell_size = 60; // 每个单元格大小
    const int padding = 5; // 单元格间距
    const int max_value = 5000; // 最大值
    
    // 创建容器用于放置所有数值控件
    lv_obj_t *container = lv_obj_create(parent);
    // lv_obj_set_size(container, grid_size * (cell_size + padding) - padding, 
    //                grid_size * (cell_size + padding) - padding);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    
    // 确保count不超过64
    if (count > 64) count = 64;
    if (count < 1) count = 1;
    
    // 存储电池控件和电压值
    lv_obj_t *cells[64] = {0};
    int voltages[64] = {0};
    
    // 创建电池控件
    for (int i = 0; i < count; i++) {
        // 创建单个数值控件的容器
        lv_obj_t *cell = lv_obj_create(container);
        cells[i] = cell;
        lv_obj_set_size(cell, cell_size+10, cell_size);
        lv_obj_set_style_border_width(cell, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(cell, lv_color_hex(0xCCCCCC), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(cell, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(cell, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // 计算电压值
        int voltage = values[i];
        voltages[i] = voltage;
        
        // 检查颜色标志位（0-63位对应64个控件）
        uint64_t bit_mask = (uint64_t)1 << i;
        
        // 设置背景颜色
        if (color_flags & bit_mask) {
            // 蓝色
            lv_obj_set_style_bg_color(cell, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(cell, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            // 白色
            lv_obj_set_style_bg_color(cell, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(cell, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        
        // 添加标号（1-64）
        lv_obj_t *index_label = lv_label_create(cell);
        lv_label_set_text_fmt(index_label, "%d", i + 1); // 从1开始编号
        lv_obj_align(index_label, LV_ALIGN_TOP_LEFT, 2, 2);
        lv_obj_set_style_text_font(index_label, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // 创建标签显示数值，转换为电压格式（除以1000，显示为X.XXXV）
        lv_obj_t *label = lv_label_create(cell);
         //lv_label_set_text_fmt(label, "%.3fV", voltage);
        lv_label_set_text_fmt(label, "%d.%03dV", voltages[i] / 1000, voltages[i] % 1000);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    
    // 找出最高和最低电压
    if (count > 0) {
        int max_idx = 0;
        int min_idx = 0;
        
        for (int i = 1; i < count; i++) {
            if (voltages[i] > voltages[max_idx]) {
                max_idx = i;
            }
            if (voltages[i] < voltages[min_idx]) {
                min_idx = i;
            }
        }
        
        // 设置最高电压为红色边框
        if (cells[max_idx]) {
            // 保持原有的背景颜色
            // 设置边框为红色
            lv_obj_set_style_border_width(cells[max_idx], 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(cells[max_idx], lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        
        // 设置最低电压为灰色边框
        if (cells[min_idx] && min_idx != max_idx) { // 避免最高和最低是同一个的情况
            // 保持原有的背景颜色
            // 设置边框为浅绿色
            lv_obj_set_style_border_width(cells[min_idx], 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(cells[min_idx], lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}
#endif



void ui_S_page01_screen_init(void) {
    ui_S_page01_screen = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_S_page01_screen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // 清除默认边距
    lv_obj_set_style_pad_all(ui_S_page01_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);



    // lv_obj_t * page01_header = lv_obj_create(ui_S_page01_screen);
    // lv_obj_remove_style_all(page01_header);
    // lv_obj_set_height(page01_header, lv_pct(80));
    // lv_obj_set_width(page01_header, lv_pct(100));
    // lv_obj_set_align(page01_header, LV_ALIGN_TOP_MID);
    // lv_obj_remove_flag(page01_header, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags   



    // lv_obj_t *btn0 = lv_button_create(ui_S_page01_screen);
    // lv_obj_set_size(btn0, 10, 10);
    // lv_obj_t *btn0_label = lv_label_create(btn0);
    // lv_label_set_text(btn0_label, "start1");
    // lv_obj_center(btn0_label);
    // lv_obj_set_pos(btn0, 0, 0);
 lv_obj_remove_style_all(ui_S_page01_screen);
 lv_obj_remove_flag(ui_S_page01_screen, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // 设置为弹性布局的容器
    lv_obj_set_layout(ui_S_page01_screen, LV_LAYOUT_FLEX);

    // 垂直列布局 + 永不换行
lv_obj_set_flex_flow(ui_S_page01_screen, LV_FLEX_FLOW_COLUMN);  // 关键：COLUMN = 垂直列
lv_obj_set_flex_align(ui_S_page01_screen,
    LV_FLEX_ALIGN_START,   // 水平方向：靠左
    LV_FLEX_ALIGN_START,   // 垂直方向：靠上
    LV_FLEX_ALIGN_START
);
    // 1. 修改整体的内边距
    lv_obj_set_style_pad_all(ui_S_page01_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_row(ui_S_page01_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(ui_S_page01_screen, 0, LV_PART_MAIN);
    // 设置屏幕大小为整个显示区域
    lv_display_t *disp = lv_display_get_default();
    if(disp) {
        lv_coord_t w = lv_display_get_horizontal_resolution(disp);
        lv_coord_t h = lv_display_get_vertical_resolution(disp);
        lv_obj_set_size(ui_S_page01_screen, w, h);
    }

        // 创建滚动视图，高度为屏幕的80%
    scroll_view = lv_obj_create(ui_S_page01_screen);
    // 清除 边框、轮廓、外框、边界线
lv_obj_remove_style_all(scroll_view); // 清空所有样式（最保险）
lv_obj_set_style_border_width(scroll_view, 0, LV_PART_MAIN);   // 边框宽度=0
lv_obj_set_style_outline_width(scroll_view, 0, LV_PART_MAIN);  // 外轮廓宽度=0
lv_obj_set_style_pad_all(scroll_view, 0, LV_PART_MAIN);        // 内边距=0
lv_obj_set_pos(scroll_view, 0, 0);
    lv_obj_set_size(scroll_view, lv_pct(100), lv_pct(80));
    lv_obj_align(scroll_view, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(scroll_view, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(scroll_view, lv_color_hex(0xCCCCCC), LV_PART_MAIN | LV_STATE_DEFAULT);
		// 开启背景显示（必须加）
lv_obj_set_style_bg_opa(scroll_view, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 示例数据：创建64个uint16数值
    uint16_t test_values[64];
    for (int i = 0; i < 64; i++) {
        // 生成0-5000之间的随机数作为测试数据
        test_values[i] = 0;
    }
//    
//    // 示例颜色标志：低1位和最高位为1，其他为0
//    // 使用uint64_t可以表示64个控件的颜色状态
//    // 这里我们设置第一个和最后一个控件为蓝色，其他为白色
//    uint64_t test_color_flags = ((uint64_t)1 << 0) | ((uint64_t)1 << 63); // 第一个和最后一个控件为蓝色
//    
//    // 调用显示电池电压的函数，只显示前16个电池
//    display_64_values(scroll_view, test_values, test_color_flags, 64);
    display_64_values_init(scroll_view); 
    // 在滚动视图下方添加控件
    lv_obj_t *btn_container = lv_obj_create(ui_S_page01_screen);
    lv_obj_set_size(btn_container, lv_pct(100), lv_pct(20));
    lv_obj_align(btn_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // 创建第一个按钮
    lv_obj_t *btn1 = lv_button_create(btn_container);
    lv_obj_set_size(btn1, 120, 40);
    lv_obj_t *btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, "start");
    lv_obj_center(btn1_label);
    // 添加点击事件回调
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 创建第二个按钮
    lv_obj_t *btn2 = lv_button_create(btn_container);
    lv_obj_set_size(btn2, 120, 40);
    lv_obj_t *btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, "stop");
    lv_obj_center(btn2_label);
    // 添加点击事件回调
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 创建单选框和文本
    lv_obj_t *radio_container = lv_obj_create(btn_container);
    lv_obj_set_flex_flow(radio_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(radio_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // 创建单选框
        // 2. 单选框 (注意：去掉了前面的 lv_obj_t *)
    ui_auto_checkbox = lv_checkbox_create(radio_container);
    lv_checkbox_set_text(ui_auto_checkbox, ""); 
    
    // 创建文本（内容为auto）
    lv_obj_t *auto_label = lv_label_create(radio_container);
    lv_label_set_text(auto_label, "auto");
    lv_obj_set_style_pad_left(auto_label, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 1. 键盘初始化
    ui_global_kb = lv_keyboard_create(ui_S_page01_screen);
    lv_obj_align(ui_global_kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(ui_global_kb, LV_OBJ_FLAG_HIDDEN);
    lv_keyboard_set_mode(ui_global_kb, LV_KEYBOARD_MODE_NUMBER);
    // 设置键盘模式为数字模式

// 找到约 384 行
ui_global_kb = lv_keyboard_create(ui_S_page01_screen);
// --- 修改后的核心配置 ---
/* 1. 关键：脱离 Flex 布局排列。如果不加这行，键盘会被排在按钮容器下面，导致看不见 */
lv_obj_add_flag(ui_global_kb, LV_OBJ_FLAG_FLOATING); 
/* 2. 替代 lv_obj_set_z_index 的写法。在 LVGL 9.4 中将对象移至最前 */
lv_obj_move_foreground(ui_global_kb); 
/* 3. 现在可以相对于屏幕正常对齐了 */
lv_obj_align(ui_global_kb, LV_ALIGN_TOP_MID, 0, 0);
/* 4. 初始隐藏并设置模式 */
lv_obj_add_flag(ui_global_kb, LV_OBJ_FLAG_HIDDEN);
lv_keyboard_set_mode(ui_global_kb, LV_KEYBOARD_MODE_NUMBER);
// -----------------------

    
    // 3. 输入框 1 (注意：去掉了前面的 lv_obj_t *)
    ui_input_stop_vol = lv_textarea_create(btn_container);
    lv_obj_set_size(ui_input_stop_vol, 100, 40);
    lv_textarea_set_placeholder_text(ui_input_stop_vol, "Stop Vol");
    lv_textarea_set_one_line(ui_input_stop_vol, true);
    // 绑定键盘回调
    lv_obj_add_event_cb(ui_input_stop_vol, keyboard_event_cb, LV_EVENT_FOCUSED, ui_global_kb);
		
    // 4. 输入框 2
    ui_input_vol_dif = lv_textarea_create(btn_container);
    lv_obj_set_size(ui_input_vol_dif, 100, 40);
    lv_textarea_set_placeholder_text(ui_input_vol_dif, "Vol Dif");
    lv_textarea_set_one_line(ui_input_vol_dif, true);
    lv_obj_add_event_cb(ui_input_vol_dif, keyboard_event_cb, LV_EVENT_FOCUSED, ui_global_kb);

    ui_input_vol_h_alarm = lv_textarea_create(btn_container);
    lv_obj_set_size(ui_input_vol_h_alarm, 100, 40);
    lv_textarea_set_placeholder_text(ui_input_vol_h_alarm, "Alarm Vol");
    lv_textarea_set_one_line(ui_input_vol_h_alarm, true);
    lv_obj_add_event_cb(ui_input_vol_h_alarm, keyboard_event_cb, LV_EVENT_FOCUSED, ui_global_kb);		
		
		
    // 添加新按钮
    lv_obj_t *btn3 = lv_button_create(btn_container);
    lv_obj_set_size(btn3, 120, 40);
    lv_obj_t *btn3_label = lv_label_create(btn3);
    lv_label_set_text(btn3_label, "Setting");
    lv_obj_center(btn3_label);
    // 添加点击事件回调
    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 为输入框添加事件处理，当获得焦点时显示键盘
    
    
    
    // 为屏幕添加事件处理，当点击屏幕时隐藏键盘
//    lv_obj_add_event_cb(ui_S_page01_screen, click_outside_event_cb, LV_EVENT_CLICKED, ui_global_kb);
    
    // 2. 给键盘本身绑定按键事件
    lv_obj_add_event_cb(ui_global_kb, keyboard_event_cb, LV_EVENT_READY, ui_global_kb);
    lv_obj_add_event_cb(ui_global_kb, keyboard_event_cb, LV_EVENT_CANCEL, ui_global_kb);
		
		    // 3. 给背景屏幕绑定点击事件，用于点击外部隐藏
    // 注意：确保屏幕对象 ui_S_page01_screen 开启了点击属性
    lv_obj_add_flag(scroll_view, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_add_event_cb(scroll_view, ui_screen_click_event_cb, LV_EVENT_CLICKED, ui_global_kb);


// 创建页面切换按钮
lv_obj_t *switch_btn = lv_button_create(btn_container);
lv_obj_set_size(switch_btn, 120, 40);
lv_obj_t *switch_btn_label = lv_label_create(switch_btn);
lv_label_set_text(switch_btn_label, "Go to Page 2");
lv_obj_center(switch_btn_label);

// 添加点击事件回调
lv_obj_add_event_cb(switch_btn, switch_page_event_cb, LV_EVENT_CLICKED, NULL);

    
}

void ui_S_page01_screen_destroy(void) {
    if(ui_S_page01_screen) lv_obj_del(ui_S_page01_screen);
    ui_S_page01_screen = NULL;
}

void ui_S_page01_screen_relocalize(void) {
    // Add relocalization code if needed
}