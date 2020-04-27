#include <mqtt_client_app.h>

//*****************************************************************************
//                 GLOBAL VARIABLES
//*****************************************************************************
static MQTTClient_Handle gMqttClient;

// Connection state: (0) - connected, (negative) - disconnected
int32_t gApConnectionState = -1;

uint32_t gInitState = 0;
bool gResetApplication = false;

// Receive task handle
uint32_t gUiConnFlag = 0;

// Subscription topics and qos values
const char *topic[SUBSCRIPTION_TOPIC_COUNT] =
{ SUBSCRIPTION_TOPIC0};
const char qos[SUBSCRIPTION_TOPIC_COUNT] =
{ MQTT_QOS_2, MQTT_QOS_2, MQTT_QOS_2, MQTT_QOS_2 };

// Client ID
// If ClientId isn't set, the MAC address of the device will be copied into
// the ClientID parameter.
char ClientId[CLIENT_ID_LEN] = {'\0'};

// start external main thread
int startMainMQTTTask(){
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
     retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
     if (retc != 0) {
         return 0;
     }
     priParam.sched_priority = 1;
     pthread_attr_setschedparam(&attrs, &priParam);
     retc = pthread_create(&thread0, &attrs, mainMqttTask, NULL);
     if (retc != 0) {
         return 0;
     }
     return 1;
}


//*****************************************************************************
//
//! MQTT_SendMsgToQueue - push queueElement to queue
//!
//! \param[in] struct message *queueElement
//!
//! \return 0 on success, -1 on error
//
//*****************************************************************************
int32_t MQTT_SendMsgToQueue(struct eventMessage *queueElement){
    if(sendMsgToEventQueue(queueElement)){
        return(0);
    }
    return(-1);
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void DisplayBanner(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t    CC32xx %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}

void * MqttClientThread(void * pvParameters)
{
    struct eventMessage queueElement;
    struct eventMessage queueElemRecv;

    MQTTClient_run((MQTTClient_Handle)pvParameters);

    queueElement.event = LOCAL_CLIENT_DISCONNECTION;
    queueElement.msgPtr = NULL;

    /*write message indicating disconnect Broker message.                   */
    if(MQTT_SendMsgToQueue(&queueElement))
    {
        UART_PRINT(
            "\n\n\rQueue is full, throw first msg and send the new one\n\n\r");
        if(readFromEventQueue(&queueElemRecv)){
            MQTT_SendMsgToQueue(&queueElement);
        }else{
            UART_PRINT("\n\n\rError throwing first msg in queue.\n\n\r");
            dbgFail();
        }
    }

    pthread_exit(0);

    return(NULL);
};

//*****************************************************************************
// attempt to send JSON formatted message over mqtt
// return 1 on success and 0 on failure
//*****************************************************************************
int sendMQTTJSON(char* publish_topic, char* publish_data ,int32_t sensorValue, int32_t sequence){

    // make sure can fit in bounds of buffer
    int32_t n = snprintf(publish_data, MAX_MSG_BUF_SIZE, "{\"sensor\": %d, \"seq\":%d, \"checksum\": %d}", sensorValue, sequence, generateChecksum(sensorValue, sequence));
    if(n < 0 || n > MAX_MSG_BUF_SIZE){
        return 0;
    }

    //send publish message
    int ret = MQTTClient_publish(gMqttClient, (char*) publish_topic, strlen(
                              (char*)publish_topic),
                          (char*)publish_data,
                          strlen((char*) publish_data), MQTT_QOS_2 |
                          ((RETAIN_ENABLE) ? MQTT_PUBLISH_RETAIN : 0));
    return ret == 0;
};

//*****************************************************************************
//
//! Task implementing MQTT Server plus client bridge
//!
//! This function
//!    1. Initializes network driver and connects to the default AP
//!    2. Initializes the mqtt client ans server libraries and set up MQTT
//!       with the remote broker.
//!    3. handles the callback signals
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************
void * MqttClient(void *pvParameters)
{
    struct eventMessage queueElemRecv;
    long lRetVal = -1;
    char *tmpBuff;

    /*Initializing Client and Subscribing to the Broker.                     */
    if(gApConnectionState >= 0)
    {
        lRetVal = MqttClient_start();
        if(lRetVal == -1)
        {
            UART_PRINT("MQTT Client lib initialization failed\n\r");
            pthread_exit(0);
            return(NULL);
        }
    }

    while(1){
        // only do something upon receiving from queue
        if(readFromEventQueue(&queueElemRecv)){

            switch(queueElemRecv.event)
            {
            //On-board client disconnected from remote broker, only
            //local MQTT network will work
            case LOCAL_CLIENT_DISCONNECTION:
                UART_PRINT("\n\rOn-board Client Disconnected\n\r\r\n");
                gUiConnFlag = 0;
                break;

            case THREAD_TERMINATE_REQ:
                gUiConnFlag = 0;
                pthread_exit(0);
                return(NULL);

            // if get unknown message, terminate
            default:
                UART_PRINT("Received unknown message type in event queue");
                dbgFail();
                break;
            }
        }
    }
}

//*****************************************************************************
//
//! This function connect the MQTT device to an AP with the SSID which was
//! configured in SSID_NAME definition which can be found in Network_if.h file,
//! if the device can't connect to to this AP a request from the user for other
//! SSID will appear.
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************
int32_t Mqtt_IF_Connect()
{
    int32_t lRetVal;
    char SSID_Remote_Name[32];
    int8_t Str_Length;

    memset(SSID_Remote_Name, '\0', sizeof(SSID_Remote_Name));
    Str_Length = strlen(SSID_NAME);

    if(Str_Length)
    {
        /*Copy the Default SSID to the local variable                        */
        strncpy(SSID_Remote_Name, SSID_NAME, Str_Length);
    }

    /*Display Application Banner                                             */
    DisplayBanner(APPLICATION_NAME);

    /*Reset The state of the machine                                         */
    Network_IF_ResetMCUStateMachine();

    /*Start the driver                                                       */
    lRetVal = Network_IF_InitDriver(ROLE_STA);
    if(lRetVal < 0)
    {
        UART_PRINT("Failed to start SimpleLink Device\n\r", lRetVal);
        return(-1);
    }

    /*Initialize AP security params                                          */
    SlWlanSecParams_t SecurityParams = { 0 };
    SecurityParams.Key = (signed char *) SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;

    /*Connect to the Access Point                                            */
    lRetVal = Network_IF_ConnectAP(SSID_Remote_Name, SecurityParams);
    if(lRetVal < 0)
    {
        UART_PRINT("Connection to an AP failed\n\r");
        return(-1);
    }

    sleep(1);

    return(0);
}

//*****************************************************************************
//!
//! MQTT Start - Initialize and create all the items required to run the MQTT
//! protocol
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************
void Mqtt_start()
{
    int32_t threadArg = 100;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int32_t retc = 0;
    mq_attr attr;
    unsigned mode = 0;

    // initialize queue
    if(!createEventMsgQueue()){
        UART_PRINT("Error creating queue.\n\r");
        dbgFail();
    }

    /*Set priority and stack size attributes                                 */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, MQTTTHREADSIZE);
    retc |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);

    if(retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT thread create fail\n\r");
        return;
    }

    pthread_t mqttThread = (pthread_t) NULL;
    retc = pthread_create(&mqttThread, &pAttrs, MqttClient, (void *) &threadArg);
    if(retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT thread create fail\n\r");
        return;
    }


    gInitState &= ~MQTT_INIT_STATE;
}

