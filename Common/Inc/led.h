#pragma once

#include "gpio.h"
#include "stm32wbxx_hal.h"

#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin , 1)
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin , 0)
#define TOGGLE_LED() HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)
