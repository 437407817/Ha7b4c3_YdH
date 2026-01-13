#ifndef __DATA_HEAP_H
#define	__DATA_HEAP_H

#include "stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>




uint32_t get_heap_used(void);
uint32_t get_heap_total(void);
void print_heap_status(void);





#endif







