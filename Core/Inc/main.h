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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LED_METAL_Pin GPIO_PIN_2
#define LED_METAL_GPIO_Port GPIOC
#define LED_PAPEL_Pin GPIO_PIN_3
#define LED_PAPEL_GPIO_Port GPIOC
#define SENSOR_IND_Pin GPIO_PIN_0
#define SENSOR_IND_GPIO_Port GPIOA
#define SENSOR_CAP_Pin GPIO_PIN_1
#define SENSOR_CAP_GPIO_Port GPIOA
#define SENSOR_PIR_Pin GPIO_PIN_2
#define SENSOR_PIR_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define SENSOR_LDR_Pin GPIO_PIN_4
#define SENSOR_LDR_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SENSOR_MIC_Pin GPIO_PIN_6
#define SENSOR_MIC_GPIO_Port GPIOA
#define EXTRA_1_Pin GPIO_PIN_7
#define EXTRA_1_GPIO_Port GPIOA
#define LED_PLASTICO_Pin GPIO_PIN_4
#define LED_PLASTICO_GPIO_Port GPIOC
#define LED_VIDRIO_Pin GPIO_PIN_5
#define LED_VIDRIO_GPIO_Port GPIOC
#define US_TRIG_MET_Pin GPIO_PIN_0
#define US_TRIG_MET_GPIO_Port GPIOB
#define US_ECHO_MET_Pin GPIO_PIN_1
#define US_ECHO_MET_GPIO_Port GPIOB
#define US_TRIG_PAP_Pin GPIO_PIN_10
#define US_TRIG_PAP_GPIO_Port GPIOB
#define SERVO_PLAS_Pin GPIO_PIN_12
#define SERVO_PLAS_GPIO_Port GPIOB
#define US_ECHO_PLAS_Pin GPIO_PIN_13
#define US_ECHO_PLAS_GPIO_Port GPIOB
#define US_TRIG_PLAS_Pin GPIO_PIN_14
#define US_TRIG_PLAS_GPIO_Port GPIOB
#define LED_ERROR_Pin GPIO_PIN_7
#define LED_ERROR_GPIO_Port GPIOC
#define LED_SYSTEM_Pin GPIO_PIN_8
#define LED_SYSTEM_GPIO_Port GPIOC
#define SERVO_PLATF_Pin GPIO_PIN_8
#define SERVO_PLATF_GPIO_Port GPIOA
#define SERVO_METL_Pin GPIO_PIN_9
#define SERVO_METL_GPIO_Port GPIOA
#define SERVO_PAPL_Pin GPIO_PIN_10
#define SERVO_PAPL_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SERVO_VIDR_Pin GPIO_PIN_10
#define SERVO_VIDR_GPIO_Port GPIOC
#define US_ECHO_PAP_Pin GPIO_PIN_11
#define US_ECHO_PAP_GPIO_Port GPIOB
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define US_TRIG_VIDR_Pin GPIO_PIN_4
#define US_TRIG_VIDR_GPIO_Port GPIOB
#define US_ECHO_VIDR_Pin GPIO_PIN_5
#define US_ECHO_VIDR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
