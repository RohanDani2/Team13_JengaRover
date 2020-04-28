#ifndef MAIN_MOTOR_TASK_H_
#define MAIN_MOTOR_TASK_H_

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
#include <spi_encoder_comm.h>
#include <publish_timer.h>

static unsigned char speed = 30;

void initSPIParams();
void openSPI();
void createThread();
void *motorThread(void *arg0);
void *testThread(void *arg0);



#endif /* MAIN_MOTOR_TASK_H_ */
