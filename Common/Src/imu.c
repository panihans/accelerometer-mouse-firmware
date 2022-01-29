#include "imu.h"
#include "math.h"

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

#pragma GCC push_options
#pragma GCC optimize ("O0")
void imu_setup() {
	// reset IMU
	while (1) {
		write_register(PWR_MGMT_1, (1UL << 7));
		HAL_Delay(100);
		uint8_t reg = read_register(PWR_MGMT_1);
		if (reg & (1UL << 6)) {
			break;
		}
	}

	// wake up
	while (1) {
		write_register(PWR_MGMT_1, (1 << 0));
		HAL_Delay(100);
		uint8_t reg = read_register(PWR_MGMT_1);
		if (reg & (1 << 0)) {
			break;
		}
	}

	//disable i2c
	while (1) {
		write_register(USER_CTRL, (1 << 4));
		HAL_Delay(100);
		uint8_t reg = read_register(USER_CTRL);
		if (reg & (1 << 4)) {
			break;
		}
	}

	//accel +-16g
	while (1) {
		write_register(ACCEL_CONFIG, (1 << 4) | (1 << 3));
		HAL_Delay(100);
		uint8_t reg = read_register(ACCEL_CONFIG);
		if (reg & (1 << 4) && reg & (1 << 3)) {
			break;
		}
	}

	//accel low pass filter setting
	while (1) {
		write_register(ACCEL_CONFIG_2, 0);
		HAL_Delay(100);
		uint8_t reg = read_register(ACCEL_CONFIG_2);
		if (reg == 0) {
			break;
		}
	}
}
#pragma GCC pop_options

int16_t read_x() {
	uint8_t h = read_register(ACCEL_XOUT_H);
	uint8_t l = read_register(ACCEL_XOUT_L);
	int16_t x = (h << 8) | l;
	return x;
}

int16_t read_y() {
	uint8_t h = read_register(ACCEL_YOUT_H);
	uint8_t l = read_register(ACCEL_YOUT_L);
	int16_t y = (h << 8) | l;
	return y;
}

int16_t read_z() {
	uint8_t h = read_register(ACCEL_ZOUT_H);
	uint8_t l = read_register(ACCEL_ZOUT_L);
	int16_t z = (h << 8) | l;
	return z;
}


// 10-200hz, order 2
#define FILTER_N 2
float num[] = { 0.1122, 0, -0.1122 };
float den[] = { -1, 1.7581, -0.7757 };
// 15-100hz, order 4
//#define FILTER_N 4
//float num[] = { 0.0512, 0, -0.1024, 0, 0.0512 };
//float den[] = { -1, 3.1673, -3.8396, 2.1405, -0.4706 };

void filter_p(int16_t r[], int16_t f[], int16_t new) {
	if (new < -10000 || new > 10000) {
		return;
	}
	r[0] = new;
	f[0] = 0;
	float sum = 0;
	for (int i = 0; i < FILTER_N + 1; i++) {
		sum += den[i] * f[i];
		sum += num[i] * r[i];
	}
	f[0] = sum;
//	f[0] = den[1] * f[1] + den[2] * f[2] + num[0] * r[0] + num[1] * r[1] + num[2] * r[2];

	for (int i = FILTER_N - 1; i >= 0; i--) {
		r[i + 1] = r[i];
		f[i + 1] = f[i];
	}
}

int16_t x_r[FILTER_N + 1] = { 0 };
int16_t x_f[FILTER_N + 1] = { 0 };
int16_t y_r[FILTER_N + 1] = { 0 };
int16_t y_f[FILTER_N + 1] = { 0 };
int16_t z_r[FILTER_N + 1] = { 0 };
int16_t z_f[FILTER_N + 1] = { 0 };

void get_xyz(int16_t *x, int16_t *y, int16_t *z) {
	filter_p(x_r, x_f, read_x());
	filter_p(y_r, y_f, read_y());
	filter_p(z_r, z_f, read_z());
	*x = x_f[0];
	*y = y_f[0];
	*z = z_f[0];
}
