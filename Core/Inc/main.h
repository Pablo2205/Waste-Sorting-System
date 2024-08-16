/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define Ligh_Sensor_Pin GPIO_PIN_0
#define Ligh_Sensor_GPIO_Port GPIOC
#define Sound_Sensor_Pin GPIO_PIN_1
#define Sound_Sensor_GPIO_Port GPIOC
#define UltraSound_Sensor_Pin GPIO_PIN_2
#define UltraSound_Sensor_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define SMPS_EN_Pin GPIO_PIN_4
#define SMPS_EN_GPIO_Port GPIOA
#define SMPS_V1_Pin GPIO_PIN_5
#define SMPS_V1_GPIO_Port GPIOA
#define SMPS_PG_Pin GPIO_PIN_6
#define SMPS_PG_GPIO_Port GPIOA
#define SMPS_SW_Pin GPIO_PIN_7
#define SMPS_SW_GPIO_Port GPIOA
#define Inductive_Sensor_Pin GPIO_PIN_0
#define Inductive_Sensor_GPIO_Port GPIOB
#define Inductive_Sensor_EXTI_IRQn EXTI0_IRQn
#define Capacitive_Sensor_Pin GPIO_PIN_1
#define Capacitive_Sensor_GPIO_Port GPIOB
#define Capacitive_Sensor_EXTI_IRQn EXTI1_IRQn
#define InfraRed_Sensor_Pin GPIO_PIN_2
#define InfraRed_Sensor_GPIO_Port GPIOB
#define InfraRed_Sensor_EXTI_IRQn EXTI2_IRQn
#define LD4_Pin GPIO_PIN_13
#define LD4_GPIO_Port GPIOB
#define LED_OK_Pin GPIO_PIN_14
#define LED_OK_GPIO_Port GPIOB
#define LED_Processing_Pin GPIO_PIN_15
#define LED_Processing_GPIO_Port GPIOB
#define Docker_Glass_Pin GPIO_PIN_6
#define Docker_Glass_GPIO_Port GPIOC
#define Docker_Plastic_Pin GPIO_PIN_7
#define Docker_Plastic_GPIO_Port GPIOC
#define Docker_Metal_Pin GPIO_PIN_8
#define Docker_Metal_GPIO_Port GPIOC
#define ServoMotor_Glass_Pin GPIO_PIN_8
#define ServoMotor_Glass_GPIO_Port GPIOA
#define ServoMotor_Plastic_Pin GPIO_PIN_9
#define ServoMotor_Plastic_GPIO_Port GPIOA
#define ServoMotor_Metal_Pin GPIO_PIN_10
#define ServoMotor_Metal_GPIO_Port GPIOA
#define ServoMotor_Base_Pin GPIO_PIN_11
#define ServoMotor_Base_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
