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
#include <encoder_queue.h>
#include <parseJSON.h>
#include <pubSubQueue.h>
#include <stats.h>

#define ENCODERTHREADSTACKSIZE (4096)
#define SPAWN_TASK_PRIORITY      9

#define SUBSCRIPTION_TOPIC0      "/sensor"
#define SUB_TOPIC0_LEN 7

static unsigned char speed = 10;

struct motorTimer {
    int timePassed;
    bool goalReached;
    int motorTime;
};

struct roverState {
    int32_t RoverState;
};

void *motorThread(void *arg0);
void *encoderThread(void *arg0);
int createEncoderThread();
int mTimerFunct();
void mTimerCallback(Timer_Handle myHandle);
int startEncoderQueueTask();
void *encoderRead(void *arg0);

void move_algorithm(struct recvMsg r, struct roverState state);
void avoidObstacleXY(struct recvMsg r);
void towardsJenga(struct recvMsg r, int timetoRoll);
void timed_rotateLeft(struct recvMsg r, int timetoRoll);
void timed_rotateRight(struct recvMsg r, int timetoRoll);
void timed_driveBackward(struct recvMsg r, int timetoRoll);
void timed_driveForward(struct recvMsg r, int timetoRoll);



#endif /* MAIN_MOTOR_TASK_H_ */
