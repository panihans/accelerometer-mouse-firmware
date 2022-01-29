#pragma once

#include "gpio.h"

extern uint8_t left_down;
extern uint8_t middle_down;
extern uint8_t right_down;
extern uint8_t b4_down;
extern uint8_t b5_down;

void update_buttons();
