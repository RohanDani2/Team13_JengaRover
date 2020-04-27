#ifndef MAINTASK_H_
#define MAINTASK_H_

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

/* std lib files */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/display/Display.h>

#include <uart_term.h>
#include <SPItoMotorEncoder.h>

static unsigned char speed = 10;

void initGPIOParams();
void initSPIParams();
void openSPI();
void openUART();
void createThread();
void *motorThread(void *arg0);
void *testThread(void *arg0);


#endif /* MAINTASK_H_ */
