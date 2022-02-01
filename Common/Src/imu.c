#include "imu.h"
#include "math.h"

uint8_t read_register(uint8_t r) {
	// reads a single value from a register
	// r - register
	uint8_t pTxData[1] = { r | (1UL << 7) };
	uint8_t pRxData[1] = { 0 };
	CS_START();
	HAL_SPI_Transmit(&hspi1, pTxData, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, pRxData, 1, HAL_MAX_DELAY);
	CS_END();
	return pRxData[0];
}

void write_register(uint8_t r, uint8_t value) {
	// writes the value to the register
	// r - register
	// value - value to write
	uint8_t pTxData[2] = { r, value };
	uint8_t pRxData[2] = { 0, 0 };
	CS_START();
	HAL_SPI_Transmit(&hspi1, pTxData, 2, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, pRxData, 2, HAL_MAX_DELAY);
	CS_END();
}

#define ACCEL_CALIBRATION_N 100
int16_t x_offset, y_offset, z_offset = 0;
void imu_setup() {
	// IMU setup operations

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
		write_register(ACCEL_CONFIG_2, 1);
		HAL_Delay(100);
		uint8_t reg = read_register(ACCEL_CONFIG_2);
		if (reg == 1) {
			break;
		}
	}

	//accel offset calculation
	int16_t x = 0;
	int16_t y = 0;
	int16_t z = 0;
	int32_t xsum = 0;
	int32_t ysum = 0;
	int32_t zsum = 0;
	for (int i = 0; i < ACCEL_CALIBRATION_N; ++i) {
		get_xyz(&x, &y, &z);
		xsum += x;
		ysum += y;
		zsum += z;
		HAL_Delay(1);
	}
	x_offset = xsum / ACCEL_CALIBRATION_N;
	y_offset = ysum / ACCEL_CALIBRATION_N;
	z_offset = zsum / ACCEL_CALIBRATION_N;
}

int16_t read_x() {
	// reads and returns the accel x axis
	uint8_t h = read_register(ACCEL_XOUT_H);
	uint8_t l = read_register(ACCEL_XOUT_L);
	int16_t x = (h << 8) | l;
	return x;
}

int16_t read_y() {
	// reads and returns the accel y axis
	uint8_t h = read_register(ACCEL_YOUT_H);
	uint8_t l = read_register(ACCEL_YOUT_L);
	int16_t y = (h << 8) | l;
	return y;
}

int16_t read_z() {
	// reads and returns the accel z axis
	uint8_t h = read_register(ACCEL_ZOUT_H);
	uint8_t l = read_register(ACCEL_ZOUT_L);
	int16_t z = (h << 8) | l;
	return z;
}

// filters
// 10-200hz, order 2
#define FILTER_N 2
float num[] = { 0.1122, 0, -0.1122 };
float den[] = { -1, 1.7581, -0.7757 };
// 15-100hz, order 4
//#define FILTER_N 4
//float num[] = { 0.0512, 0, -0.1024, 0, 0.0512 };
//float den[] = { -1, 3.1673, -3.8396, 2.1405, -0.4706 };
//#define FILTER_N 3
//float num[] = { 0.6156, -1.8468, 1.8468, -0.6156 };
//float den[] = { -1, 2.0478, -1.5057, 0.3713 };

void filter_p(int16_t r[], int16_t f[], int16_t new) {
	// applies an iir filter
	// r - raw inputs array
	// f - filtered values array
	// new - the new value to add
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
	// returns current xyz accelerometer values
	// x - accel x return value ptr
	// y - accel y return value ptr
	// z - accel z return value ptr
	filter_p(x_r, x_f, read_x());
	filter_p(y_r, y_f, read_y());
	filter_p(z_r, z_f, read_z());
	*x = x_f[0];
	*y = y_f[0];
	*z = z_f[0];
//	*x = read_x() - x_offset;
//	*y = read_y() - y_offset;
//	*z = read_z() - z_offset;
}
