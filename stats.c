#include <stats.h>

static struct statistics s = {0, 0, 0, -1, -1};

int32_t getPublishAttempt(){
    return s.published;
};

// update number of published by adding 1
void updatePublishAttempt(){
    s.published++;
};

int32_t getSubscribeReceived(){
    return s.received;
};

// update number messages received by adding 1
void updateSubscribeReceied(){
    s.received++;
};

int32_t getSubscribeShouldReceived(){
    return s.num_missed;
};

void updateSubscribeShouldReceivedState1(int32_t latest_seq){
    // account for restarted board
    if(latest_seq < s.last_seq_state1){
        s.last_seq_state1 = latest_seq;
    }else{
        if(s.last_seq_state1 != -1 && s.last_seq_state1 + 1 < latest_seq){
            s.num_missed+= latest_seq - (s.last_seq_state1 + 1);
        }
        s.last_seq_state1 = latest_seq;
    }
};
void updateSubscribeShouldReceivedState2(int32_t latest_seq){
    // account for restarted board
    if(latest_seq < s.last_seq_state2){
        s.last_seq_state2 = latest_seq;
    }else{
        if(s.last_seq_state2 != -1 && s.last_seq_state2 + 1 < latest_seq){
            s.num_missed+= latest_seq - (s.last_seq_state2 + 1);
        }
        s.last_seq_state2 = latest_seq;
    }
};