//*****************************************************************************
//!
//! MQTT Stop - Close the client instance and free all the items required to
//! run the MQTT protocol
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************

void Mqtt_Stop()
{
    struct eventMessage queueElement;
    struct eventMessage queueElemRecv;

    if(gApConnectionState >= 0)
    {
        Mqtt_ClientStop(1);
    }

    queueElement.event = THREAD_TERMINATE_REQ;
    queueElement.msgPtr = NULL;

    /*write message indicating publish message                               */
    if(MQTT_SendMsgToQueue(&queueElement))
    {
        UART_PRINT(
            "\n\n\rQueue is full, throw first msg and send the new one\n\n\r");
        if(readFromEventQueue(&queueElemRecv)){
            MQTT_SendMsgToQueue(&queueElement);
        }else{
            UART_PRINT("\n\n\rError throwing first msg in queue.\n\n\r");
            dbgFail();
        }
    }

    sleep(2);

    sl_Stop(SL_STOP_TIMEOUT);
    UART_PRINT("\n\r Client Stop completed\r\n");

}

int32_t MqttClient_start()
{
    int32_t lRetVal = -1;
    int32_t iCount = 0;

    int32_t threadArg = 100;
    pthread_attr_t pAttrs;
    struct sched_param priParam;

    MQTTClient_ConnParams Mqtt_ClientCtx =
    {
        MQTTCLIENT_NETCONN_URL,
        SERVER_ADDRESS,
        PORT_NUMBER, 0, 0, 0,
        NULL
    };

    MQTTClient_Params MqttClientExmple_params;
    MqttClientExmple_params.clientId = ClientId;
    MqttClientExmple_params.connParams = &Mqtt_ClientCtx;
    MqttClientExmple_params.mqttMode31 = MQTT_3_1;
    MqttClientExmple_params.blockingSend = true;

    gInitState |= CLIENT_INIT_STATE;

    /*Initialize MQTT client lib                                             */
    gMqttClient = MQTTClient_create(MqttClientCallback,
                                    &MqttClientExmple_params);
    if(gMqttClient == NULL)
    {
        /*lib initialization failed                                          */
        gInitState &= ~CLIENT_INIT_STATE;
        return(-1);
    }

    /*Open Client Receive Thread start the receive task. Set priority and    */
    /*stack size attributes                                                  */

    pthread_t g_rx_task_hndl = (pthread_t) NULL;
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;
    lRetVal = pthread_attr_setschedparam(&pAttrs, &priParam);
    lRetVal |= pthread_attr_setstacksize(&pAttrs, RXTASKSIZE);
    lRetVal |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);
    lRetVal |=
        pthread_create(&g_rx_task_hndl, &pAttrs, MqttClientThread,
                       (void *) &threadArg);
    if(lRetVal != 0)
    {
        UART_PRINT("Client Thread Create Failed failed\n\r");
        gInitState &= ~CLIENT_INIT_STATE;
        return(-1);
    }

    /*setting will parameters                                                */
    MQTTClient_Will will_param =
    {
        WILL_TOPIC,
        WILL_MSG,
        WILL_QOS,
        WILL_RETAIN
    };

    MQTTClient_set(gMqttClient, MQTTClient_WILL_PARAM, &will_param,
                   sizeof(will_param));


    /*Initiate MQTT Connect                                                  */
    if(gApConnectionState >= 0)
    {
#if CLEAN_SESSION == false
        bool clean = CLEAN_SESSION;
        MQTTClient_set(gMqttClient, MQTTClient_CLEAN_CONNECT, (void *)&clean,
                       sizeof(bool));
#endif
        /*The return code of MQTTClient_connect is the ConnACK value that
           returns from the server */
        lRetVal = MQTTClient_connect(gMqttClient);

        /*negative lRetVal means error,
           0 means connection successful without session stored by the server,
           greater than 0 means successful connection with session stored by
           the server */
        if(0 > lRetVal)
        {
            /*lib initialization failed                                      */
            UART_PRINT("Connection to broker failed, Error code: %d\n\r",
                       lRetVal);

            gUiConnFlag = 0;
        }
        else
        {
            gUiConnFlag = 1;
        }
        /*Subscribe to topics when session is not stored by the server       */
        if((gUiConnFlag == 1) && (0 == lRetVal))
        {
            uint8_t subIndex;
            MQTTClient_SubscribeParams subscriptionInfo[
                SUBSCRIPTION_TOPIC_COUNT];

            for(subIndex = 0; subIndex < SUBSCRIPTION_TOPIC_COUNT; subIndex++)
            {
                subscriptionInfo[subIndex].topic = topic[subIndex];
                subscriptionInfo[subIndex].qos = qos[subIndex];
            }

            if(MQTTClient_subscribe(gMqttClient, subscriptionInfo,
                                    SUBSCRIPTION_TOPIC_COUNT) < 0)
            {
                UART_PRINT("\n\r Subscription Error \n\r");
                MQTTClient_disconnect(gMqttClient);
                gUiConnFlag = 0;
            }
            else
            {
                for(iCount = 0; iCount < SUBSCRIPTION_TOPIC_COUNT; iCount++)
                {
                    UART_PRINT("Client subscribed on %s\n\r,", topic[iCount]);
                }
            }
        }
    }

    gInitState &= ~CLIENT_INIT_STATE;

    return(0);
}

