#include <maintask.h>

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

void *motorThread(void *arg0) {

    //createThread();
    //startEncoderTransmission();

    while (1) {
        driveForward(speed);

        //read_encoder(MOTOR1);
        //read_encoder(MOTOR2);
        //read_encoder(MOTOR3);
    }
}

/*
void createThread() {
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

     Create application thread
     pthread_attr_init(&attrs);

     detachState = PTHREAD_CREATE_DETACHED;
      Set priority and stack size attributes
     retc = pthread_attr_setdetachstate(&attrs, detachState);
     if (retc != 0) {
          pthread_attr_setdetachstate() failed
         while (1);
     }

     retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
     if (retc != 0) {
          pthread_attr_setstacksize() failed
         while (1);
     }

      Create slave thread
     priParam.sched_priority = 1;
     pthread_attr_setschedparam(&attrs, &priParam);

     retc = pthread_create(&thread0, &attrs, testThread, NULL);
     if (retc != 0) {
          pthread_create() failed
         while (1);
     }
}
*/
