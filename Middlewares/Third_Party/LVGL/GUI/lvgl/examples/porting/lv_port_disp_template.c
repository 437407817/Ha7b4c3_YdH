/**
 * @file lv_port_disp_template.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include <stdbool.h>
#include "../../lvgl.h"
#include "./lcd/bsp_lcd.h"
/*********************
 *      DEFINES
 *********************/
 #define MY_DISP_HOR_RES    LCD_MAX_PIXEL_WIDTH
 #define MY_DISP_VER_RES    LCD_MAX_PIXEL_HEIGHT
 
 
 

 
 
 
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_ARGB8888)) /*will be 2 for RGB565 */




/* 外部内存配置 - 按实际FMC驱动修改 */
#define EXT_SRAM_START_ADDR    0xD0000000  // 外部内存起始地址
#define EXT_SRAM_SIZE          (2 * 1024 * 1024) // 外部内存总大小2MB

/* LCD核心配置：800×480+ARGB8888 */
#define LCD_WIDTH              800
#define LCD_HEIGHT             480
#define LCD_PIXEL_FORMAT       LTDC_PIXEL_FORMAT_ARGB8888 // 固定ARGB8888
//#define PIXEL_BYTES            BYTE_PER_PIXEL                           // 4字节/像素
#define ActiveLayer            0                           // LTDC使用层0（可改1）

/* LVGL缓冲区配置 - 半缓冲+局部渲染（2MB外部内存专用，512KB占用） */
#define LV_DISP_BUF_LINE_NUM   64  // 半缓冲占64行，可根据内存调整（32/64/128）
#define LV_DISP_BUF_PIXELS     (LCD_WIDTH * LV_DISP_BUF_LINE_NUM) // 800×64=51200像素
#define LV_DISP_BUF_BYTES      (LV_DISP_BUF_PIXELS * BYTE_PER_PIXEL) // 51200×4=204800字节=200KB（极省内存）

/* 外部内存地址分配（严格4字节对齐，无重叠） */
#define LV_MEM_POOL_ADDR       (EXT_SRAM_START_ADDR)          // LVGL内存池：前256KB（0xD0000000-0xD003FFFF）
#define LV_DISP_BUF1_ADDR      (EXT_SRAM_START_ADDR + 0x40000) // 缓冲区1：偏移256KB（0xD0040000，4字节对齐）
#define LV_DISP_BUF2_ADDR      (LV_DISP_BUF1_ADDR + LV_DISP_BUF_BYTES) // 缓冲区2：偏移256+200=456KB（无重叠）




/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

/**********************
 *  STATIC VARIABLES
 **********************/
 #define LCD_FRAME_BUFFERPR ((uint32_t *)EXT_SRAM_START_ADDR)  // 屏幕显存基地址（SDRAM/FMC映射地址）
 
LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf_1_1[MY_DISP_HOR_RES * BYTE_PER_PIXEL*10] __attribute__((at(EXT_SRAM_START_ADDR + MY_DISP_HOR_RES*MY_DISP_VER_RES )));            /*A buffer for 10 rows*/		


//LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf_2_1[MY_DISP_HOR_RES * BYTE_PER_PIXEL* 10] __attribute__((at(0XD0000000 + 600*800*8 )));            /*A buffer for 10 rows*/		

//LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf_2_1[MY_DISP_HOR_RES * BYTE_PER_PIXEL] __attribute__((at(0XD0000000 + 600*800*8 )));  
//static lv_color_t buf_1[LCD_MAX_PIXEL_WIDTH * LCD_MAX_PIXEL_HEIGHT] __attribute__((at(0XD0000000 + 600*800*8 )));



// 指向外部内存的ARGB8888缓冲区指针
static lv_color32_t *disp_buf1 = (lv_color32_t *)LV_DISP_BUF1_ADDR; // 用lv_color32_t适配ARGB8888
static lv_color32_t *disp_buf2 = (lv_color32_t *)LV_DISP_BUF2_ADDR;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    

    /* Example 1
     * One buffer for partial rendering*/
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_1_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];            /*A buffer for 10 rows*/
	
	
	
	
    lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

	
//	lv_display_set_buffers(disp, disp_buf1, disp_buf2, MY_DISP_HOR_RES * BYTE_PER_PIXEL*10, LV_DISPLAY_RENDER_MODE_PARTIAL);
	

    /* Example 2
	
	
	
	
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_2_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];

//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
//    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];

//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];
//    lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);
		lv_display_set_flush_cb(disp, disp_flush);
	
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
	LCD_init_All();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}



/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map)
{
    if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

//ltdc_color_fill3(area->x1, area->y1, area->x2, area->y2, (uint32_t)px_map);//3重dui
ltdc_color_fill(area->x1, area->y1, area->x2, area->y2, (uint16_t *)px_map);			
			


	}
    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
