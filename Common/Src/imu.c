#include "imu.h"

uint8_t read_register(uint8_t r) {
	uint8_t pTxData[1] = { r | (1UL << 7) };
	uint8_t pRxData[1] = { 0 };
	CS_START();
	HAL_SPI_Transmit(&hspi1, pTxData, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, pRxData, 1, HAL_MAX_DELAY);
	CS_END();
	return pRxData[0];
}

void write_register(uint8_t r, uint8_t value) {
	uint8_t pTxData[2] = { r, value };
	CS_START();
	HAL_SPI_Transmit(&hspi1, pTxData, 2, HAL_MAX_DELAY);
	CS_END();
}

uint16_t read_x() {
	uint8_t h = read_register(ACCEL_XOUT_H);
	uint8_t l = read_register(ACCEL_XOUT_L);
	int16_t x = (h << 8) | l;
	return x;
}

uint16_t read_y() {
	uint8_t h = read_register(ACCEL_YOUT_H);
	uint8_t l = read_register(ACCEL_YOUT_L);
	int16_t y = (h << 8) | l;
	return y;
}

uint16_t read_z() {
	uint8_t h = read_register(ACCEL_ZOUT_H);
	uint8_t l = read_register(ACCEL_ZOUT_L);
	int16_t z = (h << 8) | l;
	return z;
}
