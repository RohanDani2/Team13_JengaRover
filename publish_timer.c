#include <publish_timer.h>

int pTimerFunct()
{
    Timer_Handle timer0;
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

// when timer expires, queue event to indicate that a message should be published
void pTimerCallback(Timer_Handle myHandle)
{

    struct pubSubMsg m;

    m.type = PUBLISH_TYPE;
    // generate simulated sensor data
    m.sensorValue = rand() % 50;

    //write message indicating timer expired event
    if(!sendMsgToPSQueue(&m)){
        dbgFail();
    }
}
