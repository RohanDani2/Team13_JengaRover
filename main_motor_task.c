#include <main_motor_task.h>

static struct motorTimer timeVal = {0,false,0};
static struct pubSubMsg m;

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
    }*/

    /*if(!startEncoderQueueTask()) {
        UART_PRINT("Encoder Read Thread Failed\n\r");
    }*/

    struct recvMsg r;
    m.roverState = 0;

    struct roverState state;
    state.RoverState = 0;

    while (1) {
        if(readFromPSQueue(&m)) {
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

void move_algorithm(struct recvMsg r, struct roverState state) {
    if (m.roverState == SearchingForJenga) {
        if (r.armState == 1) {
            state.RoverState = WaitingForArm;
            m.roverState = WaitingForArm;
        }
        else if(r.Jenga == 0) {
            state.RoverState = WaitingForArm;
            m.roverState = AvoidingObstacle;
        }
        else {
            if (r.X == 0 && r.Y != 0) {
                towardsJenga(r, 5);
                state.RoverState = FoundJenga;
                m.roverState = FoundJenga;
                UART_PRINT("\n\n\rupdate\n\n\r");
            }
            else if (r.X < 0 && r.Y != 0) {
                towardsJenga(r, 2);
                timed_rotateLeft(r, 2);
                towardsJenga(r, 2);
                state.RoverState = FoundJenga;
                m.roverState = FoundJenga;
            }
            else if (r.X > 0 && r.Y != 0) {
                towardsJenga(r, 2);
                timed_rotateRight(r, 2);
                towardsJenga(r, 2);
                state.RoverState = FoundJenga;
                m.roverState = FoundJenga;
            }
        }
    }
    else if(m.roverState == AvoidingObstacle) {
        avoidObstacleXY(r);
        state.RoverState = SearchingForJenga;
        m.roverState = SearchingForJenga;
    }
    else if(m.roverState == FoundJenga) {
        //avoidObstacleXY(r);
        state.RoverState = TowardDestination;
        m.roverState = TowardDestination;
    }
    else if(m.roverState == TowardDestination) {
        UART_PRINT("\n\n\rTowards Destination\n\n\r");
        timed_driveForward(r, 10);
        state.RoverState = AtDestination;
        m.roverState = AtDestination;
    }
    else if(m.roverState == AtDestination) {
        UART_PRINT("\n\n\rAt Destination\n\n\r");
        timed_rotateRight(r, 5);
        state.RoverState = ReadyForNextSensorValue;
        m.roverState = ReadyForNextSensorValue;
    }
    else if(m.roverState == WaitingForArm) {
        UART_PRINT("\n\n\rWaiting for Arm\n\n\r");
        if (r.armState == 0) {
            state.RoverState = SearchingForJenga;
            m.roverState = SearchingForJenga;
        }
    }
    else if(m.roverState == ReadyForNextSensorValue) {
        UART_PRINT("\n\n\rReady for next Sensor Value\n\n\r");
        if (r.ID += 1) {
            UART_PRINT("\n\n\rNext Data value Found\n\n\r");
            state.RoverState = SearchingForJenga;
            m.roverState = SearchingForJenga;
        }
    }
    else {
        m.roverState = ErrorState;
    }
    /*switch(state.RoverState) {
        case SearchingForJenga:
            if (r.armState == 1) {
                state.RoverState = WaitingForArm;
                m.roverState = WaitingForArm;
            }
            else if(r.Jenga == 0) {
                state.RoverState = WaitingForArm;
                m.roverState = AvoidingObstacle;
            }
            else {
                if (r.X == 0 && r.Y != 0) {
                    towardsJenga(r, 5);
                    state.RoverState = FoundJenga;
                    m.roverState = FoundJenga;
                    UART_PRINT("\n\n\rupdate\n\n\r");
                }
                else if (r.X < 0 && r.Y != 0) {
                    towardsJenga(r, 2);
                    timed_rotateLeft(r, 2);
                    towardsJenga(r, 2);
                    state.RoverState = FoundJenga;
                    m.roverState = FoundJenga;
                }
                else if (r.X > 0 && r.Y != 0) {
                    towardsJenga(r, 2);
                    timed_rotateRight(r, 2);
                    towardsJenga(r, 2);
                    state.RoverState = FoundJenga;
                    m.roverState = FoundJenga;
                }
            }
        case AvoidingObstacle:
            avoidObstacleXY(r);
            state.RoverState = SearchingForJenga;
            m.roverState = SearchingForJenga;
        case FoundJenga:
            UART_PRINT("\n\n\rFound Jenga Block\n\n\r");
            //avoidObstacleXY(r);
            state.RoverState = TowardDestination;
            m.roverState = TowardDestination;
        case TowardDestination:
            UART_PRINT("\n\n\rTowards Destination\n\n\r");
            timed_driveForward(r, 10);
            state.RoverState = AtDestination;
            m.roverState = AtDestination;
        case AtDestination:
            UART_PRINT("\n\n\rAt Destination\n\n\r");
            timed_rotateRight(r, 5);
            state.RoverState = ReadyForNextSensorValue;
            m.roverState = ReadyForNextSensorValue;
        case WaitingForArm:
            UART_PRINT("\n\n\rWaiting for Arm\n\n\r");
            if (r.armState == 0) {
                state.RoverState = SearchingForJenga;
                m.roverState = SearchingForJenga;
            }
        case ReadyForNextSensorValue:
            UART_PRINT("\n\n\rReady for next Sensor Value\n\n\r");
            if (r.ID += 1) {
                UART_PRINT("\n\n\rNext Data value Found\n\n\r");
                state.RoverState = SearchingForJenga;
                m.roverState = SearchingForJenga;
            }
        default:
            m.roverState = ErrorState;

    }*/
}

void avoidObstacleXY(struct recvMsg r) {
    if (r.Y > 0 && r.X == 0) {
        UART_PRINT("\n\n\r avoiding jenga block\n\n\r");
        motorStop();
        timed_rotateRight(r, 3);
        timed_driveForward(r, 3);
        timed_rotateLeft(r, 3);
    }
}

void towardsJenga(struct recvMsg r, int timetoRoll) {
    int timePassed;
    for (timePassed = timeVal.timePassed; timePassed <= timetoRoll; timePassed++) {
        UART_PRINT("\n\n\rMoving towards Jenga\n\n\r");
        driveForward(speed);
    }
    UART_PRINT("\n\n\rStopped at Jenga block\n\n\r");
    timeVal.motorTime = timetoRoll;
    motorStop();
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
}

void timed_rotateLeft(struct recvMsg r, int timetoRoll) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        rotateLeft(speed);
    }

    motorStop();
}

void timed_rotateRight(struct recvMsg r, int timetoRoll) {
    int timePassed;
    timetoRoll += timeVal.motorTime;
    for (timePassed = timeVal.motorTime; timePassed <= timetoRoll; timePassed++) {
        rotateRight(speed);
    }

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
