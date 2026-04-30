#ifndef UI_TRANSLATE_H
#define UI_TRANSLATE_H


#include "../ui.h"




extern void ui_label_set_translate_font(lv_obj_t *label, const char *tag);
extern void ui_textarea_set_translate_font(lv_obj_t *textarea, const char *tag);
 extern void ui_translate_1(lv_obj_t *page);
 extern void on_language_change(lv_event_t * e);

#endif