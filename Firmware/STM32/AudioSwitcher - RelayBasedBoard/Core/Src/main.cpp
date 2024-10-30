/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "button.hpp"
#include "outputControl.hpp"
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
Button	volUP_SW({VOLUP_SW_GPIO_Port, VOLUP_SW_Pin}, 	{VOLUP_LED_GPIO_Port, VOLUP_LED_Pin});
Button	volDWN_SW({VOLDWN_SW_GPIO_Port, VOLDWN_SW_Pin}, {VOLDWN_LED_GPIO_Port, VOLDWN_LED_Pin});
Button* outputSW[NUM_OF_OUTPUTS];

Output* outputs[NUM_OF_OUTPUTS];

OutputControl* outputControl[NUM_OF_OUTPUTS];

uint8_t	outputStateFlag = 0;
uint64_t timeStamp = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */
  outputSW[0] = new Button({OUT1_SW_GPIO_Port, OUT1_SW_Pin}, {OUT1_LED_GPIO_Port, OUT1_LED_Pin});		// Towers
  outputSW[1] = new Button({OUT2_SW_GPIO_Port, OUT2_SW_Pin}, {OUT2_LED_GPIO_Port, OUT2_LED_Pin});		// BookShelfs
  outputSW[2] = new Button({OUT3_SW_GPIO_Port, OUT3_SW_Pin}, {OUT3_LED_GPIO_Port, OUT3_LED_Pin});		// Center Channel Bookshelves and heights
  outputSW[3] = new Button({OUT4_SW_GPIO_Port, OUT4_SW_Pin}, {OUT4_LED_GPIO_Port, OUT4_LED_Pin});		// Add Subwoofer
  outputSW[4] = new Button({OUT5_SW_GPIO_Port, OUT5_SW_Pin}, {OUT5_LED_GPIO_Port, OUT5_LED_Pin});		// Everything

  outputs[0] = new Output({SUB_RELAY_GPIO_Port, 	SUB_RELAY_Pin}, 	{SUB_SEL_LED_GPIO_Port, 	SUB_SEL_LED_Pin});
  outputs[1] = new Output({CENTER_RELAY_GPIO_Port, 	CENTER_RELAY_Pin}, 	{CENTER_SEL_LED_GPIO_Port, 	CENTER_SEL_LED_Pin});
  outputs[2] = new Output({REAR2_RELAY_GPIO_Port, 	REAR2_RELAY_Pin}, 	{REAR2_SEL_LED_GPIO_Port, 	REAR2_SEL_LED_Pin});
  outputs[3] = new Output({REAR_RELAY_GPIO_Port, 	REAR_RELAY_Pin}, 	{REAR_SEL_LED_GPIO_Port, 	REAR_SEL_LED_Pin});
  outputs[4] = new Output({FRONT_RELAY_GPIO_Port, 	FRONT_RELAY_Pin}, 	{FRONT_SEL_LED_GPIO_Port, 	FRONT_SEL_LED_Pin});

  for(int i=0; i<NUM_OF_OUTPUTS; i++)
  {
	  outputControl[i] = new OutputControl(outputSW[i], outputs[i]);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  volUP_SW.update();
	  volDWN_SW.update();

	  for(int i=0; i<NUM_OF_OUTPUTS; i++)
	  {
		  outputControl[i]->process();
		  if(outputControl[i]->getState())
		  {
			  outputStateFlag |= (1<<i);
		  }
		  else
		  {
			  outputStateFlag &= ~(1<<i);
		  }
	  }

	  if(outputStateFlag)
	  {
		  if((HAL_GetTick() - timeStamp) > STATUS_LED_TOGGLE_PERIOD)
		  {
			  timeStamp = HAL_GetTick();
			  HAL_GPIO_TogglePin(STATUS_GPIO_Port, STATUS_Pin);
		  }
	  }
	  else
	  {
		  HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin, GPIO_PIN_SET);
	  }
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OUT1_LED_Pin|OUT2_LED_Pin|OUT3_LED_Pin|OUT4_LED_Pin
                          |VOLDWN_LED_Pin|FRONT_RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OUT5_LED_Pin|OUT6_LED_Pin|VOLUP_LED_Pin|SUB_RELAY_Pin
                          |CENTER_RELAY_Pin|FRONT_SEL_LED_Pin|REAR2_SEL_LED_Pin|REAR_SEL_LED_Pin
                          |CENTER_SEL_LED_Pin|REAR2_RELAY_Pin|REAR_RELAY_Pin|SUB_SEL_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_OUT1_Pin|GPIO_OUT_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : OUT1_LED_Pin OUT2_LED_Pin OUT3_LED_Pin OUT4_LED_Pin
                           VOLDWN_LED_Pin FRONT_RELAY_Pin */
  GPIO_InitStruct.Pin = OUT1_LED_Pin|OUT2_LED_Pin|OUT3_LED_Pin|OUT4_LED_Pin
                          |VOLDWN_LED_Pin|FRONT_RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT1_SW_Pin OUT2_SW_Pin OUT3_SW_Pin VOLUP_SW_Pin
                           VOLDWN_SW_Pin */
  GPIO_InitStruct.Pin = OUT1_SW_Pin|OUT2_SW_Pin|OUT3_SW_Pin|VOLUP_SW_Pin
                          |VOLDWN_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT4_SW_Pin OUT5_SW_Pin OUT6_SW_Pin */
  GPIO_InitStruct.Pin = OUT4_SW_Pin|OUT5_SW_Pin|OUT6_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT5_LED_Pin OUT6_LED_Pin VOLUP_LED_Pin SUB_RELAY_Pin
                           CENTER_RELAY_Pin FRONT_SEL_LED_Pin REAR2_SEL_LED_Pin REAR_SEL_LED_Pin
                           CENTER_SEL_LED_Pin REAR2_RELAY_Pin REAR_RELAY_Pin SUB_SEL_LED_Pin
                           STATUS_Pin */
  GPIO_InitStruct.Pin = OUT5_LED_Pin|OUT6_LED_Pin|VOLUP_LED_Pin|SUB_RELAY_Pin
                          |CENTER_RELAY_Pin|FRONT_SEL_LED_Pin|REAR2_SEL_LED_Pin|REAR_SEL_LED_Pin
                          |CENTER_SEL_LED_Pin|REAR2_RELAY_Pin|REAR_RELAY_Pin|SUB_SEL_LED_Pin
                          |STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_IN_1_Pin GPIO_IN_2_Pin */
  GPIO_InitStruct.Pin = GPIO_IN_1_Pin|GPIO_IN_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_OUT1_Pin GPIO_OUT_2_Pin */
  GPIO_InitStruct.Pin = GPIO_OUT1_Pin|GPIO_OUT_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
