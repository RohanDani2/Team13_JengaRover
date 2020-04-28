#include <main_motor_task.h>

/*void *testThread(void *arg0) {
    struct MotorMessage msg;
    int nextMove;
    int encoder_val;
    char encoderChosen;

    while(1)
    {
       if (readFromMotorQueue(&msg)) {
            if (msg.type == 'c')
            {
                nextMove = (int)msg.val;
                movementThroughQueue(nextMove, speed);
            }

            if (msg.type == 'e')
            {
                encoder_val = (int)msg.val;
                encoderChosen = msg.encoder;
                chooseEncoder(encoder_val, encoderChosen);
            }
       }
    }
}*/

int mTimerFunct() {
    Timer_Params params;

    /* Call driver init functions */
    Timer_init();

    /* Setting up the timer in continuous callback mode that calls the callback
     * function every 1,000,000 microseconds, or 1 second.
     */
    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = pTimerCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL || Timer_start(timer0) == Timer_STATUS_ERROR){
        return 0;
    }
    return 1;
}

void *motorThread(void *arg0) {

    //startEncoderTransmission();
    struct pubSubMsg m;
    struct recvMsg r;
    while (1) {
        if(readFromPSQueue(&m)) {
            if(m.type == SUBSCRIBE_TYPE) {
            // receive topic0 message
                if(strncmp(m.topic, SUBSCRIPTION_TOPIC0, SUB_TOPIC0_LEN) == 0) {
                    if(parseJSON(m.data_buf, &r)){
                        UART_PRINT("Received Sensor Value\n\r");
                        move_algorithm(r);
                    }
                    else {
                        UART_PRINT("Failed to parse Sensor message: \n\r %s\n\r", m.data_buf);
                    }
                }
            }
        }

        //read_encoder(MOTOR1);
        //read_encoder(MOTOR2);
        //read_encoder(MOTOR3);
    }
}

void move_algorithm(struct recvMsg r) {

    switch(r.ID) {
        case 1:
            if (r.Jenga) {
                if (r.X == 0) {
/*                    int timeVal = Timer_getCount(timer0);
                    while (timeVal != 3) {
                        driveForward(speed);
                    }*/
                }
            }
        break;
    }
    motorStop();
}
