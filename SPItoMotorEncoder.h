/*
 * SPItoMotorEncoder.h
 *
 *  Created on: Feb 26, 2020
 *      Author: Rohan J. Dani
 */

#ifndef SPITOMOTORENCODER_H_
#define SPITOMOTORENCODER_H_

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include "UartToMotorController.h"

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

void read_encoder(motorAddress motor_Address);
void clear_encoder(motorAddress motor_Address);
void initEncoder(motorAddress motor_Address);
void encoderCallback(SPI_Handle handle, SPI_Transaction *transaction);
void startEncoderTransmission();
void clear_encoderVal(motorAddress motor_Address);
void choose_Register(motorAddress motor_Address);
void chooseEncoder(int encoder_val, char encoderChosen);


#endif /* SPITOMOTORENCODER_H_ */