//*****************************************************************************
//!
//! MQTT Client stop - Unsubscribe from the subscription topics and exit the
//! MQTT client lib.
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************

void Mqtt_ClientStop(uint8_t disconnect)
{
    uint32_t iCount;

    MQTTClient_UnsubscribeParams subscriptionInfo[SUBSCRIPTION_TOPIC_COUNT];

    for(iCount = 0; iCount < SUBSCRIPTION_TOPIC_COUNT; iCount++)
    {
        subscriptionInfo[iCount].topic = topic[iCount];
    }

    MQTTClient_unsubscribe(gMqttClient, subscriptionInfo,
                           SUBSCRIPTION_TOPIC_COUNT);
    for(iCount = 0; iCount < SUBSCRIPTION_TOPIC_COUNT; iCount++)
    {
        UART_PRINT("Unsubscribed from the topic %s\r\n", topic[iCount]);
    }
    gUiConnFlag = 0;

    /*exiting the Client library                                             */
    MQTTClient_delete(gMqttClient);
}

//*****************************************************************************
//!
//! Utility function which prints the borders
//!
//! \param[in] ch  -  hold the charater for the border.
//! \param[in] n   -  hold the size of the border.
//!
//! \return none.
//!
//*****************************************************************************

void printBorder(char ch,
                 int n)
{
    int i = 0;

    for(i = 0; i < n; i++)
    {
        putch(ch);
    }
}

