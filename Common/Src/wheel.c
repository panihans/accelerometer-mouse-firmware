#include "wheel.h"

int8_t calculate_encoder_diff(uint32_t prev_pos, uint32_t cur_pos) {
	// calculates encoder diff
	// prev_pos - previous position of the encoder
	// cur_pos - current position of the encoder
	int diff = 0;
	if (prev_pos > ENCODER_QUADRANT_3 && cur_pos < ENCODER_QUADRANT) {
		// encoder counter overflow from high to low
		diff = 65535 - prev_pos + cur_pos;
	} else if (prev_pos < ENCODER_QUADRANT && cur_pos > ENCODER_QUADRANT_3) {
		// encoder counter overflow from low to high
		diff = -65535 + cur_pos - prev_pos;
	} else {
		// encoder didn't overflow
		diff = cur_pos - prev_pos;
	}
	return (int8_t) clamp(INT8_MIN, INT8_MAX, diff);
}

uint32_t encoder_prev = 0;
int8_t get_wheel_change(uint32_t encoder_timer_count) {
	// returns wheel change -1, 0 or 1 when compared to the last wheel position
	// encoder_timer_count - current wheel encoder timer count
	uint32_t encoder_cur = encoder_timer_count;
	int8_t diff = calculate_encoder_diff(encoder_prev, encoder_cur);
	encoder_prev = encoder_cur;
	if (diff < 0) {
		return -1;
	}
	if (diff > 0) {
		return 1;
	}
	return 0;
}

#define WHEEL_HOLD 10
int wheel_t = 0;
void update_wheel(int8_t *wheel, uint32_t encoder_timer_count) {
	// updates wheel
	// wheel - wheel value pointer
	// encoder_timer_count - current wheel encoder timer count
	int new_wheel = get_wheel_change(TIM1->CNT);
	if (*wheel == 0 || wheel_t > WHEEL_HOLD) {
		*wheel = new_wheel;
		wheel_t = 0;
	} else {
		wheel_t++;
	}
}
