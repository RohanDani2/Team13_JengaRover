#include <parseJSON.h>

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int addStringChecksum(unsigned char *string, size_t length){
    int chk = 0, x;
    for(x = 0; x < length; x++)
        chk += string[x];
    return chk;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int generateChecksum(int32_t roverState, int32_t seq) {
    char tok_string[MAX_TOK_LENGTH];
    int32_t chk = 0;
    chk += addStringChecksum("RoverState", strlen("RoverState"));
    chk += addStringChecksum("seq", strlen("seq"));
    int32_t s1 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", roverState);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    int32_t s2 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", seq);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    // if bad checksum just return -1, when checksum calculated will lead to failure
    if(s1 < MAX_TOK_LENGTH && s1 > 0 && s2 < MAX_TOK_LENGTH && s1 > 0) {
        return chk;
    }
    return -1;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------

int generateStatsChecksum(int32_t publish_attempt, int32_t subscribe_received, int32_t subscribe_shouldReceived, int32_t ID) {
    char tok_string[MAX_TOK_LENGTH];
    int32_t chk = 0;
    chk += addStringChecksum("ID", strlen("ID"));
    chk += addStringChecksum("publish_attempt", strlen("publish_attempt"));
    chk += addStringChecksum("subscribe_received", strlen("subscribe_received"));
    chk += addStringChecksum("subscribe_shouldReceived", strlen("subscribe_shouldReceived"));
    int32_t s0 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", ID);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    int32_t s1 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", publish_attempt);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    int32_t s2 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", subscribe_received);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    int32_t s3 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", subscribe_shouldReceived);
    chk += addStringChecksum(tok_string, strlen(tok_string));

    // if bad checksum just return -1, when checksum calculated will lead to failure
    if(s1 < MAX_TOK_LENGTH && s1 >= 0 && s2 < MAX_TOK_LENGTH && s2 >= 0 && s3 < MAX_TOK_LENGTH && s3 >= 0){
        return chk;
    }
    return -1;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int parseJSON(char* s, struct recvMsg *m){
    jsmn_parser p;
    jsmntok_t t[MAX_TOKENS];
    int32_t i = 0, checksum = 0, sval = 0;
    char tok_string[MAX_TOK_LENGTH];

    jsmn_init(&p);
    int r = jsmn_parse(&p, s, strlen(s), t, sizeof(t) / sizeof(t[0]));

    if(r < STATE_MSG_TOKENS){
        return 0;
    }
    else {
        m->seq = -1, m->armState=-1, m->ID =-1, m->X =-1, m->Y =-1,m->ObjWidth =-1, m->Jenga =-1, m->checksum=-1;
        for(i = 1; i < r; i++){
            if(jsoneq(s, &t[i], "Seq") == 0){
                checksum += addStringChecksum("Seq #", strlen("Seq #"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->seq = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "ID") == 0){
                checksum += addStringChecksum("ID", strlen("ID"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->ID = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "X") == 0){
                checksum += addStringChecksum("X", strlen("X"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->X = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "Y") == 0){
                checksum += addStringChecksum("Y", strlen("Y"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->Y = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }else{
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "ObjWidth") == 0){
                checksum += addStringChecksum("Obj Width", strlen("Obj Width"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->ObjWidth = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "Jenga") == 0){
                checksum += addStringChecksum("Jenga", strlen("Jenga"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->Jenga = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "ArmState") == 0){
                checksum += addStringChecksum("ArmState", strlen("ArmState"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->armState = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
            else if(jsoneq(s, &t[i], "checkSum") == 0) {
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval >= 0){
                        m->checksum = atoi(tok_string);
                    }
                    else {
                        return 0;
                    }
                }
                else { // unknown type
                    return 0;
                }
            }
        }

        //make sure assigned all member variables (succesful parse) and checksum is correct
        //fix checksum issue
        if(m->seq == -1
                || m->ID == -1
                || m->X == -1
                || m->Y == -1
                || m->ObjWidth == -1
                || m->Jenga == -1
                || m->armState == -1
                || m->checksum == -1) {
            UART_PRINT("%d", checksum);
            return 0;
        }
        return 1;
    }
};

