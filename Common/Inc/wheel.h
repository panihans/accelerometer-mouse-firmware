#pragma once

#include "stm32wbxx_hal.h"

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define clamp(l, h, val) max(l, min(h, val))

#define ENCODER_MAX 65535
#define ENCODER_QUADRANT (ENCODER_MAX / 4)
#define ENCODER_QUADRANT_3 (ENCODER_QUADRANT * 3)

uint8_t get_wheel_change(uint32_t encoder_timer_count);