//*****************************************************************************
//!
//! Set the ClientId with its own mac address
//! This routine converts the mac address which is given
//! by an integer type variable in hexadecimal base to ASCII
//! representation, and copies it into the ClientId parameter.
//!
//! \param  macAddress  -   Points to string Hex.
//!
//! \return void.
//!
//*****************************************************************************
int32_t SetClientIdNamefromMacAddress()
{
    int32_t ret = 0;
    uint8_t Client_Mac_Name[2];
    uint8_t Index;
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;
    uint8_t macAddress[SL_MAC_ADDR_LEN];

    /*Get the device Mac address */
    ret = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);

    /*When ClientID isn't set, use the mac address as ClientID               */
    if(ClientId[0] == '\0')
    {
        /*6 bytes is the length of the mac address                           */
        for(Index = 0; Index < SL_MAC_ADDR_LEN; Index++)
        {
            /*Each mac address byte contains two hexadecimal characters      */
            /*Copy the 4 MSB - the most significant character                */
            Client_Mac_Name[0] = (macAddress[Index] >> 4) & 0xf;
            /*Copy the 4 LSB - the least significant character               */
            Client_Mac_Name[1] = macAddress[Index] & 0xf;

            if(Client_Mac_Name[0] > 9)
            {
                /*Converts and copies from number that is greater than 9 in  */
                /*hexadecimal representation (a to f) into ascii character   */
                ClientId[2 * Index] = Client_Mac_Name[0] + 'a' - 10;
            }
            else
            {
                /*Converts and copies from number 0 - 9 in hexadecimal       */
                /*representation into ascii character                        */
                ClientId[2 * Index] = Client_Mac_Name[0] + '0';
            }
            if(Client_Mac_Name[1] > 9)
            {
                /*Converts and copies from number that is greater than 9 in  */
                /*hexadecimal representation (a to f) into ascii character   */
                ClientId[2 * Index + 1] = Client_Mac_Name[1] + 'a' - 10;
            }
            else
            {
                /*Converts and copies from number 0 - 9 in hexadecimal       */
                /*representation into ascii character                        */
                ClientId[2 * Index + 1] = Client_Mac_Name[1] + '0';
            }
        }
    }
    return(ret);
}

//*****************************************************************************
//!
//! Utility function which Display the app banner
//!
//! \param[in] appName     -  holds the application name.
//! \param[in] appVersion  -  holds the application version.
//!
//! \return none.
//!
//*****************************************************************************

int32_t DisplayAppBanner(char* appName,
                         char* appVersion)
{
    int32_t ret = 0;
    uint8_t macAddress[SL_MAC_ADDR_LEN];
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;
    uint16_t ConfigSize = 0;
    uint8_t ConfigOpt = SL_DEVICE_GENERAL_VERSION;
    SlDeviceVersion_t ver = {0};

    ConfigSize = sizeof(SlDeviceVersion_t);

    /*Print device version info. */
    ret =
        sl_DeviceGet(SL_DEVICE_GENERAL, &ConfigOpt, &ConfigSize,
                     (uint8_t*)(&ver));

    /*Print device Mac address */
    ret |= (int32_t)sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);

    UART_PRINT("\n\r\t");
    printBorder('=', 44);
    UART_PRINT("\n\r\t   %s Example Ver: %s",appName, appVersion);
    UART_PRINT("\n\r\t");
    printBorder('=', 44);
    UART_PRINT("\n\r\n\r\t CHIP: 0x%x",ver.ChipId);
    UART_PRINT("\n\r\t MAC:  %d.%d.%d.%d",ver.FwVersion[0],ver.FwVersion[1],
               ver.FwVersion[2],
               ver.FwVersion[3]);
    UART_PRINT("\n\r\t PHY:  %d.%d.%d.%d",ver.PhyVersion[0],ver.PhyVersion[1],
               ver.PhyVersion[2],
               ver.PhyVersion[3]);
    UART_PRINT("\n\r\t NWP:  %d.%d.%d.%d",ver.NwpVersion[0],ver.NwpVersion[1],
               ver.NwpVersion[2],
               ver.NwpVersion[3]);
    UART_PRINT("\n\r\t ROM:  %d",ver.RomVersion);
    UART_PRINT("\n\r\t HOST: %s", SL_DRIVER_VERSION);
    UART_PRINT("\n\r\t MAC address: %02x:%02x:%02x:%02x:%02x:%02x", macAddress[0],
               macAddress[1], macAddress[2], macAddress[3], macAddress[4],
               macAddress[5]);
    UART_PRINT("\n\r\n\r\t");
    printBorder('=', 44);
    UART_PRINT("\n\r\n\r");

    return(ret);
}

