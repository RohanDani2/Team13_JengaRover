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
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include <uart_term.h>
#include <debug.h>
#include <spi_encoder_comm.h>
#include <parseJSON.h>
#include <pubSubQueue.h>
#include <stats.h>

#define SUBSCRIPTION_TOPIC0      "/sensor"
#define SUB_TOPIC0_LEN 7

struct pubSubMsg m;
static unsigned char speed = 20;

struct motorTimer {
    int timePassed;
    bool goalReached;
};

void *motorThread(void *arg0);
int mTimerFunct();
void mTimerCallback(Timer_Handle myHandle);
void move_algorithm(struct recvMsg r);



#endif /* MAIN_MOTOR_TASK_H_ */
