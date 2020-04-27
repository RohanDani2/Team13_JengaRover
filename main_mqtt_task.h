#ifndef MAIN_MQTT_TASK_H_
#define MAIN_MQTT_TASK_H_

#include <stdlib.h>

/* RTOS header files */
#include "FreeRTOS.h"

// custom includes
#include <debug.h>
#include <pubSubQueue.h>
#include <publish_timer.h>
#include <parseJSON.h>

#define PUBLISH_TOPIC0           "/data"
#define MAX_TOPIC_LEN 256

// max buffer size (TODO maybe change so larger to accomodate max size)
#define MAX_MSG_BUF_SIZE 1000

extern int sendMQTTJSON(char* publish_topic, char* publish_data ,int32_t sensorValue, int32_t sequence);

// main task to handle publish/subscribe
void *mainMqttTask(void *arg0);


#endif /* MAIN_MQTT_TASK_H_ */
