#include <eventQueue.h>

// declare queue globally
static QueueHandle_t queueHandle = NULL;

// setup queue by calling freertos lib func, each item 8 bytes
int createEventMsgQueue(){
    queueHandle = xQueueCreate( EVENT_QUEUE_SIZE, sizeof(struct eventMessage) );
    return queueHandle != NULL;
}

int sendMsgToEventQueue(struct eventMessage* m){
    // if in ISR, non-blocking call to freeRTOS
    if(HwiP_inISR()){
        if(xQueueSendFromISR(queueHandle, m, NULL) == pdPASS ){
            return 1;
        }
        return 0;
    }
    // otherwise, make blocking call
    else{
        if(xQueueSend(queueHandle, m, portMAX_DELAY) == pdPASS ){
            return 1;
        }
        return 0;
    }
}

// read value from queue
int readFromEventQueue(struct eventMessage* m){
    // if in ISR, non-blocking call to freeRTOS
    if(HwiP_inISR()){
        if(xQueueReceiveFromISR(queueHandle, m, NULL) == pdPASS ){
            return 1;
        }
        return 0;
    }
    //otherwise, blocking read
    else{
        if(xQueueReceive(queueHandle, m, portMAX_DELAY) == pdPASS ){
            return 1;
        }
        return 0;
    }
}
