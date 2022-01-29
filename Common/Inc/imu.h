#pragma once

#include "spi.h"

#define ACCEL_CONFIG 28
#define ACCEL_XOUT_H 59
#define ACCEL_XOUT_L 60
#define ACCEL_YOUT_H 61
#define ACCEL_YOUT_L 62
#define ACCEL_ZOUT_H 63
#define ACCEL_ZOUT_L 64
#define PWR_MGMT_1 107
#define PWR_MGMT_2 108
#define WHO_AM_I 117

#define CS_START() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin , 0)
#define CS_END() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin , 1)

void read_register(uint8_t r, uint8_t pRxData[]);
void write_register(uint8_t r, uint8_t pTxData);

uint16_t read_x();
uint16_t read_y();
uint16_t read_z();