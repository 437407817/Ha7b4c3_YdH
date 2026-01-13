/**
 * @file shell_port.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

//#include "serial.h"
#include "shell.h"

extern Shell shell;

void LetterShell_OS_Init(void);
void letterShell_no_os_WhileInit(void);
void letter_Shell_NoOsWhileTask(void);
void letter_Shell_NoOsHandleTask(void);

#endif
