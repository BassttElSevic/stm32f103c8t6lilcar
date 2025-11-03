/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

  // 第一个L298N模块（前轮）
#define L298N1_IN1_PIN   GPIO_PIN_8
#define L298N1_IN1_PORT  GPIOA
#define L298N1_IN2_PIN   GPIO_PIN_9
#define L298N1_IN2_PORT  GPIOA
#define L298N1_IN3_PIN   GPIO_PIN_10
#define L298N1_IN3_PORT  GPIOA
#define L298N1_IN4_PIN   GPIO_PIN_11
#define L298N1_IN4_PORT  GPIOA

  // 第二个L298N模块（后轮）
#define L298N2_IN1_PIN   GPIO_PIN_8
#define L298N2_IN1_PORT  GPIOB
#define L298N2_IN2_PIN   GPIO_PIN_9
#define L298N2_IN2_PORT  GPIOB
#define L298N2_IN3_PIN   GPIO_PIN_10
#define L298N2_IN3_PORT  GPIOB
#define L298N2_IN4_PIN   GPIO_PIN_11
#define L298N2_IN4_PORT  GPIOB

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
