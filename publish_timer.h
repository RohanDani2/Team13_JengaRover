#ifndef PUBLISH_TIMER_H_
#define PUBLISH_TIMER_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Driver Header files */
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include <debug.h>
#include <uart_term.h>
#include <pubSubQueue.h>

int pTimerFunct();
void pTimerCallback(Timer_Handle myHandle);

#endif /* PUBLISH_TIMER_H_ */
