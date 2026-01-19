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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
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
#define GPIO_POMPE_Pin GPIO_PIN_9
#define GPIO_POMPE_GPIO_Port GPIOB
#define T_NRST_Pin GPIO_PIN_2
#define T_NRST_GPIO_Port GPIOF
#define T_VCP_TX_Pin GPIO_PIN_2
#define T_VCP_TX_GPIO_Port GPIOA
#define T_VCP_RX_Pin GPIO_PIN_3
#define T_VCP_RX_GPIO_Port GPIOA
#define PUSH_BUTTON2_Pin GPIO_PIN_6
#define PUSH_BUTTON2_GPIO_Port GPIOA
#define PUSH_BUTTON2_EXTI_IRQn EXTI4_15_IRQn
#define PUSH_BUTTON1_Pin GPIO_PIN_7
#define PUSH_BUTTON1_GPIO_Port GPIOA
#define PUSH_BUTTON1_EXTI_IRQn EXTI4_15_IRQn
#define LD3_Pin GPIO_PIN_6
#define LD3_GPIO_Port GPIOC
#define PUSH_BUTTON3_Pin GPIO_PIN_11
#define PUSH_BUTTON3_GPIO_Port GPIOA
#define PUSH_BUTTON3_EXTI_IRQn EXTI4_15_IRQn
#define T_JTMS_Pin GPIO_PIN_13
#define T_JTMS_GPIO_Port GPIOA
#define T_JTCK_Pin GPIO_PIN_14
#define T_JTCK_GPIO_Port GPIOA
#define MOTOR_IN1_Pin GPIO_PIN_3
#define MOTOR_IN1_GPIO_Port GPIOB
#define MOTOR_IN2_Pin GPIO_PIN_4
#define MOTOR_IN2_GPIO_Port GPIOB
#define PWM_POMPE_Pin GPIO_PIN_5
#define PWM_POMPE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
//Definition de la structure de l'etat du LCD
//On viendra la comparer avec l'etat precedent pour voir si on a besoin de rafraîchir la page
typedef struct
{
    int page;
    int t_filt;
    int h_filt;
    int seuil_temperature;
    int seuil_humidite;
    int code_erreur;
    bool pump_on;
    bool fan_on;
} lcd_state_t;


void LED_BLINK(uint32_t blink_period_ms); //prototype pour blinker la LED

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
