#include <main_mqtt_task.h>

void *mainMqttTask(void *arg0) {

    struct pubSubMsg m;
    int32_t sequence = 0;
    int32_t ID = 0;
    char publish_topic0[MAX_TOPIC_LEN] = { PUBLISH_TOPIC0 };
    char publish_topic1[MAX_TOPIC_LEN] = { PUBLISH_TOPIC1 };
    char publish_data[MAX_MSG_BUF_SIZE];

    while(1){
        if(readFromPSQueue(&m)) {
            if(m.type == PUBLISH_TYPE){
                if(sendMQTTJSON(publish_topic0, publish_data ,m.roverState, sequence)) {
                    sequence++;
                    updatePublishAttempt();
                    UART_PRINT("Published message %s\n\r", publish_data);
                }
                else {
                    UART_PRINT("Failed to send message");
                }
            }
            else if(m.type == PUBLISH_STAT){
                int32_t pub_attempt = getPublishAttempt();
                int32_t sub_received = getSubscribeReceived();
                int32_t sub_not_received = getSubscribeShouldReceived();
                if(sendStatMQTTJSON(publish_topic1,
                                            publish_data,
                                            pub_attempt,
                                            sub_received,
                                            sub_not_received, ID)) {
                    ID += 1;
                    UART_PRINT("Published stats message\n\r");
                }
                else {
                    UART_PRINT("Failed to send stats msg\n\r\n\r");
                }
            }
        }
    }
}
