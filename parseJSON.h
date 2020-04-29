#ifndef PARSEJSON_H_
#define PARSEJSON_H_

// need to declare before including jsmn
#define JSMN_STATIC

#include <jsmn.h>
#include <string.h>
#include <stdint.h>
#include <uart_term.h>

#define MAX_TOKENS 128

#define MAX_TOK_LENGTH 100

#define STATE_MSG_TOKENS 6

struct recvMsg{
    int32_t ID;
    int32_t X;
    int32_t Y;
    int32_t ObjWidth;
    int32_t Jenga;
    int32_t armState;
    int32_t seq;
    int32_t checksum;
};

/* check if a token's string is equal to a test string (provided in jsmn repo)
 * return 1 on true, 0 on false
 */
int jsoneq(const char *json, jsmntok_t *tok, const char *s);

/* add string's value to checksum
 * return the value to add to checksum
 */
int addStringChecksum(unsigned char *string, size_t length);

/* generate checksum of message with sensor and sequence data
 * return the checksum
*/
int generateStatsChecksum(int32_t publish_attempt, int32_t subscribe_received, int32_t subscribe_shouldReceived, int32_t ID);
int generateChecksum(int32_t roverState, int32_t seq);

/* attempt to parse JSON string, verify checksum in process
 * return 1 on succesful parse, 0 on failed parse
 */
int parseJSON(char* s, struct recvMsg *m);


#endif /* PARSEJSON_H_ */
