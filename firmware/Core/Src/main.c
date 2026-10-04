/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "state_machine.h"
#include "feature_extraction.h"
#include "health_fusion.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Redirect printf to UART2 (debug output via ST-Link VCP) */
int __io_putchar(int ch) {
    extern UART_HandleTypeDef huart2;
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 100);
    return ch;
}

/* Non-blocking blink counter */
static uint32_t blink_counter = 0;

/* Timer callback — called by HAL on timer events */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        /* 10 Hz — sampling tick */
        blink_counter++;
    }
    if (htim->Instance == TIM7) {
        /* 1 Hz — inference tick */
    }
    if (htim->Instance == TIM14) {
        /* 20 Hz — watchdog feed */
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);  // WDI toggle
    }
}

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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  printf("\r\n\r\n");
printf("====================================\r\n");
printf("  SwasthEdge Boot OK\r\n");
printf("====================================\r\n");

/* Print clock configuration */
printf("SYSCLK: %lu Hz\r\n", HAL_RCC_GetSysClockFreq());
printf("HCLK:   %lu Hz\r\n", HAL_RCC_GetHCLKFreq());
printf("PCLK1:  %lu Hz\r\n", HAL_RCC_GetPCLK1Freq());
printf("PCLK2:  %lu Hz\r\n", HAL_RCC_GetPCLK2Freq());
printf("\r\n");

/* Start timers */
HAL_TIM_Base_Start_IT(&htim6);
HAL_TIM_Base_Start_IT(&htim7);
HAL_TIM_Base_Start_IT(&htim14);
printf("Timers started: TIM6 (10Hz), TIM7 (1Hz), TIM14 (20Hz)\r\n");
printf("\r\n");

/* Test feature extraction with synthetic data */
printf("=== FEATURE EXTRACTION TEST ===\r\n");
float test_buffer[100];
for (int i = 0; i < 100; i++) {
    test_buffer[i] = 25.0f + 0.5f * sinf(i * 0.1f) + 0.05f * ((i * 7) % 10 - 5);
}

float features[8];
extract_features(test_buffer, 100, features);

printf("  Mean:     %.3f\r\n", features[FEAT_MEAN]);
printf("  Variance: %.5f\r\n", features[FEAT_VARIANCE]);
printf("  Slope:    %.5f\r\n", features[FEAT_SLOPE]);
printf("  RMS:      %.3f\r\n", features[FEAT_RMS]);
printf("  EWMA:     %.3f\r\n", features[FEAT_EWMA]);
printf("  CUSUM:    %.5f\r\n", features[FEAT_CUSUM]);
printf("  Rate:     %.5f\r\n", features[FEAT_RATE]);
printf("  Range:    %.3f\r\n", features[FEAT_RANGE]);
printf("\r\n");

/* Test state machine */
printf("=== STATE MACHINE TEST ===\r\n");
state_machine_init();
for (float conf = 100.0f; conf >= 0.0f; conf -= 10.0f) {
    /* Feed 3 times to satisfy consecutive readings requirement */
    state_machine_update(conf);
    state_machine_update(conf);
    state_machine_update(conf);
    printf("  Confidence %.0f%% -> %s\r\n", conf, state_machine_get_name());
}
printf("\r\n");

/* Test health fusion */
printf("=== HEALTH FUSION TEST ===\r\n");
float fused1 = health_fusion(100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
printf("  All healthy:             %.1f%%\r\n", fused1);

float fused2 = health_fusion(50.0f, 100.0f, 100.0f, 100.0f, 100.0f);
printf("  Sensor degraded to 50:   %.1f%%\r\n", fused2);

float fused3 = health_fusion(50.0f, 50.0f, 100.0f, 100.0f, 100.0f);
printf("  Sensor + Electrical:     %.1f%%\r\n", fused3);

float fused4 = health_fusion(25.0f, 25.0f, 50.0f, 50.0f, 25.0f);
printf("  Multiple failures:       %.1f%%\r\n", fused4);
printf("\r\n");

/* Init state machine for main loop */
state_machine_init();
printf("Setup complete. Entering main loop...\r\n");
printf("\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Blink user LED (PA5) every 500ms */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
