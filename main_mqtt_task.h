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
#include <uart_term.h>

#define PUBLISH_TOPIC0           "/data"

#define SUBSCRIPTION_TOPIC0      "/sensor"
#define SUB_TOPIC0_LEN 7

extern int sendMQTTJSON(char* publish_topic, char* publish_data ,int32_t sensorValue, int32_t sequence);

// main task to handle publish/subscribe
void *mainMqttTask(void *arg0);


#endif /* MAIN_MQTT_TASK_H_ */
