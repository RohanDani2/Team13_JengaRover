#include <encoder_queue.h>

// setup queue by calling freertos lib func, each item 8 bytes
int createEncoderQueue() {
    encoderHandle = xQueueCreate( ENCODER_QUEUE_SIZE, sizeof(struct MotorMessage));
    return encoderHandle != NULL;
}

int sendEncodertoQueue(int32_t encoderVal, char encoder) {
    struct MotorMessage m = {(uint32_t)encoderVal, encoder};
    // if in ISR, non-blocking call to freeRTOS
    if(HwiP_inISR()){
        if(xQueueSendFromISR(encoderHandle, &m, NULL) == pdPASS ){
            return 1;
        }
        return 0;
    }
    // otherwise, make blocking call
    else{
        if(xQueueSend(encoderHandle, &m, portMAX_DELAY) == pdPASS ){
            return 1;
        }
        return 0;
    }
}

// read value from queue
int readFromQueue(struct MotorMessage* m) {
    // if in ISR, non-blocking call to freeRTOS
    if(HwiP_inISR()){
        if(xQueueReceiveFromISR(encoderHandle, m, NULL) == pdPASS ){
            return 1;
        }
        return 0;
    }
    //otherwise, blocking read
    else{
        if(xQueueReceive(encoderHandle, m, portMAX_DELAY) == pdPASS ){
            return 1;
        }
        return 0;
    }
}





