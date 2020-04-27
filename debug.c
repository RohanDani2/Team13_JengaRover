#include <debug.h>

static UART_Handle uart0;

// check that UART can be created
int initUART(){
    UART_init();

    /* Create a UART with the parameters below. */
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.baudRate = 9600;
    uartParams.readEcho = UART_ECHO_OFF;

    uart0 = UART_open(CONFIG_UART_0, &uartParams);

    return uart0 != NULL;
}

// initalize GPIO
void initGPIO(){
    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_PIN_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_PIN_1, GPIO_OFF);
    GPIO_setConfig(CONFIG_GPIO_PIN_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_PIN_2, GPIO_OFF);
    GPIO_setConfig(CONFIG_GPIO_PIN_3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_PIN_3, GPIO_OFF);
    GPIO_setConfig(CONFIG_GPIO_PIN_4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_PIN_4, GPIO_OFF);
    GPIO_setConfig(CONFIG_GPIO_PIN_5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_PIN_5, GPIO_OFF);
    GPIO_setConfig(CONFIG_GPIO_PIN_7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_PIN_7, GPIO_OFF);
}

// write to UART
void dbgUARTVal(unsigned char outVal){

    if (uart0 != NULL) {
        char word[] = outVal;
        UART_write(uart0, (const void *)word, sizeof(word));
    }
}

// write string to UART
void dbgUARTString(char *word){
    if (uart0 != NULL) {
        UART_write(uart0, (const void *)word, strlen(word));
    }
}

// output an error code and suspend task
void dbgFail(){
    dbgUARTString("fail");
    dbgOutputLoc(DEBUG_ERROR);
    vTaskSuspend(NULL);
}

// write to GPIO pins
void dbgOutputLoc(unsigned int outLoc){
    if(outLoc > 127){
        dbgFail();
    }

    if (outLoc & (1 << 1))
        GPIO_write(CONFIG_GPIO_PIN_1, GPIO_ON);
    else
        GPIO_write(CONFIG_GPIO_PIN_1, GPIO_OFF);
    if (outLoc & (1 << 2))
        GPIO_write(CONFIG_GPIO_PIN_2, GPIO_ON);
    else
        GPIO_write(CONFIG_GPIO_PIN_2, GPIO_OFF);
    if (outLoc & (1 << 3))
        GPIO_write(CONFIG_GPIO_PIN_3, GPIO_ON);
    else
        GPIO_write(CONFIG_GPIO_PIN_3, GPIO_OFF);
    if (outLoc & (1 << 4))
        GPIO_write(CONFIG_GPIO_PIN_4, GPIO_ON);
    else
        GPIO_write(CONFIG_GPIO_PIN_4, GPIO_OFF);
    if (outLoc & (1 << 5))
        GPIO_write(CONFIG_GPIO_PIN_5, GPIO_ON);
    else
        GPIO_write(CONFIG_GPIO_PIN_5, GPIO_OFF);

    GPIO_toggle(CONFIG_GPIO_PIN_7);
}


