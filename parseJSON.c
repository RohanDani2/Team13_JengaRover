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
int generateChecksum(int32_t sensor, int32_t seq){
    char tok_string[MAX_TOK_LENGTH];
    int32_t chk = 0;
    chk += addStringChecksum("sensor", strlen("sensor"));
    chk += addStringChecksum("seq", strlen("seq"));
    int32_t s1 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", sensor);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    int32_t s2 = snprintf(tok_string, MAX_TOK_LENGTH, "%d", seq);
    chk += addStringChecksum(tok_string, strlen(tok_string));
    // if bad checksum just return -1, when checksum calculated will lead to failure
    if(s1 < MAX_TOK_LENGTH && s1 > 0 && s2 < MAX_TOK_LENGTH && s1 > 0){
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
    }else{
        m->checksum = -1, m->state =-1, m->checksum=-1;
        for(i = 1; i < r; i++){
            if(jsoneq(s, &t[i], "state") == 0){
                checksum += addStringChecksum("state", strlen("state"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->state = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }else{
                        return 0;
                    }
                }else{ // unknown type
                    return 0;
                }
            }else if(jsoneq(s, &t[i], "seq") == 0){
                checksum += addStringChecksum("seq", strlen("seq"));
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->seq = atoi(tok_string);
                        checksum += addStringChecksum(tok_string, strlen(tok_string));
                    }else{
                        return 0;
                    }
                }else{ // unknown type
                    return 0;
                }
            }else if(jsoneq(s, &t[i], "checksum") == 0){
                if(t[i+1].type == JSMN_PRIMITIVE){
                    sval = snprintf(tok_string, MAX_TOK_LENGTH, "%.*s", t[i+1].end - t[i+1].start, s + t[i+1].start);
                    if(sval < MAX_TOK_LENGTH && sval > 0){
                        m->checksum = atoi(tok_string);
                    }else{
                        return 0;
                    }
                }else{ // unknown type
                    return 0;
                }
            }
        }

        //make sure assigned all member variables (succesful parse) and checksum is correct
        if(m->state == -1 || m->seq == -1 || m->checksum == -1 || checksum != m->checksum){
            UART_PRINT("%d", checksum);
            return 0;
        }
        return 1;
    }
};

