#include "../ui.h"




//电池错误无最高压故障上一页下平均差中文低号均衡充放恒流过报警温断线超时校准自动手启动停止设置大于小等完成伏前后跳转到面语言


static const char * const tags[] = {"Err Battery", "no error","have error", "error", "highest voltage","lowest voltage", 
     "Diff Voltage", "Avg Voltage", "Start", "Stop", "Auto", "Stop Vol", "Vol Dif", "Alarm Vol", "Setting", "Go to Page 2", NULL};
static const char * const languages[] = {"English", "中文", "Español", NULL};
#if USE_LVGL_PC_SIMULATION
static void add_static_translations(void)
{   
    static const char * const translations[] = {
        "Err Battery",    "故障电池",     "Err Battery2",
        "no error",     "无错误",      "no error2",
        "bat %d error",   "电池 %d 故障", "have error2",
        "error",   "故障", "error2",
        "highest voltage", "最高电压", "highest voltage2",
        "lowest voltage", "最低电压", "lowest voltage2",

        "Diff Voltage", "压差", "Diff Voltage2",
        "Avg Voltage", "平均电压", "Avg Voltage2",
        "Start", "启动", "Start2",
        "Stop", "停止", "Stop2",
        "Auto", "自动", "Auto2",
        "Stop Vol", "停止电压", "Stop Vol2",
        "Vol Dif", "电压差", "Vol Dif2",    
        "Alarm Vol", "报警电压", "Alarm Vol2",
        "Setting", "设置", "Setting2",
        "Go to Page 2", "转到页面2", "Go to Page 22",

    };

    lv_translation_add_static(languages, tags, translations);
}
#else

static void add_static_translations(void)
{   
    static const char * const translations[] = {
        "Err Battery",    "\u6545\u969c\u7535\u6c60",     "Err Battery2",
        "no error",     "\u65e0\u9519\u8bef",      "no error2",
        "bat %d error",   "\u7535\u6c60 %d \u6545\u969c", "have error2",
        "error",   "\u6545\u969c", "error2",
        "highest voltage", "\u6700\u9ad8\u7535\u538b", "highest voltage2",
        "lowest voltage", "\u6700\u4f4e\u7535\u538b", "lowest voltage2",

        "Diff Voltage", "\u538b\u5dee", "Diff Voltage2",
        "Avg Voltage", "\u5e73\u5747\u7535\u538b", "Avg Voltage2",
        "Start", "\u542f\u52a8", "Start2",
        "Stop", "\u505c\u6b62", "Stop2",
        "Auto", "\u81ea\u52a8", "Auto2",
        "Stop Vol", "\u505c\u6b62\u7535\u538b", "Stop Vol2",
        "Vol Dif", "\u7535\u538b\u5dee", "Vol Dif2",    
        "Alarm Vol", "\u62a5\u8b66\u7535\u538b", "Alarm Vol2",
        "Setting", "\u8bbe\u7f6e", "Setting2",
        "Go to Page 2", "\u8f6c\u5230\u9875\u9762 2", "Go to Page 22",
        NULL  // 数组结尾建议加NULL，防止越界
    };

    lv_translation_add_static(languages, tags, translations);
}
#endif

/**
 * @brief 给标签绑定多语言事件 + 统一设置字体
 * @param label 要设置的标签对象
 * @param tag 翻译用的tag（如 "Start"）
 */
 void ui_label_set_translate_font(lv_obj_t *label, const char *tag)
{
    // 统一设置字体
    lv_obj_set_style_text_font(label, &lvgl_Font, 0);
    
    // 绑定语言切换事件，自动刷新
    lv_obj_add_event_cb(label, on_language_change, LV_EVENT_TRANSLATION_LANGUAGE_CHANGED, (void *)tag);
}

static void on_textarea_lang_change(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target_obj(e);
    const char *tag = lv_event_get_user_data(e);
    
    // 重点：Textarea 是刷新占位符，不是 set_text
    lv_textarea_set_placeholder_text(ta, lv_tr(tag));
}
/**
 * @brief 给 Textarea 设置字体 + 多语言占位符（placeholder）
 */
 void ui_textarea_set_translate_font(lv_obj_t *textarea, const char *tag)
{
    // 设置字体
    lv_obj_set_style_text_font(textarea, &lvgl_Font, 0);
    
    // 绑定语言切换事件，刷新占位符
    lv_obj_add_event_cb(textarea, on_textarea_lang_change, LV_EVENT_TRANSLATION_LANGUAGE_CHANGED, (void *)tag);
}

/**
 * @brief Textarea 语言切换专用回调（刷新占位符）
 */


 void on_language_change(lv_event_t * e)
{
    lv_obj_t * label      = lv_event_get_target_obj(e);
    
    /* 检查对象是否是 label 类型，防止事件冒泡导致的错误 */
    if(!lv_obj_has_class(label, &lv_label_class)) {
        return;
    }
    
    const char * tag      = (const char *) lv_event_get_user_data(e);
    /* You can get the new language with `lv_event_get_param`*/
    const char * language = (const char *) lv_event_get_param(e);
    LV_UNUSED(language);

    lv_label_set_text(label, lv_tr(tag));
}

static void language_change_cb(lv_event_t * e)
{
    static char selected_lang[20];

    lv_obj_t * dropdown = lv_event_get_target_obj(e);

    lv_dropdown_get_selected_str(dropdown, selected_lang, sizeof(selected_lang));

    lv_translation_set_language(selected_lang);
}



/**
 * @title Static and dynamic translation packs
 * @brief Register two translation sources and print labels via `lv_tr`.
 *
 * A static pack provides the tags `tiger`, `lion`, `rabbit`, and `elephant` in
 * English, German, and Spanish through `lv_translation_add_static`. A dynamic
 * pack built with `lv_translation_add_dynamic` adds the tags `table` and
 * `chair` for English and German using `lv_translation_add_tag` and
 * `lv_translation_set_tag_translation`. `lv_translation_set_language("de")`
 * selects German and two labels render the `tiger` and `chair` translations
 * through `lv_tr`.
 */
void ui_translate_1(lv_obj_t *page)
{
    lv_obj_set_flex_flow(page, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(page, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    add_static_translations();
    const size_t tag_count = sizeof(tags) / sizeof(tags[0]) - 1;
    const size_t lang_count = sizeof(languages) / sizeof(languages[0]) - 1;

    /* Create a dropdown to be able to select the language */
    lv_obj_t * language_dropdown = lv_dropdown_create(page);
    lv_dropdown_clear_options(language_dropdown);

    for(size_t i = 0; i < lang_count; ++i) {
        lv_dropdown_add_option(language_dropdown, languages[i], i);
    }

    lv_obj_add_event_cb(language_dropdown, language_change_cb, LV_EVENT_VALUE_CHANGED, NULL);



    lv_translation_set_language("English");
}