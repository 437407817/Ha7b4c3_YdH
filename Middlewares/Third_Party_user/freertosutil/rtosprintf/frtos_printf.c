

#include "./rtosprintf/frtos_printf.h"


#include "./sys/sysio.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <stdio.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"          // letter-shell log 头文件
#include "shell.h"        // letter-shell 核心头文件




// 全局标志：0=FreeRTOS未启动，1=FreeRTOS已启动
static uint8_t rtosShell_running = 0;
static char log_buf[256] = {0};
/**
 * @brief 标记FreeRTOS已启动（需在vTaskStartScheduler后调用）
 */
void SYSTEM_set_rtosShell_running(void)
{
    rtosShell_running = 1;
}

/**
 * @brief 跨RTOS启动前后的通用打印函数
 * @param fmt 格式化字符串（同printf）
 * @param ... 可变参数
 * @note FreeRTOS启动前→printf输出；启动后→logDebug输出
 */
void SYSTEM_i_print(const char *fmt, ...)
{
    // 1. 参数校验：格式化字符串不能为空
    if (fmt == NULL) {
        return;
    }

    // 2. 定义可变参数列表
    va_list args;
    va_start(args, fmt);

    // 3. 按RTOS状态分支处理
    if (rtosShell_running == 0) {
        // FreeRTOS未启动：用标准printf（vprintf处理可变参数）
        vprintf(fmt, args);
    } else {
        // FreeRTOS已启动：用letter-shell的logDebug（需先初始化log）
        // 方式1：直接调用logDebug（推荐，适配shell尾行模式）
//        logDebug(fmt, args);

        // 方式2：若logDebug不支持va_list，改用logPrintf封装（备选）
        
//        if (shell.lock) shell.lock(&shell);
        vsnprintf(log_buf, sizeof(log_buf)-1, fmt, args);
        logInfo("%s", log_buf);
               // 解锁
//        if (shell.unlock) shell.unlock(&shell);
    }

    // 4. 释放可变参数列表
    va_end(args);
}



/**
 * @brief 内部核心打印函数（带调试信息）
 * @param file 文件名
 * @param line 行号
 * @param func 函数名
 * @param fmt 格式化字符串
 * @param args 可变参数列表
 */
 void SYSTEM_d_print(const char *file, int line, const char *func, const char *fmt, ...)
{
    if (fmt == NULL || file == NULL || func == NULL) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    // 提取文件名（去掉绝对路径，只保留xxx.c）
    const char *filename = strrchr(file, '/');
    if (filename == NULL) {
        filename = strrchr(file, '\\'); // 兼容Windows路径
    }
    filename = (filename == NULL) ? file : (filename + 1);

    // 分支处理：FreeRTOS启动前/后
    if (rtosShell_running == 0) {
        // 启动前：printf输出（带调试信息）
        printf("[%s:%d %s] ", filename, line, func);
        vprintf(fmt, args);
        printf("\r\n"); // 自动换行
    } else {
        // 启动后：全局缓冲区格式化（无栈溢出）+ logDebug
//        if (shell.lock) shell.lock(&shell);
        
        // 拼接调试信息+用户内容
        vsnprintf(log_buf, sizeof(log_buf)-1, fmt, args);
        logDebug("[%s:%d %s] %s", filename, line, func, log_buf);
        
//        if (shell.unlock) shell.unlock(&shell);
    }
		 // 4. 释放可变参数列表
    va_end(args);
}
















