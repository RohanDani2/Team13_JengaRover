/*
 * spi_encoder_comm.h
 *
 *  Created on: Apr 27, 2020
 *      Author: Rohan J. Dani
 */

#ifndef SPI_ENCODER_COMM_H_
#define SPI_ENCODER_COMM_H_

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include <uart_term.h>
#include <encoder_queue.h>

SPI_Handle      masterSpi;
SPI_Params      spiParams;
SPI_Transaction transaction;

static char buffer[30];

/* Motor Addresses */
typedef enum
{
    Encoder1,
    Encoder2,
    Encoder3
} encoderSelected;

/* Motor Addresses */
typedef enum
{
    MOTOR1 = 128,
    MOTOR2 = 129,
    MOTOR3 = 130
} motorAddress;

void initSPIParams();
void read_encoder(motorAddress motor_Address);
void clear_encoder(motorAddress motor_Address);
void initEncoder(motorAddress motor_Address);
void encoderCallback(SPI_Handle handle, SPI_Transaction *transaction);
void startEncoderTransmission();
void clear_encoderVal(motorAddress motor_Address);
void choose_Register(motorAddress motor_Address);
void chooseEncoder(int encoder_val, char encoderChosen);


#endif /* SPI_ENCODER_COMM_H_ */
