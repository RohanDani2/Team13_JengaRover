#ifndef PUBSUBQUEUE_H_
#define PUBSUBQUEUE_H_

/* std lib files */
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <queue.h>

// to be able to determine if calling from ISR or not
#include <ti/drivers/dpl/HwiP.h>

#define PUBLISH_TYPE 0
#define PUBLISH_STAT 1
#define SUBSCRIBE_TYPE 2

#define SearchingForJenga 0
#define AvoidingObstacle 1
#define WaitingForArm 2
#define FoundJenga 3
#define TowardDestination 4
#define AtDestination 5
#define ReadyForNextSensorValue 6
#define ErrorState 7

#define MAX_MSG_BUF_SIZE 350

#define MAX_TOPIC_LEN 15

// struct for msg type, either publish or subscribe
struct pubSubMsg
{
    int16_t type;
    int32_t roverState;
    int32_t sensorValue;
    char *json_string;
    char topic[MAX_TOPIC_LEN];
    char data_buf[MAX_MSG_BUF_SIZE];
};

// queue size, in items
#define QUEUE_SIZE 100

// create queue, return value indicates success or failure
int createPSMsgQueue();

// send time message to queue, return value indicates success or failure
int sendMsgToPSQueue(struct pubSubMsg* m);

// read from queue, return value indicates success or failure
int readFromPSQueue(struct pubSubMsg* m);

#endif /* PUBSUBQUEUE_H_ */
