#include "uart_term.h"

// static buffer
static char pcBuff[BUF_SIZE];

// UART handle
static UART_Handle uartHandle;
static UART_Handle uart;

static char buffer[100];

extern int vsnprintf(char * s,
                     size_t n,
                     const char * format,
                     va_list arg);

//*****************************************************************************
//
//! Initialization
//!
//! This function
//!        1. Configures the UART to be used.
//!
//! \param  none
//!
//! \return none
//
//*****************************************************************************
int InitTerm(void)
{
    UART_Params uartParams;

    UART_init();
    UART_Params_init(&uartParams);

    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;

    uart = UART_open(CONFIG_UART_0, &uartParams);
    uartHandle = UART_open(CONFIG_UART_1, &uartParams);
}

//*****************************************************************************
//
//! prints the formatted string on to the console
//!
//! \param[in]  format  - is a pointer to the character string specifying the
//!                       format in the following arguments need to be
//!                       interpreted.
//! \param[in]  [variable number of] arguments according to the format in the
//!             first parameters
//!
//! \return count of characters printed
//
//*****************************************************************************
int Report(const char *pcFormat,
           ...)
{
    int iRet = 0;
    va_list list;

    va_start(list,pcFormat);
    iRet = vsnprintf(pcBuff, BUF_SIZE, pcFormat, list);
    va_end(list);
    Message(pcBuff);

    return(iRet);
}

//*****************************************************************************
//
//! Trim the spaces from left and right end of given string
//!
//! \param  pcInput - string on which trimming happens
//!
//! \return length of trimmed string
//
//*****************************************************************************
int TrimSpace(char * pcInput)
{
    size_t size;
    char        *endStr;
    char        *strData = pcInput;
    char index = 0;

    size = strlen(strData);

    if(!size)
    {
        return(0);
    }

    endStr = strData + size - 1;
    while((endStr >= strData) && (IS_SPACE(*endStr)))
    {
        endStr--;
    }
    *(endStr + 1) = '\0';

    while(*strData && IS_SPACE(*strData))
    {
        strData++;
        index++;
    }
    memmove(pcInput, strData, strlen(strData) + 1);

    return(strlen(pcInput));
}

//*****************************************************************************
//
//! Get the Command string from UART
//!
//! \param[in]  pucBuffer   - is the command store to which command will be
//!                           populated
//! \param[in]  ucBufLen    - is the length of buffer store available
//!
//! \return Length of the bytes received. -1 if buffer length exceeded.
//!
//*****************************************************************************
int GetCmd(char *pcBuffer,
           unsigned int uiBufLen)
{
    char cChar;
    int iLen = 0;

    UART_readPolling(uartHandle, &cChar, 1);

    iLen = 0;

    //
    // Checking the end of Command
    //
    while(1)
    {
        //
        // Handling overflow of buffer
        //
        if(iLen >= uiBufLen)
        {
            return(-1);
        }

        //
        // Copying Data from UART into a buffer
        //
        if((cChar == '\r') || (cChar == '\n'))
        {
            UART_writePolling(uartHandle, &cChar, 1);
            break;
        }
        else if(cChar == '\b')
        {
            //
            // Deleting last character when you hit backspace
            //
            char ch;

            UART_writePolling(uartHandle, &cChar, 1);
            ch = ' ';
            UART_writePolling(uartHandle, &ch, 1);
            if(iLen)
            {
                UART_writePolling(uartHandle, &cChar, 1);
                iLen--;
            }
            else
            {
                ch = '\a';
                UART_writePolling(uartHandle, &ch, 1);
            }
        }
        else
        {
            //
            // Echo the received character
            //
            UART_writePolling(uartHandle, &cChar, 1);

            *(pcBuffer + iLen) = cChar;
            iLen++;
        }

        UART_readPolling(uartHandle, &cChar, 1);
    }

    *(pcBuffer + iLen) = '\0';

    return(iLen);
}

//*****************************************************************************
//
//! Outputs a character string to the console
//!
//! This function
//!        1. prints the input string character by character on to the console.
//!
//! \param[in]  str - is the pointer to the string to be printed
//!
//! \return none
//!
//! \note If UART_NONPOLLING defined in than Message or UART write should be
//!       called in task/thread context only.
//
//*****************************************************************************
void Message(const char *str)
{
#ifdef UART_NONPOLLING
    UART_write(uartHandle, str, strlen(str));
#else
    UART_writePolling(uartHandle, str, strlen(str));
#endif
}

//*****************************************************************************
//
//! Clear the console window
//!
//! This function
//!        1. clears the console window.
//!
//! \param  none
//!
//! \return none
//
//*****************************************************************************
void ClearTerm()
{
    Message("\33[2J\r");
}

//*****************************************************************************
//
//! Read a character from the console
//!
//! \param none
//!
//! \return Character
//
//*****************************************************************************
char getch(void)
{
    char ch;

    UART_readPolling(uartHandle, &ch, 1);
    return(ch);
}

