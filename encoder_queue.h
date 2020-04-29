/*
 * encoder_queue.h
 *
 *  Created on: Apr 29, 2020
 *      Author: Rohan J. Dani
 */

#ifndef ENCODER_QUEUE_H_
#define ENCODER_QUEUE_H_

/* RTOS header files */
#include <FreeRTOS.h>
#include <queue.h>

// to be able to determine if calling from ISR or not
#include <ti/drivers/dpl/HwiP.h>

// declare queue globally
static QueueHandle_t encoderHandle = NULL;

// queue size, in items
#define ENCODER_QUEUE_SIZE 100

typedef struct MotorMessage{
    uint32_t val;
    char encoder;
} MotorMessage;

int createEncoderQueue();
int sendEncodertoQueue(int32_t encoderVal, char encoder);
int readFromQueue(struct MotorMessage* m);
MotorMessage readEncoderVals();


#endif /* ENCODER_QUEUE_H_ */
