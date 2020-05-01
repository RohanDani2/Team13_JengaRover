#include <main_motor_task.h>

static struct motorTimer timeVal = {0,false,0};
static struct pubSubMsg m;
int count = 0;
int32_t  roverState = 0;

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

    m.roverState = roverState;
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
    }*/

    /*if(!startEncoderQueueTask()) {
        UART_PRINT("Encoder Read Thread Failed\n\r");
    }*/

    struct recvMsg r;
    m.roverState = 0;

    while (1) {
        if(readFromPSQueue(&m)) {
            if (count == 0 && roverState < 7 && roverState >= 0) {
                roverState = move_algorithm(r, roverState, count);
            }

            if(m.type == SUBSCRIBE_TYPE) {
                if (strncmp(m.topic, SUBSCRIPTION_TOPIC0, SUB_TOPIC0_LEN) == 0) {
                    if(parseJSON(m.data_buf, &r)){
                        updateSubscribeReceied();
                        updateSubscribeShouldReceivedState1(r.seq);
                        UART_PRINT("\n\rReceived Sensor Value\n\r");
                    }
                    else {
                        UART_PRINT("\n\rFailed to parse Sensor message: \n\r %s\n\r", m.data_buf);
                    }
                }
            }
        }
    }
}

int32_t  move_algorithm(struct recvMsg r,int32_t roverState, int count) {
    unsigned char speed = 15;
    if (roverState != Finished) {
        if (r.X >= 0 && r.Y >= 0) {
            switch(roverState) {
                case SearchingForJenga:
                    if (r.armState == 1) {
                        roverState = WaitingForArm;
                    }
                    else if(r.Jenga == 0) {
                        roverState = AvoidingObstacle;
                    }
                    else {
                        if (r.X == 0 && r.Y != 0) {
                            towardsJenga(r, 80, speed);
                            roverState = FoundJenga;
                        }
                        else if (r.X < 0 && r.Y != 0) {
                            towardsJenga(r, 2, speed);
                            timed_rotateLeft(r, 2, speed);
                            towardsJenga(r, 2, speed);
                            roverState = FoundJenga;
                        }
                        else if (r.X > 0 && r.Y != 0) {
                            towardsJenga(r, 2, speed);
                            timed_rotateRight(r, 2, speed);
                            towardsJenga(r, 2, speed);
                            roverState = FoundJenga;
                        }
                    }
                    UART_PRINT("\n\n\rCase 1\n\n\r");
                    break;
                case AvoidingObstacle:
                    avoidObstacleXY(r, 10, 10, 10, speed);
                    roverState = SearchingForJenga;
                    break;
                case FoundJenga:
                    UART_PRINT("\n\n\rCase 2\n\n\r");
                    avoidObstacleXY(r, 40, 60, 30, speed);
                    roverState = TowardDestination;
                    break;
                case TowardDestination:
                    UART_PRINT("\n\n\rCase 3\n\n\r");
                    timed_driveForward(r, 100, speed);
                    sleep(2);
                    roverState = AtDestination;
                    break;
                case AtDestination:
                    UART_PRINT("\n\n\rCase 4\n\n\r");
                    timed_rotateRight(r, 60, speed);
                    sleep(2);
                    roverState = ReadyForNextSensorValue;
                    break;
                case WaitingForArm:
                    UART_PRINT("\n\n\rWaiting for Arm\n\n\r");
                    if (r.armState == 0) {
                        roverState = SearchingForJenga;
                    }
                    break;
                case ReadyForNextSensorValue:
                    UART_PRINT("\n\n\rCase 5\n\n\r");
                    UART_PRINT("\n\n\rSent to Queue\n\n\r");
                    count += 1;
                    if (r.ID == 2) {
                        UART_PRINT("\n\n\rNext Data value Found\n\n\r");
                        roverState = SearchingForJenga;
                    }
                    roverState = Finished;
                    break;
                default:
                    roverState = ErrorState;
            }
        }
    }
    return roverState;
}

void avoidObstacleXY(struct recvMsg r, int RR_timetoRoll, int F_timetoRoll, int RL_timetoRoll, unsigned char speed) {
    if (r.Y > 0 && r.X == 0) {
        UART_PRINT("\n\n\r avoiding jenga block\n\n\r");
        motorStop();
        timed_rotateRight(r, RR_timetoRoll,speed);
        sleep(2);
        timed_driveForward(r, F_timetoRoll,speed);
        sleep(2);
        timed_rotateLeft(r, RL_timetoRoll,speed);
        sleep(2);
        motorStop();
    }
}

void towardsJenga(struct recvMsg r, int timetoRoll, unsigned char speed) {
    int timePassed;
    for (timePassed = timeVal.timePassed; timePassed <= timetoRoll; timePassed++) {
        UART_PRINT("\n\n\rMoving towards Jenga\n\n\r");
        driveForward(speed, speed+1);
    }
    UART_PRINT("\n\n\rStopped at Jenga block\n\n\r");
    timeVal.motorTime = timetoRoll;
    sleep(2);
    motorStop();
}

void timed_driveForward(struct recvMsg r, int timetoRoll, unsigned char speed) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        driveForward(speed,speed+1);
    }
    sleep(2);
    motorStop();
}

void timed_driveBackward(struct recvMsg r, int timetoRoll, unsigned char speed) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        driveBackward(speed,speed-3);
    }
    sleep(2);
    motorStop();
}

void timed_rotateLeft(struct recvMsg r, int timetoRoll, unsigned char speed) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        rotateLeft(speed,speed,speed);
    }
    sleep(2);
    motorStop();
}

void timed_rotateRight(struct recvMsg r, int timetoRoll, unsigned char speed) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        rotateRight(speed,speed,speed);
    }
    sleep(2);
    motorStop();
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
