#include "imu.h"

void read_register(uint8_t r, uint8_t pRxData[]) {
	uint8_t pTxData[1] = { r | (1UL << 7) };
	CS_START();
	HAL_SPI_Transmit(&hspi1, pTxData, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, pRxData, 1, HAL_MAX_DELAY);
//	HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, 1, HAL_MAX_DELAY);
	CS_END();
}

void write_register(uint8_t r, uint8_t value) {
	uint8_t pTxData[2] = { r, value };
	uint8_t pRxData[2] = { 0, 0 };
	CS_START();
	HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, 2, HAL_MAX_DELAY);
	CS_END();
}

uint16_t read_x() {
	uint8_t pRxData[1] = { 0 };
	read_register(ACCEL_XOUT_H, pRxData);
	uint8_t pRxData2[1] = { 0 };
	read_register(ACCEL_XOUT_L, pRxData2);
	uint16_t x = (pRxData[0] << 8) | pRxData2[0];
	return x;
}

uint16_t read_y() {
	uint8_t pRxData[1] = { 0 };
	read_register(ACCEL_YOUT_H, pRxData);
	uint8_t pRxData2[1] = { 0 };
	read_register(ACCEL_YOUT_L, pRxData2);
	uint16_t y = (pRxData[0] << 8) | pRxData2[0];
	return y;
}

uint16_t read_z() {
	uint8_t pRxData[1] = { 0 };
	read_register(ACCEL_ZOUT_H, pRxData);
	uint8_t pRxData2[1] = { 0 };
	read_register(ACCEL_ZOUT_L, pRxData2);
	uint16_t z = (pRxData[0] << 8) | pRxData2[0];
	return z;
}
