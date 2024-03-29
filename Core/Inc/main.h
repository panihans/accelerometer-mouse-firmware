/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_9
#define LED_GPIO_Port GPIOB
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define RIGHT_DOWN_Pin GPIO_PIN_0
#define RIGHT_DOWN_GPIO_Port GPIOB
#define RIGHT_UP_Pin GPIO_PIN_1
#define RIGHT_UP_GPIO_Port GPIOB
#define MIDDLE_DOWN_Pin GPIO_PIN_4
#define MIDDLE_DOWN_GPIO_Port GPIOE
#define MIDDLE_UP_Pin GPIO_PIN_10
#define MIDDLE_UP_GPIO_Port GPIOA
#define LEFT_UP_Pin GPIO_PIN_15
#define LEFT_UP_GPIO_Port GPIOA
#define LEFT_DOWN_Pin GPIO_PIN_3
#define LEFT_DOWN_GPIO_Port GPIOB
#define B4_DOWN_Pin GPIO_PIN_4
#define B4_DOWN_GPIO_Port GPIOB
#define B4_UP_Pin GPIO_PIN_5
#define B4_UP_GPIO_Port GPIOB
#define B5_DOWN_Pin GPIO_PIN_6
#define B5_DOWN_GPIO_Port GPIOB
#define B5_UP_Pin GPIO_PIN_7
#define B5_UP_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
