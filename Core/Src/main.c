/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "imu.h"
#include "buttons.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct Command {
	int16_t x;
	int16_t y;
	int16_t z;
	uint8_t left;
	uint8_t middle;
	uint8_t right;
	uint8_t b4;
	uint8_t b5;
} Command;

Command feedback;


#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define clamp(l, h, val) max(l, min(h, val))

#define ENCODER_MAX 65535
#define ENCODER_QUADRANT (ENCODER_MAX / 4)
#define ENCODER_QUADRANT_3 (ENCODER_QUADRANT * 3)

int8_t calculate_encoder_diff(uint32_t prev_pos, uint32_t cur_pos) {
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
	return (int8_t)clamp(INT8_MIN, INT8_MAX, diff);
}

typedef struct Mouse_Report {
	uint8_t buttons;
	int8_t x;
	int8_t y;
	int8_t wheel;
	uint8_t motion_wakeup;
} Mouse_Report;

Mouse_Report report;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_Device_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1 | TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	imu_setup();

	uint32_t encoder_prev = 0;
	while (1) {
		update_buttons();
		feedback.left = left_down;
		feedback.middle = middle_down;
		feedback.right = right_down;
		feedback.b4 = b4_down;
		feedback.b5 = b5_down;
		get_xyz(&feedback.x, &feedback.y, &feedback.z);
//		report.x = feedback.x;
//		report.y = feedback.y;

		uint32_t encoder_cur = TIM1->CNT;
		report.wheel = calculate_encoder_diff(encoder_prev, encoder_cur); // needs a larger change to move at all
		encoder_prev = encoder_cur;
//		uint8_t buttons = 0;
//		uint8_t buttons = (2 << left_down) | (1 << middle_down) | (0 << right_down);// | (3 << feedback.b4) | (4 << feedback.b5);

		report.buttons = (left_down << 0) | (right_down << 1) | (middle_down << 2) | (b5_down << 3) | (b4_down << 4);

		USBD_HID_SendReport(&hUsbDeviceFS, &report, 5);
//		TOGGLE_LED();
//		HAL_Delay(500);
//		TOGGLE_LED();
//		HAL_Delay(500);

//
//		int16_t x = 0;
//		int16_t y = 0;
//		int16_t z = 0;

//		get_xyz(&feedback.x, &feedback.y, &feedback.z);
//		CDC_Transmit_FS(&feedback, sizeof(feedback));
		HAL_Delay(1);

//		int16_t x = read_x();
//		int16_t y = read_y();
//		int16_t z = read_z();

//		read_xyz(&x, &y, &z);q

//		feedback.x = x;
//		feedback.y = y;
//		feedback.z = z;
//		CDC_Transmit_FS(&feedback, sizeof(feedback));
//		HAL_Delay(1);
//		uint8_t pTxData2[2] = { 0b01101011, 0b0 };
//		uint8_t pRxData2[2] = { 0, 0 };
//		CS_START();
//		HAL_SPI_TransmitReceive(&hspi1, pTxData2, pRxData2, 2, HAL_MAX_DELAY);
//		CS_END();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

