/*
 * main_motor_task.h
 *
 *  Created on: Apr 27, 2020
 *      Author: Rohan J. Dani
 */

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

static unsigned char speed = 10;

void initSPIParams();
void openSPI();
void createThread();
void *motorThread(void *arg0);
void *testThread(void *arg0);



#endif /* MAIN_MOTOR_TASK_H_ */
