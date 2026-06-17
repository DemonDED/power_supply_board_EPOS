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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
  uint8_t status_encoder_1;
  uint8_t status_encoder_2;
  uint8_t status_press_encder_1;
  uint8_t status_press_encder_2;
  uint8_t status_button_1;
  uint8_t status_button_2;
  uint8_t status_button_3;
} FlagsStatus;

typedef enum {
  MODE_SET = 0,
  MODE_GET = 1
} ModsIndicator;

typedef enum {
  AMPERE = 0,
  VOLTAGE = 1,
} ParamSelect;
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
#define ENC_A1_Pin GPIO_PIN_3
#define ENC_A1_GPIO_Port GPIOE
#define ENC_A1_EXTI_IRQn EXTI3_IRQn
#define ENC_B1_Pin GPIO_PIN_4
#define ENC_B1_GPIO_Port GPIOE
#define ENC_A2_Pin GPIO_PIN_5
#define ENC_A2_GPIO_Port GPIOE
#define ENC_A2_EXTI_IRQn EXTI9_5_IRQn
#define ENC_B2_Pin GPIO_PIN_6
#define ENC_B2_GPIO_Port GPIOE
#define ENCODER_S2_BTN_Pin GPIO_PIN_13
#define ENCODER_S2_BTN_GPIO_Port GPIOC
#define ENCODER_S2_BTN_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_S1_BTN_Pin GPIO_PIN_14
#define ENCODER_S1_BTN_GPIO_Port GPIOC
#define ENCODER_S1_BTN_EXTI_IRQn EXTI15_10_IRQn
#define SPI3_CS_1_Pin GPIO_PIN_15
#define SPI3_CS_1_GPIO_Port GPIOA
#define SPI3_CS_2_Pin GPIO_PIN_11
#define SPI3_CS_2_GPIO_Port GPIOC
#define G_O_Led1_green_Pin GPIO_PIN_3
#define G_O_Led1_green_GPIO_Port GPIOB
#define G_O_Led1_red_Pin GPIO_PIN_4
#define G_O_Led1_red_GPIO_Port GPIOB
#define G_O_Led2_green_Pin GPIO_PIN_5
#define G_O_Led2_green_GPIO_Port GPIOB
#define G_O_Led2_red_Pin GPIO_PIN_6
#define G_O_Led2_red_GPIO_Port GPIOB
#define G_O_Led3_green_Pin GPIO_PIN_7
#define G_O_Led3_green_GPIO_Port GPIOB
#define G_O_Led3_red_Pin GPIO_PIN_8
#define G_O_Led3_red_GPIO_Port GPIOB
#define BTN_S1_PE0_Pin GPIO_PIN_0
#define BTN_S1_PE0_GPIO_Port GPIOE
#define BTN_S1_PE0_EXTI_IRQn EXTI0_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
