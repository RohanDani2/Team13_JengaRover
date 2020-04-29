#include <main_motor_task.h>

static struct motorTimer timeVal = {0,false};

void *encoderThread(void *arg0) {
    struct MotorMessage msg;
    int encoder_val;
    char encoderChosen;

    while (1) {
         read_encoder(MOTOR1);
         read_encoder(MOTOR2);
         read_encoder(MOTOR3);

         if(readFromQueue(&msg)) {
             encoder_val = (int)msg.val;
             encoderChosen = msg.encoder;
             chooseEncoder(encoder_val, encoderChosen);
         }
    }
}

int mTimerFunct() {
    Timer_Handle timer1;
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
    params.timerCallback = mTimerCallback;

    timer1 = Timer_open(CONFIG_TIMER_1, &params);
    if (timer1 == NULL || Timer_start(timer1) == Timer_STATUS_ERROR) {
        return 0;
    }
    return 1;
}

// when timer expires, queue event to indicate that a message should be published
void mTimerCallback(Timer_Handle myHandle) {
    timeVal.timePassed += 1;
    m.type = PUBLISH_TYPE;

    if(!sendMsgToPSQueue(&m)){
        dbgFail();
    }
}

void *motorThread(void *arg0) {
    initSPIParams();
    startEncoderTransmission();

    struct recvMsg r;

    m.roverState = SearchingForJenga;
    while (1) {
        driveBackward(speed);

        if(readFromPSQueue(&m)) {
            if(m.type == SUBSCRIBE_TYPE) {
            // receive topic0 message
                if(strncmp(m.topic, SUBSCRIPTION_TOPIC0, SUB_TOPIC0_LEN) == 0) {
                    if(parseJSON(m.data_buf, &r)){
                        updateSubscribeReceied();
                        updateSubscribeShouldReceivedState1(r.seq);
                        UART_PRINT("Received Sensor Value\n\r");
                    }
                    else {
                        UART_PRINT("Failed to parse Sensor message: \n\r %s\n\r", m.data_buf);
                    }
                }
            }
        }
        move_algorithm(r);
    }
}

void move_algorithm(struct recvMsg r) {
    switch(r.ID) {
        case 1:
            if (r.Jenga) {
                if (r.X == 0) {
                }
            }
        break;
    }
}
