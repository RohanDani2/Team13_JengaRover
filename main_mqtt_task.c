#include <main_mqtt_task.h>

void *mainMqttTask(void *arg0) {

    struct pubSubMsg m;
    int32_t sequence = 0;
    char publish_topic[MAX_TOPIC_LEN] = { PUBLISH_TOPIC0 };
    char publish_data[MAX_MSG_BUF_SIZE];

    while(1){
        if(readFromPSQueue(&m)) {
            if(m.type == PUBLISH_TYPE){
                if(sendMQTTJSON(publish_topic, publish_data ,m.roverState, sequence)){
                    sequence++;
                    UART_PRINT("Published message %s\n\r", publish_data);
                }
                else{
                    UART_PRINT("Failed to send message");
                }
            }
        }
    }
}
