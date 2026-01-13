/**
 * @file lv_port_indev_template.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_template.h"
#include "../../lvgl.h"
//#include "./touch/gt9xx.h"
#include "./touch/touch_drv.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_t * indev, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(int32_t * x, int32_t * y);



/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;




/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, touchpad_read);

    /*------------------
     * Mouse
     * -----------------*/


    /*------------------
     * Keypad
     * -----------------*/



    /*------------------
     * Encoder
     * -----------------*/



    /*------------------
     * Button
     * -----------------*/


}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/
static TouchInfo_t g_touchInfo;      // modify
/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
		    //´¥Ãþ³õÊ¼»¯
    TouchDrvInit();
	
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
    static int32_t last_x = 0;
    static int32_t last_y = 0;
//printf("Touch2 \r\n");
    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/
	TouchScan(&g_touchInfo);           // modify
	if (g_touchInfo.state == DOWN)
	{
		return true;
	}
    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(int32_t * x, int32_t * y)
{
    /*Your code comes here*/
    (*x) = g_touchInfo.point.x;     // modify
    (*y) = g_touchInfo.point.y;
//	printf("Touch at (%d, %d), size: %d\n", g_touchInfo.point.x, g_touchInfo.point.y, g_touchInfo.point.size);
//    (*x) = 0;
//    (*y) = 0;
}

/*------------------
 * Mouse
 * -----------------*/




/*------------------
 * Keypad
 * -----------------*/




/*------------------
 * Encoder
 * -----------------*/


/*------------------
 * Button
 * -----------------*/





#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
