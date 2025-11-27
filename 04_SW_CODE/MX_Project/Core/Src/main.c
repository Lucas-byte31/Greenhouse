/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <motor_driver_fan.h>
#include <motor_driver_pump.h>
#include "capteurs.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Calibration humidite SEN0193 - base DFRobot adaptée en 12 bits */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

//Push button
volatile bool PB1_pressed = false;   // Flag PB1
volatile bool PB2_pressed = false;   // Flag PB2
volatile bool PB3_pressed = false;   // Flag PB3
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
static void LED_BLINK(int p_blink);
float TMP235_linear(float voltage);
float humidity_from_raw(uint32_t raw);
uint32_t ReadADC_Channel(uint32_t channel);


#define V_REF      3.3f		 // Vref 3.3V
#define V_OFFSET   0.130f    // ton offset mesure a 0V

#define AIR_VALUE   2675.0f  // mesure en air sec
#define WATER_VALUE 1780.0f  // mesure dans l'eau
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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  //Lcd_Set_Cursor(0, 0);
  //Lcd_Write_String("HELLO WORLD");
  //Lcd_Set_Cursor(1, 0);
  //Lcd_Write_String("from CESI");

  // On initialise le moteur a OFF
  Motor_Off();
  // On initialise la pompe a OFF
  Pump_Init(); // init de la pompe
  
  //Init de la page du LCD
  int Var_page = 1;

  int seuil_temperature = 25;  // exemple
  int seuil_humidite = 50;     // exemple

  // Initialisation LCD
  lcd_init();
  lcd_clear();

  // Affichage
  lcd_position_cursor(0,0);
  lcd_show_txt("HELLO WORLD");

  lcd_position_cursor(0,1);
  lcd_show_txt("STM32G031U");


  char buffert[80];
  char bufferh[80];

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


	  /* ================== HUMIDITE sur PA0 = ADC1_IN0 ================== */

	      uint32_t raw_hum = ReadADC_Channel(ADC_CHANNEL_0);
	      float humidity = CAPTEURS_ComputeHumidity(raw_hum, AIR_VALUE, WATER_VALUE);

	      int h100 = (int)(humidity * 100.0f);
	      int h_int = h100 / 100;
	      int h_dec = abs(h100 % 100);

	      sprintf(bufferh,
	              "H: rawHum=%lu  Humidite=%d.%02d %%\r\n",
	              raw_hum, h_int, h_dec);
	      HAL_UART_Transmit(&huart2, (uint8_t*)bufferh, strlen(bufferh), HAL_MAX_DELAY);

	      /* ============== TEMPERATURE sur PA1 = ADC1_IN1 (TMP235) ============== */

	      uint32_t raw_temp = ReadADC_Channel(ADC_CHANNEL_1);
	      float temperature = CAPTEURS_ComputeTemperature(raw_temp, V_REF, V_OFFSET);

	      int t100 = (int)(temperature * 100.0f);
	      int t_int = t100 / 100;
	      int t_dec = abs(t100 % 100);

	      sprintf(buffert,
	              "T: raw=%lu  T=%d.%02d C\r\n",
	              raw_temp, t_int, t_dec);
	      HAL_UART_Transmit(&huart2, (uint8_t*)buffert, strlen(buffert), HAL_MAX_DELAY);


	     /* // ===================== Ventilateur Automatique=====================
	      if (temperature >= 30.0)
	      {
	              Motor_On();
	              HAL_Delay(2000);
	      } else {
	    	  	  Motor_Off();
	      }

	      // ===================== Pompe Automatique=====================
	      if (humidity <=10.0)
	      {
	    	  	  Pump_SetSpeed(100);
	      } else {
	    	  	  Pump_Stop();
	      }*/


	        //----------------------------------CODE DE LUCAS!!!-----------------------------------//
    /*  // ===================== Ventilateur Manuel=====================
      if (PB1_pressed)
      {
          PB1_pressed = false;   // On remet PB à false
          if (Motor_IsOn()) // si le moteur est déjà allumé on l'éteind
              Motor_Off();
          else				// si le moteur est éteind on l'allume
              Motor_On();
      }

      // ===================== Pompe Manuelle=====================
      if (PB2_pressed)
      {
          PB2_pressed = false;   // On remet PB à false
          if (Pump_IsOn())		// si la pompe est déjà allumé on l'éteind
              Pump_Stop();
          else
              Pump_SetSpeed(100);   // // si la pompe est éteinte on l'allume à x% PWM
      }
*/

	      /* Changement de page si PB1 appuyé */
	      if (PB1_pressed)
	      {
	          PB1_pressed = false;       // on consomme l'événement

	          Var_page++;
	          if (Var_page > 4)
	              Var_page = 1;          // retour à la page 1
	      }

	      /* Rafraîchissement écran LCD suivant Var_page */
	      lcd_clear();

	      switch (Var_page)
	         {
	             case 1:
	                 // -------- PAGE 1 --------
	                 // Temperature     x°C
	                 // Humidite        x%

	            	 lcd_position_cursor(0, 0);
	                 char line1[16];
	                 sprintf(line1, "Temp: %dC", t_int);   // t_int : valeur déjà calculée
	                 lcd_show_txt(line1);

	                 lcd_position_cursor(0, 1);
	                 char line2[16];
	                 sprintf(line2, "Hum: %d%%", h_int);   // h_int : valeur humidité
	                 lcd_show_txt(line2);
	                 break;

	             case 2:
	                 // -------- PAGE 2 --------
	                 // Temperature     x°C
	                 // Seuil           x°C

	            	 lcd_position_cursor(0, 0);
	                 sprintf(line1, "Temp: %dC", t_int);
	                 lcd_show_txt(line1);

	                 lcd_position_cursor(0, 1);
	                 sprintf(line2, "Seuil: %dC", seuil_temperature);
	                 lcd_show_txt(line2);
	                 break;

	             case 3:
	                 // -------- PAGE 3 --------
	                 // Humidite        x%
	                 // Seuil           x%

	            	 lcd_position_cursor(0, 0);
	                 sprintf(line1, "Hum: %d%%", h_int);
	                 lcd_show_txt(line1);

	                 lcd_position_cursor(0, 1);
	                 sprintf(line2, "Seuil: %d%%", seuil_humidite);
	                 lcd_show_txt(line2);
	                 break;

	             case 4:
	                 // -------- PAGE 4 --------
	                 // Pompe       ON/OFF
	                 // Ventilateur ON/OFF

		           // ===================== Ventilateur Manuel=====================
		            if (PB2_pressed)
		            {
		               PB2_pressed = false;   // On remet PB à false
		                if (Motor_IsOn()) // si le moteur est déjà allumé on l'éteind
		                    Motor_Off();
		                else				// si le moteur est éteind on l'allume
		                    Motor_On();
		            }

		            // ===================== Pompe Manuelle=====================
		            if (PB3_pressed)
		            {
		                PB3_pressed = false;   // On remet PB à false
		                if (Pump_IsOn())		// si la pompe est déjà allumé on l'éteind
		                    Pump_Stop();
		                else
		                    Pump_SetSpeed(100);   // // si la pompe est éteinte on l'allume à x% PWM
		            }

	            	 lcd_position_cursor(0, 0);
	                 sprintf(line1, "Pompe: %s", Pump_IsOn() ? "ON" : "OFF");
	                 lcd_show_txt(line1);

	                 lcd_position_cursor(0, 1);
	                 sprintf(line2, "Ventil: %s", Motor_IsOn() ? "ON" : "OFF");
	                 lcd_show_txt(line2);

	                 break;
	         }

	         HAL_Delay(1500);  // petit délai pour éviter de rafraîchir trop souvent

      // ===================== LED =====================
      //LED_BLINK(500);
	  
	  	      //HAL_Delay(2000);


	        //----------------------------------CODE DE LUCAS!!!-----------------------------------//
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 799;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_POMPE_Pin|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |MOTOR_IN1_Pin|MOTOR_IN2_Pin|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GPIO_POMPE_Pin PB0 PB1 PB2
                           MOTOR_IN1_Pin MOTOR_IN2_Pin PB6 PB7
                           PB8 */
  GPIO_InitStruct.Pin = GPIO_POMPE_Pin|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |MOTOR_IN1_Pin|MOTOR_IN2_Pin|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : T_NRST_Pin */
  GPIO_InitStruct.Pin = T_NRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(T_NRST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PUSH_BUTTON2_Pin PUSH_BUTTON1_Pin PUSH_BUTTON3_Pin */
  GPIO_InitStruct.Pin = PUSH_BUTTON2_Pin|PUSH_BUTTON1_Pin|PUSH_BUTTON3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void LED_BLINK(int p_blink)
{
	int i_blinking = p_blink;
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);  // Change l'état de la LED
    HAL_Delay(i_blinking);   // attente
}


// Interruption
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    // Anti-rebond simple
    static uint32_t last_interrupt_time = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_interrupt_time < 200) return;  // 200 ms debounce
    last_interrupt_time = now;

    if (GPIO_Pin == PUSH_BUTTON1_Pin)
    {
        PB1_pressed = true;   // Flag PB1
    }

    if (GPIO_Pin == PUSH_BUTTON2_Pin)
    {
        PB2_pressed = true;   // Flag PB2
    }

    if (GPIO_Pin == PUSH_BUTTON3_Pin)
    {
        PB3_pressed = true;   // Flag PB3
    }
}

/* Lecture d un canal ADC1 quelconque */
uint32_t ReadADC_Channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel      = channel;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);

    return HAL_ADC_GetValue(&hadc1);
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
