#include <main_mqtt_task.h>

void *mainMqttTask(void *arg0){

    struct pubSubMsg m;
    struct recvMsg r;
    int32_t sequence = 0;

    // track last known state of rover
    int32_t state = -1;

    char publish_topic[MAX_TOPIC_LEN] = { PUBLISH_TOPIC0 };
    char publish_data[MAX_MSG_BUF_SIZE];

    while(1){
        if(readFromPSQueue(&m)){
            if(m.type == PUBLISH_TYPE){
                if(sendMQTTJSON(publish_topic, publish_data ,m.sensorValue, sequence)){
                    sequence++;
                    UART_PRINT("Published message %s\n\r", publish_data);
                }else{
                    UART_PRINT("Failed to send msg");
                }
            }
            else if(m.type == SUBSCRIBE_TYPE) {
                if(parseJSON(m.json_string, &r)) {
                    UART_PRINT("Received state: %d\n\r", r.state);
                    state = r.state;
                }else{
                    UART_PRINT("Failed to parse message: \n\r %s\n\r", m.json_string);
                }
            }
            else {
                UART_PRINT("Unknown message");
                dbgFail();
            }
        }
    }

    return NULL;
}
