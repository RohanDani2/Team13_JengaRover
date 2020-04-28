#ifndef MSG_QUEUE_H_
#define MSG_QUEUE_H_

/* std lib files */
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <queue.h>

// to be able to determine if calling from ISR or not
#include <ti/drivers/dpl/HwiP.h>

// struct for msg type
struct eventMessage
{
    int32_t event;
    void *msgPtr;
    int32_t topLen;
    int32_t sensorValue;
    char *json_string;
};

// queue size, in items
#define EVENT_QUEUE_SIZE 100

// create queue, return value indicates success or failure
int createEventMsgQueue();

// send time message to queue, return value indicates success or failure
int sendMsgToEventQueue(struct eventMessage* m);

// read from queue, return value indicates success or failure
int readFromEventQueue(struct eventMessage* m);

#endif /* MSG_QUEUE_H_ */
