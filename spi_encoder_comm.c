#include "spi_encoder_comm.h"

/* global buffers to handle the read and write from spi */
uint8_t register_tx_buff[0x2] = {0};
uint8_t register_rx_buff[0x2] = {0};
uint8_t tx_buffer[0x5] = {0};
uint8_t rx_buffer[0x5] = {0};

void initSPIParams() {
    SPI_init();
    SPI_Params_init(&spiParams);

    spiParams.transferMode = SPI_MODE_CALLBACK;
    spiParams.transferCallbackFxn = encoderCallback;
    spiParams.dataSize = 8;
    spiParams.bitRate = 250000;

    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
    if (masterSpi == NULL) {
        while (1);
    }

    GPIO_setConfig(SPI_Encoder1, GPIO_CFG_OUTPUT);
    GPIO_setConfig(SPI_Encoder2, GPIO_CFG_OUTPUT);
    GPIO_setConfig(SPI_Encoder3, GPIO_CFG_OUTPUT);

    GPIO_write(SPI_Encoder1, 1);
    GPIO_write(SPI_Encoder2, 1);
    GPIO_write(SPI_Encoder3, 1);
}

void startEncoderTransmission() {

    initEncoder(MOTOR1);
    initEncoder(MOTOR2);
    initEncoder(MOTOR3);

    clear_encoder(MOTOR1);
    choose_Register(MOTOR1);

    clear_encoder(MOTOR2);
    choose_Register(MOTOR2);

    clear_encoder(MOTOR3);
    choose_Register(MOTOR3);
}

void encoderCallback(SPI_Handle handle, SPI_Transaction *transaction)
{
    encoderSelected encoder = (encoderSelected)transaction->arg;
    uint8_t *value = transaction->rxBuf;
    uint32_t count_value;
    char whichEncoder;
    if (encoder == Encoder1) {
        count_value = (value[1] << 24) | (value[2] << 16) | (value[3] << 8) | (value[4]);
        whichEncoder = '1';
        sendEncodertoQueue(count_value, whichEncoder);
    }
    else if(encoder == Encoder2) {
        count_value = (value[1] << 24) | (value[2] << 16) | (value[3] << 8) | (value[4]);
        whichEncoder = '2';
        sendEncodertoQueue(count_value, whichEncoder);
    }
    else if(encoder == Encoder3) {
        count_value = (value[1] << 24) | (value[2] << 16) | (value[3] << 8) | (value[4]);
        whichEncoder = '3';
        sendEncodertoQueue(count_value, whichEncoder);
    }
}

void initEncoder(motorAddress motor_Address)
{

    if (motor_Address == MOTOR1) {
        GPIO_write(SPI_Encoder1, 0);
        register_tx_buff[0] = 0x88;
        register_tx_buff[1] = 0x03;
        transaction.count = 0x2;
        transaction.txBuf = register_tx_buff;
        transaction.rxBuf = register_rx_buff;
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder1, 1);
    }
    else if (motor_Address == MOTOR2) {
        GPIO_write(SPI_Encoder2, 0);
        register_tx_buff[0] = 0x88; /* Write to MDR0 */
        register_tx_buff[1] = 0x03;
        transaction.count = 0x2;
        transaction.txBuf = register_tx_buff;
        transaction.rxBuf = register_rx_buff;
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder2, 1);
    }
    else if (motor_Address == MOTOR3) {
        GPIO_write(SPI_Encoder3, 0);
        register_tx_buff[0] = 0x88;
        register_tx_buff[1] = 0x03;
        transaction.count = 0x2;
        transaction.txBuf = register_tx_buff;
        transaction.rxBuf = register_rx_buff;
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder3, 1);
    }
}

