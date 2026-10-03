/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define FAULT_DISC_Pin GPIO_PIN_1
#define FAULT_DISC_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOA
#define WDI_Pin GPIO_PIN_1
#define WDI_GPIO_Port GPIOA
#define CS_SD_Pin GPIO_PIN_4
#define CS_SD_GPIO_Port GPIOA
#define CS_FLASH_Pin GPIO_PIN_0
#define CS_FLASH_GPIO_Port GPIOB
#define SD_DETECT_Pin GPIO_PIN_2
#define SD_DETECT_GPIO_Port GPIOB
#define CS_POT_Pin GPIO_PIN_10
#define CS_POT_GPIO_Port GPIOB
#define LED_ORANGE_Pin GPIO_PIN_7
#define LED_ORANGE_GPIO_Port GPIOC
#define FAULT_CURR_Pin GPIO_PIN_8
#define FAULT_CURR_GPIO_Port GPIOC
#define FAULT_COMM_Pin GPIO_PIN_9
#define FAULT_COMM_GPIO_Port GPIOC
#define RS485_DE_Pin GPIO_PIN_8
#define RS485_DE_GPIO_Port GPIOA
#define RELAY_MAIN_Pin GPIO_PIN_3
#define RELAY_MAIN_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_4
#define LED_GREEN_GPIO_Port GPIOB
#define LED_YELLOW_Pin GPIO_PIN_5
#define LED_YELLOW_GPIO_Port GPIOB
#define RELAY_FEEDBACK_Pin GPIO_PIN_6
#define RELAY_FEEDBACK_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
