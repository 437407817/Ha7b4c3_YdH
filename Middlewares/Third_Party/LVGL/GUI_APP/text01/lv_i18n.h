/* Minimal stub for lv_i18n to satisfy generated UI includes.
   If you need full i18n support, replace this with the real lv_i18n library.
*/
#ifndef LV_I18N_H
#define LV_I18N_H

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize translation subsystem (no-op stub) */
static inline void lv_i18n_init(void) { (void)0; }

/* Translate a string (stub returns input as-is) */
static inline const char * lv_i18n_translate(const char * src) { return src; }

#ifdef __cplusplus
}
#endif

#endif /* LV_I18N_H */