void read_encoder(motorAddress motor_Address)
{
    encoderSelected encoder;

    if (motor_Address == MOTOR1) {
        tx_buffer[0] = 0x60;
        tx_buffer[1] = 0x00;
        tx_buffer[2] = 0x00;
        tx_buffer[3] = 0x00;
        tx_buffer[4] = 0x00;
        transaction.count = 0x5;
        transaction.txBuf = tx_buffer;
        transaction.rxBuf = rx_buffer;
        encoder = Encoder1;
        transaction.arg = (void *) encoder;
        GPIO_write(SPI_Encoder1, 0);
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder1, 1);
    }
    else if (motor_Address == MOTOR2) {
        tx_buffer[0] = 0x60;
        tx_buffer[1] = 0x00;
        tx_buffer[2] = 0x00;
        tx_buffer[3] = 0x00;
        tx_buffer[4] = 0x00;
        transaction.count = 0x5;
        transaction.txBuf = tx_buffer;
        transaction.rxBuf = rx_buffer;
        encoder = Encoder2;
        transaction.arg = (void *) encoder;
        GPIO_write(SPI_Encoder2, 0);
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder2, 1);
    }
    else if (motor_Address == MOTOR3) {
        tx_buffer[0] = 0x60;
        tx_buffer[1] = 0x00;
        tx_buffer[2] = 0x00;
        tx_buffer[3] = 0x00;
        tx_buffer[4] = 0x00;
        transaction.count = 0x5;
        transaction.txBuf = tx_buffer;
        transaction.rxBuf = rx_buffer;
        encoder = Encoder3;
        transaction.arg = (void *) encoder;
        GPIO_write(SPI_Encoder3, 0);
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder3, 1);
    }
}

void choose_Register(motorAddress motor_Address) {

    if (motor_Address == MOTOR1) {
        register_tx_buff[0] = 0xE0;
        transaction.count = 0x1;
        transaction.txBuf = register_tx_buff;
        transaction.rxBuf = register_rx_buff;
        GPIO_write(SPI_Encoder1, 0);
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder1, 1);
    }

    if (motor_Address == MOTOR2) {
        register_tx_buff[0] = 0xE0;
        transaction.count = 0x1;
        transaction.txBuf = register_tx_buff;
        transaction.rxBuf = register_rx_buff;
        GPIO_write(SPI_Encoder2, 0);
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder2, 1);
    }

    if (motor_Address == MOTOR3) {
        register_tx_buff[0] = 0xE0;
        transaction.count = 0x1;
        transaction.txBuf = register_tx_buff;
        transaction.rxBuf = register_rx_buff;
        GPIO_write(SPI_Encoder3, 0);
        SPI_transfer(masterSpi, &transaction);
        usleep(2);
        GPIO_write(SPI_Encoder3, 1);
    }
}

void clear_encoder(motorAddress motor_Address)
{

     if (motor_Address == MOTOR1) {
         GPIO_write(SPI_Encoder1, 0);
         tx_buffer[0] = 0x98;
         tx_buffer[1] = 0x00;
         tx_buffer[2] = 0x00;
         tx_buffer[3] = 0x00;
         tx_buffer[4] = 0x00;
         transaction.count = 0x5;
         transaction.txBuf = tx_buffer;
         transaction.rxBuf = rx_buffer;
         SPI_transfer(masterSpi, &transaction);
         usleep(2);
         GPIO_write(SPI_Encoder1, 1);
     }

     if (motor_Address == MOTOR2) {
         GPIO_write(SPI_Encoder2, 0);
         tx_buffer[0] = 0x98;
         tx_buffer[1] = 0x00;
         tx_buffer[2] = 0x00;
         tx_buffer[3] = 0x00;
         tx_buffer[4] = 0x00;
         transaction.count = 0x5;
         transaction.txBuf = tx_buffer;
         transaction.rxBuf = rx_buffer;
         SPI_transfer(masterSpi, &transaction);
         usleep(2);
         GPIO_write(SPI_Encoder2, 1);
     }

     if (motor_Address == MOTOR3) {
         GPIO_write(SPI_Encoder3, 0);
         tx_buffer[0] = 0x98;
         tx_buffer[1] = 0x00;
         tx_buffer[2] = 0x00;
         tx_buffer[3] = 0x00;
         tx_buffer[4] = 0x00;
         transaction.count = 0x5;
         transaction.txBuf = tx_buffer;
         transaction.rxBuf = rx_buffer;
         SPI_transfer(masterSpi, &transaction);
         usleep(2);
         GPIO_write(SPI_Encoder3, 1);
     }
}
