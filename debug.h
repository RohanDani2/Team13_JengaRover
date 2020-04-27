#ifndef DEBUG_H_
#define DEBUG_H_

#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* driver header files */
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define DLOC_TASK_ENTER 1
#define DLOC_BEFORE_WHILE 8
#define DLOC_BEFORE_QUEUE 4
#define DLOC_AFTER_QUEUE 5
#define DLOC_ENTER_ISR 16
#define DLOC_LEAVE_ISR 17
#define DLOC_BEFORE_ISR_QUEUE 2
#define DLOC_AFTER_ISR_QUEUE 3
#define DEBUG_ERROR 127

#define GPIO_ON 1
#define GPIO_OFF 0

void initGPIO();

// initialize UART
int initUART();

// write to UART
void dbgUARTVal(unsigned char outVal);

void dbgUARTString(char *word);

// write to GPIO pins
void dbgOutputLoc(unsigned int outLoc);

// failed stop everything function
void dbgFail();

#endif /* DEBUG_H_ */
