/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "command.h"
#include "ADS1115.h"
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
extern uint8_t Command_Index;
extern uint8_t Command_List[10][128];

uint16_t config_set = 0x0583;
DataRate dr = RATE_128;
OPREATION_MODE mode = SS_MODE;
PGA pga = FSR_2d048;
MUX mux = DIFF01;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_I2C3_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	Command_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (Command_Index != 0)
	  {
		  Command_Excute(&Command_List[0][0]);
		  Command_Index = 0;
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Func_div(uint8_t* command)
{
	uint8_t value = strtol(command + 20, '\0', 10);
	
	// fast refresh
	if (strcmp(command + 10, "_defaut") == 0)
	{
		dr = RATE_128;
		mode = SS_MODE;
		pga = FSR_2d048;
		mux = DIFF01;
		config_set |= (dr << 5);
		config_set |= (mode << 8);
		config_set |= (pga << 9);
		config_set |= (mux << 12);
		ADSConfig(config_set);
	}
	// change data rate
	else if (strcmp(command + 10, "_DR") == 0)
	{
		dr = value;
		config_set |= (dr << 5);
		ADSConfig(config_set);
	}
	// change oprating mode
	else if (strcmp(command + 10, "_MODE") == 0)
	{
		mode = value;
		config_set |= (mode << 8);
		ADSConfig(config_set);
	}
	// change Gain Amplifier
	else if(strcmp(command + 10, "_PGA") == 0)
	{
		pga = value;
		config_set |= (pga << 9);
		ADSConfig(config_set);
	}
	// change multiplexer configuration
	else if(strcmp(command + 10, "_MUX") == 0)
	{
		mux = value;
		config_set |= (mux << 12);
		ADSConfig(config_set);
	}
	else
	{
		printf("Argument incorrect\r\n");
		printf("format is like \"config _DR 4\" ");
	}
		
}


void Func_trig(uint8_t* command)
{
	double gain = 0;
	double data;
	if (pga == FSR_d256)
		gain = 0.256;
	else if (pga == FSR_d512)
		gain = 0.512;
	else if (pga == FSR_1d024)
		gain = 1.024;
	else if (pga == FSR_2d048)
		gain = 2.048;
	else if (pga == FSR_4d096)
		gain = 4.096;
	else if (pga == FSR_6d144)
		gain = 6.144;
	else
	{
		gain = 2.048;
	}
		
	uint8_t value = strtol(command + 20,'\0', 10);
	if (strcmp(command + 10, "_defaut") == 0)
	{
		for (int i = 0; i != 1024; i++) {
			ADSReadADC(&data);
			printf("input data%d is %f\r\n", i, data);
		}
	}
	putchar0('\n');
	HAL_Delay(8000);
	
}		


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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
