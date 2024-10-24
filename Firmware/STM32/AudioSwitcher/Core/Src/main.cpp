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
#include "LM1972.h"
#include "MAX14661.h"
#include "outputControl.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX14661_ADDRESS	0x4C		// 7-bit address
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */
MAX14661*	outputMUX;
LM1972*		volumeController;
//LM1972*		volumeControlIC;

Button	output1({OUT1_SW_GPIO_Port, OUT1_SW_Pin});		// Towers
Button	output2({OUT2_SW_GPIO_Port, OUT2_SW_Pin});		// BookShelfs
Button	output3({OUT3_SW_GPIO_Port, OUT3_SW_Pin});		// Center Channel Bookshelves and heights
Button	output4({OUT4_SW_GPIO_Port, OUT4_SW_Pin});		// Add Subwoofer
Button	output5({OUT5_SW_GPIO_Port, OUT5_SW_Pin});		// Everything
//Button	output6({OUT6_SW_GPIO_Port, OUT6_SW_Pin});


Output* outputs[NUM_OF_OUTPUTS];//(outputMUX, {});
//Output* front;//(outputMUX, {});
//Output* rear;//(outputMUX, {});
//Output* center;//(outputMUX, {});
//Output* sub;//(outputMUX, {});
//Output* rear2;//();

OutputControl* outputControl[NUM_OF_OUTPUTS];
//OutputControl* frontControl;
//OutputControl* rearControl;
//OutputControl* centerControl;
//OutputControl* subControl;
//OutputControl* rear2Control;


uint8_t	outputStateFlag = 0;
uint64_t timeStamp = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
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
  MX_I2C2_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  outputMUX = new MAX14661(&hi2c2, MAX14661_ADDRESS);
  volumeController = new LM1972(&hspi2, {SPI2_CS_GPIO_Port, SPI2_CS_Pin}, {VOLUP_SW_GPIO_Port, VOLUP_SW_Pin}, {VOLDWN_SW_GPIO_Port, VOLDWN_SW_Pin}, {VOLUP_LED_GPIO_Port, VOLUP_LED_Pin}, {VOLDWN_LED_GPIO_Port, VOLDWN_LED_Pin});

  outputs[0] = new Output(outputMUX, {OUT1_LED_GPIO_Port,OUT1_LED_Pin}, 0, 1);
  outputs[1] = new Output(outputMUX, {OUT2_LED_GPIO_Port,OUT2_LED_Pin}, 3, 4);
  outputs[2] = new Output(outputMUX, {OUT3_LED_GPIO_Port,OUT3_LED_Pin}, 6, 7);
  outputs[3] = new Output(outputMUX, {OUT4_LED_GPIO_Port,OUT4_LED_Pin}, 8, 9);
  outputs[4] = new Output(outputMUX, {OUT5_LED_GPIO_Port,OUT5_LED_Pin}, 11, 12);

  outputControl[0] = new OutputControl(&output1,outputs[0]);
  outputControl[1] = new OutputControl(&output2,outputs[1]);
  outputControl[2] = new OutputControl(&output3,outputs[2]);
  outputControl[3] = new OutputControl(&output4,outputs[3]);
  outputControl[4] = new OutputControl(&output5,outputs[4]);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  volumeController->processButtons();

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
		  HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin, GPIO_PIN_RESET);
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
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00000003;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_1LINE;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, MUTE_Pin|OUT1_LED_Pin|OUT2_LED_Pin|OUT3_LED_Pin
                          |OUT4_LED_Pin|VOLDWN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OUT5_LED_Pin|OUT6_LED_Pin|VOLUP_LED_Pin|FRONT_SEL_LED_Pin
                          |REAR2_SEL_LED_Pin|REAR_SEL_LED_Pin|CENTER_SEL_LED_Pin|SUB_SEL_LED_Pin
                          |STATUS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : MUTE_Pin OUT1_LED_Pin OUT2_LED_Pin OUT3_LED_Pin
                           OUT4_LED_Pin VOLDWN_LED_Pin */
  GPIO_InitStruct.Pin = MUTE_Pin|OUT1_LED_Pin|OUT2_LED_Pin|OUT3_LED_Pin
                          |OUT4_LED_Pin|VOLDWN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT1_SW_Pin VOLUP_SW_Pin VOLDWN_SW_Pin */
  GPIO_InitStruct.Pin = OUT1_SW_Pin|VOLUP_SW_Pin|VOLDWN_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT2_SW_Pin OUT3_SW_Pin */
  GPIO_InitStruct.Pin = OUT2_SW_Pin|OUT3_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT4_SW_Pin OUT5_SW_Pin OUT6_SW_Pin */
  GPIO_InitStruct.Pin = OUT4_SW_Pin|OUT5_SW_Pin|OUT6_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT5_LED_Pin OUT6_LED_Pin VOLUP_LED_Pin FRONT_SEL_LED_Pin
                           REAR2_SEL_LED_Pin REAR_SEL_LED_Pin CENTER_SEL_LED_Pin SUB_SEL_LED_Pin
                           STATUS_Pin */
  GPIO_InitStruct.Pin = OUT5_LED_Pin|OUT6_LED_Pin|VOLUP_LED_Pin|FRONT_SEL_LED_Pin
                          |REAR2_SEL_LED_Pin|REAR_SEL_LED_Pin|CENTER_SEL_LED_Pin|SUB_SEL_LED_Pin
                          |STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI2_CS_Pin */
  GPIO_InitStruct.Pin = SPI2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI2_CS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

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
