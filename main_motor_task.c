#include <main_motor_task.h>

static struct motorTimer timeVal = {0,false,0};

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

/*    if(!createEncoderThread()) {
        UART_PRINT("Encoder Thread Failed\n\r");
    }

    if(!startEncoderQueueTask()) {
        UART_PRINT("Encoder Read Thread Failed\n\r");
    }*/

    struct recvMsg r;

    m.roverState = SearchingForJenga;
    while (1) {
        move_algorithm(r);
        if(readFromPSQueue(&m)) {
            if(m.type == SUBSCRIBE_TYPE) {
                if (strncmp(m.topic, SUBSCRIPTION_TOPIC0, SUB_TOPIC0_LEN) == 0) {
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
    }
}

void move_algorithm(struct recvMsg r) {
    int timetoRollX = 0;
    int timetoRollY = 0;

    switch(r.ID) {
        case 1:
            if (r.Jenga == 1 && r.armState == 0) {
                if (r.X == 0 && r.Y != 0) {
                    towardsJenga(r, 3);
                    m.roverState = TowardDestination;
                    UART_PRINT("changed to destination in rover state");
                    avoidObstacleXY(r);
                }
                else if (r.X < 0 && r.Y != 0) {
                    UART_PRINT("shouldn't be here 1");
                    timetoRollX = r.X%speed;
                    timetoRollY = r.Y%speed;
                    towardsJenga(r, timetoRollY);
                    timed_rotateLeft(r, timetoRollX);
                    towardsJenga(r, timetoRollY);
                }
                else if (r.X > 0 && r.Y != 0) {
                    UART_PRINT("shouldn't be here 2");
                    timetoRollX = r.X%speed;
                    timetoRollY = r.Y%speed;
                    towardsJenga(r, timetoRollY);
                    timed_rotateRight(r, timetoRollX);
                    towardsJenga(r, timetoRollY);
                }
            }
            else if(r.armState == 1) {
                UART_PRINT("shouldn't be here 3");
                m.roverState = WaitingForArm;
            }
            else if(r.Jenga == 0) {
                UART_PRINT("shouldn't be here 4");
                m.roverState = AvoidingObstacle;
                avoidObstacleXY(r);
            }

            if (m.roverState == TowardDestination) {
                UART_PRINT("going towards destination");
                timed_driveForward(r, 10);
                sleep(2);
                m.roverState = AtDestination;
                timed_rotateRight(r, 3);
                sleep(2);
                UART_PRINT("ready for next sensor value");
                m.roverState = ReadyForNextSensorValue;
                sleep(2);
            }
        break;
    }
}

void avoidObstacleXY(struct recvMsg r) {
    if (r.Y > 0 && r.X == 0) {
        UART_PRINT("avoiding jenga block so I can go towards destination");
        motorStop();
        sleep(2);
        timed_rotateRight(r, 2);
        sleep(2);
        timed_driveForward(r, 2);
        sleep(2);
        timed_rotateLeft(r, 2);
    }
}

void towardsJenga(struct recvMsg r, int timetoRoll) {
    int timePassed;
    for (timePassed = timeVal.timePassed; timePassed <= timetoRoll; timePassed++) {
        m.roverState = SearchingForJenga;
        UART_PRINT("Moving towards Jenga");
        driveForward(speed);
    }
    sleep(2);
    UART_PRINT("Found the Jenga");
    m.roverState = FoundJenga;
    timeVal.motorTime = timetoRoll;
    motorStop();
    sleep(2);
}

void timed_driveForward(struct recvMsg r, int timetoRoll) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        driveForward(speed);
    }

    motorStop();
    sleep(2);
}

void timed_driveBackward(struct recvMsg r, int timetoRoll) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        driveBackward(speed);
    }

    motorStop();
    sleep(2);
}

void timed_rotateLeft(struct recvMsg r, int timetoRoll) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        rotateLeft(speed);
    }

    motorStop();
    sleep(2);
}

void timed_rotateRight(struct recvMsg r, int timetoRoll) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        rotateRight(speed);
    }

    motorStop();
    sleep(2);
}

void *encoderThread(void *arg0) {
    while (1) {
        read_encoder(MOTOR1);
        read_encoder(MOTOR2);
        read_encoder(MOTOR3);
    }
}

void *encoderRead(void *arg0) {
    struct MotorMessage msg;
    int encoder_val;
    char encoderChosen;

    while (1) {
        if(readFromQueue(&msg)) {
             encoder_val = (int)msg.val;
             encoderChosen = msg.encoder;
             chooseEncoder(encoder_val, encoderChosen);
        }
    }
}

int startEncoderQueueTask() {
    return xTaskCreate( encoderRead, "encoder queue",configPOSIX_STACK_SIZE*4, NULL,SPAWN_TASK_PRIORITY, NULL) == pdPASS;
}

int createEncoderThread() {
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        return 0;
    }
    retc |= pthread_attr_setstacksize(&attrs, ENCODERTHREADSTACKSIZE);
    if (retc != 0) {
        return 0;
    }
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);
    retc = pthread_create(&thread0, &attrs, encoderThread, NULL);
    if (retc != 0) {
        return 0;
    }
    return 1;
}
