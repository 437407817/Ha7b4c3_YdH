

#include "./w_string.h"
//#include "./usart/bsp_usart.h"


// 是否是数字字符串
int isNumericString(const char *str) {
    if (str == NULL) {
        return 0;
    }
    while (*str!= '\0') {
        if (!isdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// 将数字字符串转换为整数（简单实现，考虑基本的溢出处理）
int stringToInt(const char *str) {
    int num = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str!= '\0') {

        num = num * 10 + (*str - '0');
        str++;
    }
    return num * sign;
}

/*
*********************************************************************************************************
*	函 数 名: power
*	功能说明: 求n的k次方
*	形    参：n底  k次方
*	返 回 值: 
*********************************************************************************************************
*/




/**************************END OF FILE************************************/











