#ifndef STATS_H_
#define STATS_H_

#include <stdint.h>

struct statistics
{
    int32_t published;
    int32_t received;
    int32_t num_missed;
    int32_t last_seq_state1;
    int32_t last_seq_state2;
};

int32_t getPublishAttempt();
void updatePublishAttempt();
int32_t getSubscribeReceived();
void updateSubscribeReceied();
int32_t getSubscribeShouldReceived();
void updateSubscribeShouldReceivedState1(int32_t latest_seq);
void updateSubscribeShouldReceivedState2(int32_t latest_seq);

#endif /* STATS_H_ */