int createMotorThread() {
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
   retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
   if (retc != 0) {
       return 0;
   }
   priParam.sched_priority = 1;
   pthread_attr_setschedparam(&attrs, &priParam);
   retc = pthread_create(&thread0, &attrs, motorThread, NULL);
   if (retc != 0) {
       return 0;
   }
   return 1;
}

void mainThread(void * args)
{
    uint32_t count = 0;
    pthread_t spawn_thread = (pthread_t) NULL;
    pthread_attr_t pAttrs_spawn;
    struct sched_param priParam;
    int32_t retc = 0;

    /*Initialize SlNetSock layer with CC31xx/CC32xx interface */
    SlNetIf_init(0);
    SlNetIf_add(SLNETIF_ID_1, "CC32xx",
                (const SlNetIf_Config_t *)&SlNetIfConfigWifi,
                SLNET_IF_WIFI_PRIO);

    SlNetSock_init(0);
    SlNetUtil_init(0);

    SPI_init();


    // open UART
    if(!InitTerm()){
        dbgFail();
    }

    // initialize GPIO for debugging
    initGPIO();

    /*Create the sl_Task                                                     */
    pthread_attr_init(&pAttrs_spawn);
    priParam.sched_priority = SPAWN_TASK_PRIORITY;
    retc = pthread_attr_setschedparam(&pAttrs_spawn, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs_spawn, TASKSTACKSIZE);
    retc |= pthread_attr_setdetachstate
                                    (&pAttrs_spawn, PTHREAD_CREATE_DETACHED);

    retc = pthread_create(&spawn_thread, &pAttrs_spawn, sl_Task, NULL);

    if(retc != 0)
    {
        UART_PRINT("could not create simplelink task\n\r");
        dbgFail();
    }

    retc = sl_Start(0, 0, 0);
    if(retc < 0)
    {
        /*Handle Error */
        UART_PRINT("\n sl_Start failed\n");
        dbgFail();
    }

    /*Output device information to the UART terminal */
    retc = DisplayAppBanner(APPLICATION_NAME, APPLICATION_VERSION);
    /*Set the ClientId with its own mac address */
    retc |= SetClientIdNamefromMacAddress();


    retc = sl_Stop(SL_STOP_TIMEOUT);
    if(retc < 0)
    {
        /*Handle Error */
        UART_PRINT("\n sl_Stop failed\n");
        dbgFail();
    }

    if(retc < 0)
    {
        /*Handle Error */
        UART_PRINT("mqtt_client - Unable to retrieve device information \n");
        dbgFail();
    }

    if(!createPSMsgQueue()) {
        UART_PRINT("fail queue");
    }

    if (!pTimerFunct()) {
        dbgFail();
    }

    gResetApplication = false;
    topic[0] = SUBSCRIPTION_TOPIC0;
    gInitState = 0;

    //Connect to AP
    gApConnectionState = Mqtt_IF_Connect();

    gInitState |= MQTT_INIT_STATE;
    //Run MQTT Main Thread (it will open the Client and Server)
    Mqtt_start();

    if(!startMainMQTTTask()) {
        UART_PRINT("failed mqtt task");
    }

    if(!createMotorThread()){
        UART_PRINT("failed motor task");
    }

    while(1)
    {


        /* ------------------------------------------------------------------------------
         * ------------------------------------------------------------------------------
         * TODO - change this
         * ------------------------------------------------------------------------------
         * ------------------------------------------------------------------------------
         */
        /*Wait for init to be completed!!!                                   */
        while(gInitState != 0)
        {
            UART_PRINT(".");
            sleep(1);
        }
        UART_PRINT(".\r\n");

        /* ------------------------------------------------------------------------------
         * ------------------------------------------------------------------------------
         * TODO - change this
         * ------------------------------------------------------------------------------
         * ------------------------------------------------------------------------------
         */
        while(gResetApplication == false)
        {
            ;
        }

        UART_PRINT("TO Complete - Closing all threads and resources\r\n");

        /*Stop the MQTT Process                                              */
        Mqtt_Stop();

        UART_PRINT("reopen MQTT # %d  \r\n", ++count);
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
