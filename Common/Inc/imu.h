#pragma once

#include "spi.h"

// register addresses
#define ACCEL_CONFIG 28
#define ACCEL_CONFIG_2 29
#define ACCEL_XOUT_H 59
#define ACCEL_XOUT_L 60
#define ACCEL_YOUT_H 61
#define ACCEL_YOUT_L 62
#define ACCEL_ZOUT_H 63
#define ACCEL_ZOUT_L 64
#define USER_CTRL 106
#define PWR_MGMT_1 107
#define PWR_MGMT_2 108
#define WHO_AM_I 117

// spi start end
#define CS_START() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin , 0)
#define CS_END() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin , 1)

// read register bit
#define IMU_READ (1 << 7)

uint8_t read_register(uint8_t r);
void write_register(uint8_t r, uint8_t pTxData);
void imu_setup();

int16_t read_x();
int16_t read_y();
int16_t read_z();
void get_xyz(int16_t *x, int16_t *y, int16_t *z);
extern int16_t y_r[];
