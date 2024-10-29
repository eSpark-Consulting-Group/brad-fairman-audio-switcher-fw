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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
    GPIO_TypeDef* port;  // Pointer to the GPIO port (e.g., GPIOA, GPIOB)
    uint16_t pin;        // GPIO pin number (e.g., GPIO_PIN_0, GPIO_PIN_1, etc.)
} GPIO_Pin_t;

typedef enum
{
    RELEASED,
    SHORT_PRESS,
    LONG_PRESS,
    EXTRA_LONG_PRESS,
}BUTTON_STATE_e;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define DEBOUNCE_TIME_MS	50
#define NUM_OF_OUTPUTS		5
#define STATUS_LED_TOGGLE_PERIOD	50			// toggle LED every 100ms
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MUTE_Pin GPIO_PIN_0
#define MUTE_GPIO_Port GPIOA
#define OUT1_LED_Pin GPIO_PIN_1
#define OUT1_LED_GPIO_Port GPIOA
#define OUT1_SW_Pin GPIO_PIN_2
#define OUT1_SW_GPIO_Port GPIOA
#define OUT2_LED_Pin GPIO_PIN_3
#define OUT2_LED_GPIO_Port GPIOA
#define OUT2_SW_Pin GPIO_PIN_4
#define OUT2_SW_GPIO_Port GPIOA
#define OUT3_LED_Pin GPIO_PIN_5
#define OUT3_LED_GPIO_Port GPIOA
#define OUT3_SW_Pin GPIO_PIN_6
#define OUT3_SW_GPIO_Port GPIOA
#define OUT4_LED_Pin GPIO_PIN_7
#define OUT4_LED_GPIO_Port GPIOA
#define OUT4_SW_Pin GPIO_PIN_0
#define OUT4_SW_GPIO_Port GPIOB
#define OUT5_LED_Pin GPIO_PIN_1
#define OUT5_LED_GPIO_Port GPIOB
#define OUT5_SW_Pin GPIO_PIN_2
#define OUT5_SW_GPIO_Port GPIOB
#define OUT6_LED_Pin GPIO_PIN_10
#define OUT6_LED_GPIO_Port GPIOB
#define OUT6_SW_Pin GPIO_PIN_11
#define OUT6_SW_GPIO_Port GPIOB
#define VOLUP_LED_Pin GPIO_PIN_12
#define VOLUP_LED_GPIO_Port GPIOB
#define FRONT_SEL_LED_Pin GPIO_PIN_15
#define FRONT_SEL_LED_GPIO_Port GPIOB
#define VOLUP_SW_Pin GPIO_PIN_8
#define VOLUP_SW_GPIO_Port GPIOA
#define VOLDWN_LED_Pin GPIO_PIN_9
#define VOLDWN_LED_GPIO_Port GPIOA
#define VOLDWN_SW_Pin GPIO_PIN_10
#define VOLDWN_SW_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_0
#define SPI2_CS_GPIO_Port GPIOD
#define REAR2_SEL_LED_Pin GPIO_PIN_3
#define REAR2_SEL_LED_GPIO_Port GPIOB
#define REAR_SEL_LED_Pin GPIO_PIN_4
#define REAR_SEL_LED_GPIO_Port GPIOB
#define CENTER_SEL_LED_Pin GPIO_PIN_5
#define CENTER_SEL_LED_GPIO_Port GPIOB
#define SUB_SEL_LED_Pin GPIO_PIN_8
#define SUB_SEL_LED_GPIO_Port GPIOB
#define STATUS_Pin GPIO_PIN_9
#define STATUS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
