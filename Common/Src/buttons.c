#include "buttons.h"

uint8_t left_down = 0;
uint8_t middle_down = 0;
uint8_t right_down = 0;
uint8_t b4_down = 0;
uint8_t b5_down = 0;

void update_buttons() {
 	if (HAL_GPIO_ReadPin(LEFT_UP_GPIO_Port, LEFT_UP_Pin)) {
		left_down = 0;
	}
	if (HAL_GPIO_ReadPin(LEFT_DOWN_GPIO_Port, LEFT_DOWN_Pin)) {
		left_down = 1;
	}

	if (HAL_GPIO_ReadPin(MIDDLE_UP_GPIO_Port, MIDDLE_UP_Pin)) {
		middle_down = 0;
	}
	if (HAL_GPIO_ReadPin(MIDDLE_DOWN_GPIO_Port, MIDDLE_DOWN_Pin)) {
		middle_down = 1;
	}

	if (HAL_GPIO_ReadPin(RIGHT_UP_GPIO_Port, RIGHT_UP_Pin)) {
		right_down = 0;
	}
	if (HAL_GPIO_ReadPin(RIGHT_DOWN_GPIO_Port, RIGHT_DOWN_Pin)) {
		right_down = 1;
	}

	if (HAL_GPIO_ReadPin(B4_UP_GPIO_Port, B4_UP_Pin)) {
		b4_down = 0;
	}
	if (HAL_GPIO_ReadPin(B4_DOWN_GPIO_Port, B4_DOWN_Pin)) {
		b4_down = 1;
	}

	if (HAL_GPIO_ReadPin(B5_UP_GPIO_Port, B5_UP_Pin)) {
		b5_down = 0;
	}
	if (HAL_GPIO_ReadPin(B5_DOWN_GPIO_Port, B5_DOWN_Pin)) {
		b5_down = 1;
	}
}
