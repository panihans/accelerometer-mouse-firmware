#include "move.h"
#include "imu.h"
#include "math.h"

typedef struct Accel {
	int16_t x;
	int16_t y;
	int16_t z;
} Accel;

Accel accel;

void get_mouse_xy(int8_t *x, int8_t *y) {
	get_xyz(&accel.x, &accel.y, &accel.z);
	if (accel.x < 0) {
		*x = ceil(accel.x / 500.0);
	} else {
		*x = floor(accel.x / 500.0);
	}
	if (accel.y < 0) {
		*y = -ceil(accel.y / 500.0);
	} else {
		*y = -floor(accel.y / 500.0);
	}
}
