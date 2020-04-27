#ifndef __SERVER_CLIENT_CBS_H__
#define __SERVER_CLIENT_CBS_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/* Standard includes                                                         */
#include <stdlib.h>

/* Kernel (Non OS/Free-RTOS/TI-RTOS) includes                                */
#include "pthread.h"
#include "mqueue.h"

/* Common interface includes                                                 */
#include "uart_term.h"

/* MQTT library includes                                                      */
#include <ti/net/mqtt/mqttserver.h>
#include <ti/net/mqtt/mqttclient.h>

// custom include
#include <pubSubQueue.h>

//*****************************************************************************
// Macros
//*****************************************************************************
#define MAX_CONNECTION              1

#define PUBLISH_PUSH_BUTTON_PRESSED 1
#define MSG_RECV_BY_CLIENT          11
/* Client disconnected from remote broker       */
#define LOCAL_CLIENT_DISCONNECTION  12 
#define DISC_PUSH_BUTTON_PRESSED    14
#define THREAD_TERMINATE_REQ        15


// max buffer size (TODO maybe change so larger to accomodate max size)
#define MAX_MSG_BUF_SIZE 1000


//*****************************************************************************
// typedef enum/struct/union
//*****************************************************************************
struct publishMsgHeader
{
    uint32_t topicLen;
    uint32_t payLen;
    bool retain;
    bool dup;
    unsigned char qos;
};

struct client_info
{
    void *ctx;
};
//******************************************************************************
// APIs
//******************************************************************************

extern void MqttClientCallback(int32_t event,
                               void * metaData,
                               uint32_t metaDateLen,
                               void *data,
                               uint32_t dataLen);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __SERVER_CLIENT_CBS_H__