//*****************************************************************************
//
//! Outputs a character to the console
//!
//! \param[in]  char    - A character to be printed
//!
//! \return none
//
//*****************************************************************************
void putch(char ch)
{
    UART_writePolling(uartHandle, &ch, 1);
}

void chooseEncoder(int encoder_val, char encoderChosen) {
    if (encoderChosen == '1') {
        int c;
        c = snprintf(buffer, 100, "Encoder 1: %d \r\n", encoder_val);
        UART_write(uartHandle, buffer, c);
    }
    else if(encoderChosen == '2')
    {
        int c;
        c = snprintf(buffer, 100, "Encoder 2: %d \r\n", encoder_val);
        UART_write(uartHandle, buffer, c);
    }
    else if(encoderChosen == '3')
    {
        int c;
        c = snprintf(buffer, 100, "Encoder 3: %d \r\n", encoder_val);
        UART_write(uartHandle, buffer, c);
    }
}

void initBaudRate() {
    unsigned char baudAddress = 170;
    UART_write(uart, &baudAddress, sizeof(baudAddress));
}

void driveForward(char speed) {

    unsigned char forward = 0;
    unsigned char backward = 1;
    unsigned char motor1_address = 128;
    unsigned char motor2_address = 129;

    unsigned char motor1_Mask = ((motor1_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor1_address, sizeof(motor1_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor1_Mask, sizeof(motor1_Mask));

    unsigned char motor2_Mask = ((motor2_address + backward + speed) & 0b01111111);
    UART_write(uart, &motor2_address, sizeof(motor2_address));
    UART_write(uart, &backward, sizeof(backward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor2_Mask, sizeof(motor2_Mask));
}

void driveBackward(char speed) {

    unsigned char forward = 0;
    unsigned char backward = 1;
    unsigned char motor1_address = 128;
    unsigned char motor2_address = 129;

    unsigned char motor1_Mask = ((motor1_address + backward + speed) & 0b01111111);
    UART_write(uart, &motor1_address, sizeof(motor1_address));
    UART_write(uart, &backward, sizeof(backward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor1_Mask, sizeof(motor1_Mask));

    unsigned char motor2_Mask = ((motor2_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor2_address, sizeof(motor2_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor2_Mask, sizeof(motor2_Mask));
}

void rotateRight(char speed) {

    unsigned char backward = 1;
    unsigned char motor1_address = 128;
    unsigned char motor2_address = 129;
    unsigned char motor3_address = 130;

    unsigned char motor1_Mask = ((motor1_address + backward + speed) & 0b01111111);
    UART_write(uart, &motor1_address, sizeof(motor1_address));
    UART_write(uart, &backward, sizeof(backward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor1_Mask, sizeof(motor1_Mask));

    unsigned char motor2_Mask = ((motor2_address + backward + speed) & 0b01111111);
    UART_write(uart, &motor2_address, sizeof(motor2_address));
    UART_write(uart, &backward, sizeof(backward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor2_Mask, sizeof(motor2_Mask));

    unsigned char motor3_Mask = ((motor3_address + backward + speed) & 0b01111111);
    UART_write(uart, &motor3_address, sizeof(motor3_address));
    UART_write(uart, &backward, sizeof(backward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor3_Mask, sizeof(motor3_Mask));
}

void rotateLeft(char speed) {

    unsigned char forward = 0;
    unsigned char motor1_address = 128;
    unsigned char motor2_address = 129;
    unsigned char motor3_address = 130;

    unsigned char motor1_Mask = ((motor1_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor1_address, sizeof(motor1_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor1_Mask, sizeof(motor1_Mask));

    unsigned char motor2_Mask = ((motor2_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor2_address, sizeof(motor2_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor2_Mask, sizeof(motor2_Mask));

    unsigned char motor3_Mask = ((motor3_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor3_address, sizeof(motor3_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor3_Mask, sizeof(motor3_Mask));
}

void motorStop() {
    char speed = 0;
    unsigned char forward = 0;
    unsigned char motor1_address = 128;
    unsigned char motor2_address = 129;
    unsigned char motor3_address = 130;

    unsigned char motor1_Mask = ((motor1_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor1_address, sizeof(motor1_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor1_Mask, sizeof(motor1_Mask));

    unsigned char motor2_Mask = ((motor2_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor2_address, sizeof(motor2_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor2_Mask, sizeof(motor2_Mask));

    unsigned char motor3_Mask = ((motor3_address + forward + speed) & 0b01111111);
    UART_write(uart, &motor3_address, sizeof(motor3_address));
    UART_write(uart, &forward, sizeof(forward));
    UART_write(uart, &speed, sizeof(speed));
    UART_write(uart, &motor3_Mask, sizeof(motor3_Mask));
}
