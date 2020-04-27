#ifndef __UART_IF_H__
#define __UART_IF_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// TI-Driver includes
#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"

//Defines
#define UART_PRINT Report
#define DBG_PRINT  Report
#define ERR_PRINT(x) Report("Error [%d] at line [%d] in function [%s]  \n\r",\
                                                                x, __LINE__, \
                                                                 __FUNCTION__)
#define IS_SPACE(x)       (x == 32 ? 1 : 0)

// buffer size
#define BUF_SIZE 256

/* API */

int InitTerm(void);

int Report(const char *pcFormat,
           ...);

int TrimSpace(char * pcInput);

int GetCmd(char *pcBuffer,
           unsigned int uiBufLen);

void Message(const char *str);

void ClearTerm();

char getch(void);

void putch(char ch);

void initBaudRate();
void driveForward(char speed);
void driveBackward(char speed);
void rotateLeft(char speed);
void rotateRight(char speed);

#endif // __UART_IF_H__
