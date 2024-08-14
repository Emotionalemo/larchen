/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define control_1_Pin GPIO_PIN_6
#define control_1_GPIO_Port GPIOE
#define input_4_Pin GPIO_PIN_6
#define input_4_GPIO_Port GPIOF
#define input_3_Pin GPIO_PIN_8
#define input_3_GPIO_Port GPIOF
#define control_2_Pin GPIO_PIN_1
#define control_2_GPIO_Port GPIOC
#define input_1_Pin GPIO_PIN_0
#define input_1_GPIO_Port GPIOB
#define input_2_Pin GPIO_PIN_2
#define input_2_GPIO_Port GPIOB
#define input_7_Pin GPIO_PIN_11
#define input_7_GPIO_Port GPIOE
#define input_8_Pin GPIO_PIN_12
#define input_8_GPIO_Port GPIOE
#define stop_2_Pin GPIO_PIN_10
#define stop_2_GPIO_Port GPIOB
#define input_5_Pin GPIO_PIN_11
#define input_5_GPIO_Port GPIOD
#define input_6_Pin GPIO_PIN_13
#define input_6_GPIO_Port GPIOD
#define stop_1_Pin GPIO_PIN_3
#define stop_1_GPIO_Port GPIOD
#define stop_4_Pin GPIO_PIN_5
#define stop_4_GPIO_Port GPIOD
#define control_4_Pin GPIO_PIN_10
#define control_4_GPIO_Port GPIOG
#define stop_3_Pin GPIO_PIN_3
#define stop_3_GPIO_Port GPIOB
#define control_3_Pin GPIO_PIN_0
#define control_3_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
