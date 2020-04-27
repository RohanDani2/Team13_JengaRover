#include <pubSubQueue.h>

// declare queue globally
static QueueHandle_t psQueueHandle = NULL;

// setup queue by calling freertos lib func, each item 8 bytes
int createPSMsgQueue(){
    psQueueHandle = xQueueCreate( QUEUE_SIZE, sizeof(struct pubSubMsg) );
    return psQueueHandle != NULL;
}

int sendMsgToPSQueue(struct pubSubMsg* m){
    // if in ISR, non-blocking call to freeRTOS
    if(HwiP_inISR()){
        if(xQueueSendFromISR(psQueueHandle, m, NULL) == pdPASS ){
            return 1;
        }
        return 0;
    }
    // otherwise, make blocking call
    else{
        if(xQueueSend(psQueueHandle, m, portMAX_DELAY) == pdPASS ){
            return 1;
        }
        return 0;
    }
}

// read value from queue
int readFromPSQueue(struct pubSubMsg* m){
    // if in ISR, non-blocking call to freeRTOS
    if(HwiP_inISR()){
        if(xQueueReceiveFromISR(psQueueHandle, m, NULL) == pdPASS ){
            return 1;
        }
        return 0;
    }
    //otherwise, blocking read
    else{
        if(xQueueReceive(psQueueHandle, m, portMAX_DELAY) == pdPASS ){
            return 1;
        }
        return 0;
    }
}
