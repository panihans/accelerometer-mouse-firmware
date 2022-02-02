#include "move.h"
#include "imu.h"
#include "math.h"

typedef struct Accel {
	int16_t x;
	int16_t y;
	int16_t z;
} Accel;

Accel accel;

#define ACCEL_DIV 50.0
#define SIG_ACCEL 25

uint8_t i = 0;
double avg_x = 0;
double avg_y = 0;

#define PREV_C 10
int16_t prev_x[PREV_C] = { 0 };
int16_t prev_y[PREV_C] = { 0 };

void get_mouse_xy(int8_t *x, int8_t *y) {
//	get_xyz(&accel.x, &accel.y, &accel.z);
//	if (accel.x < 0) {
//		*x = ceil(accel.x / 500.0);
//	} else {
//		*x = floor(accel.x / 500.0);
//	}
//	if (accel.y < 0) {
//		*y = -ceil(accel.y / 500.0);
//	} else {
//		*y = -floor(accel.y / 500.0);
//	}

	get_xyz(&accel.x, &accel.y, &accel.z);

	// exponential moving avg. to smooth peaks
	if (i == 0) {
		avg_x = accel.x;
		avg_y = accel.y;
		i++;
	} else {
		avg_x = 0.5 * accel.x + (1 - 0.5) * avg_x;
		avg_y = 0.5 * accel.y + (1 - 0.5) * avg_y;
	}

	for (int l = PREV_C - 2; l >= 0; l--) {
		prev_x[l + 1] = prev_x[l];
		prev_y[l + 1] = prev_y[l];
	}
	prev_x[0] = avg_x;
	prev_y[0] = avg_y;

	// movement direction trigger
	int x_reset = 1;
	int y_reset = 1;
	for (int k = 0; k < PREV_C; k++) {
		// reset if 5 small values in a row
		if (abs(prev_x[k]) > 5) {
			x_reset = 0;
		}
		if (abs(prev_y[k]) > 5) {
			y_reset = 0;
		}
	}
	if (x_reset) {
		*x = 0;
	}
	if (y_reset) {
		*y = 0;
	}

	// if x trigger is not set and accel is significant
	if (*x == 0) {
		if (avg_x < -SIG_ACCEL) {
			*x = -avg_x / ACCEL_DIV;
		} else if (avg_x > SIG_ACCEL) {
			*x = -avg_x / ACCEL_DIV;
		}
	}
	// if x accel is greater than starting accel
	if (*x > 0) {
		if (avg_x < -SIG_ACCEL) {
			if (fabs(avg_x / ACCEL_DIV) > fabs(*x)) {
				*x = -avg_x / ACCEL_DIV;
			}
		}
	}
	if (*x < 0) {
		if (avg_x > SIG_ACCEL) {
			if (fabs(avg_x / ACCEL_DIV) > fabs(*x)) {
				*x = -avg_x / ACCEL_DIV;
			}
		}
	}

	// if y trigger is not set and accel is significant
	if (*y == 0) {
		if (avg_y < -SIG_ACCEL) {
			*y = avg_y / ACCEL_DIV;
		} else if (avg_y > SIG_ACCEL) {
			*y = avg_y / ACCEL_DIV;
		}
	}
	// if y accel is greater than starting accel
	if (*y < 0 || *y > 0) {
		if (avg_y < -SIG_ACCEL) {
			if (fabs(avg_y / ACCEL_DIV) > fabs(*y)) {
				*y = avg_y / ACCEL_DIV;
			}
		}
	}
	if (*y > 0) {
		if (avg_y > SIG_ACCEL) {
			if (fabs(avg_y / ACCEL_DIV) > fabs(*y)) {
				*y = avg_y / ACCEL_DIV;
			}
		}
	}
}
