
#ifndef __NR_SHELL_USER_CMD_H
#define	__NR_SHELL_USER_CMD_H


#include "stdint.h"








int nr_pti(uint8_t argc, char **argv);
int nr_ptl(uint8_t argc, char **argv);
int nr_mem(uint8_t argc, char **argv);
int nr_suspend(uint8_t argc, char **argv);
int nr_resume(uint8_t argc, char **argv);
int nr_delete(uint8_t argc, char **argv);
int nr_setpriority(uint8_t argc, char **argv);
int nr_filterElogLever(uint8_t argc, char **argv);
int nr_fllt(uint8_t argc, char **argv);
int nr_fllk(uint8_t argc, char **argv);




int nr_log(uint8_t argc, char **argv);
int nr_print(uint8_t argc, char **argv);



#endif



