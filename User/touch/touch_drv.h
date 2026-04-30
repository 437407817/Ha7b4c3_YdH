#ifndef _TOUCH_DRV_H_
#define _TOUCH_DRV_H_

#include <stdint.h>

#define   TOUCH_POINT_MAX  5

#define USE_GT911 0
#define TOUCH_I2C_WR			  	0		    // 写控制bit
#define TOUCH_I2C_RD	         	1		    // 读控制bit


/* GT911 部分寄存器定义 */ 
//#define TOUCH_DEV_ADDR 	 			0xBA      // 写
#define GT911_CTRL_REG   			0x8040      // GT911控制寄存器
#define GT911_CFGS_REG   			0x8050      // GT911配置起始地址寄存器
#define GT911_PID_REG    			0x8140      // GT911产品ID寄存器
//#define  TOUCH_REG_ADDR_SIZE  2
#define GT911_STATUS_REG 			0x814E      // GT911当前检测到的触摸情况
#define GT911_TP1_REG    			0x8150      // 第一个触摸点数据地址
#define GT911_TP2_REG    			0x8158      // 第二个触摸点数据地址
#define GT911_TP3_REG    			0x8160      // 第三个触摸点数据地址
#define GT911_TP4_REG    			0x8168      // 第四个触摸点数据地址
#define GT911_TP5_REG    			0x8170      // 第五个触摸点数据地址



#if USE_GT911
    /* GT911 定义 */
    #define TOUCH_DEV_ADDR          0xBA      
    #define TOUCH_REG_ADDR_SIZE     2           // GT911 使用 16 位寄存器地址
    #define TOUCH_PID_REG           0x8140      
    #define TOUCH_STATUS_REG        0x814E      
    #define TOUCH_TP1_REG           0x8150      
#else
    /* FT5446U 定义 */
    #define TOUCH_DEV_ADDR          0x70      // FT 系列通常是 0x38 << 1 = 0x70
    #define TOUCH_REG_ADDR_SIZE     1           // FT5446 使用 8 位寄存器地址
    #define TOUCH_PID_REG           0xA8      // ID 寄存器
    #define TOUCH_STATUS_REG        0x02      // TD_STATUS
    #define TOUCH_TP1_REG           0x03      // Touch1_XH
#endif




#define DETECT_INTERVAL_TIME        20          // GT911检测间隔时间要求10ms以上




/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;           // 触摸点X坐标
    uint16_t y;           // 触摸点Y坐标
    uint16_t size;        // 触摸点大小
} TouchPoint_t;

#define UP    0
#define DOWN  1
/* 触摸信息结构体 */
typedef struct
{
	uint8_t state;        // 1：按下 or 0：未按下
	TouchPoint_t point;
} TouchInfo_t;	          //触摸信息结构体

void TouchDrvInit(void);
void TouchScan(TouchInfo_t *touchInfo);

void touch_drv_while_test(void);



#endif
