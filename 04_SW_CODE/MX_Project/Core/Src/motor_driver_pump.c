/*
 * MOTOR_DRIVER_PUMP.c
 *
 *  Created on: Nov 27, 2025
 *      Author: lucas
 */


#include <motor_driver_pump.h>

extern TIM_HandleTypeDef htim3;   // Timer utilisé pour PB5 (TIM3_CH2)

static bool pump_state = false;   // Mémorise l'état de la pompe


//Initialisation de la pompe : direction + PWM
void Pump_Init(void)
{
    // IN4 = PB9 = direction -> LOW (sens normal)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

    // Lancer le PWM sur PB5 (TIM3_CH2) avec un duty = 0%
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);

    pump_state = false;
}


//Définit la vitesse de la pompe en % (0-100)

void Pump_SetSpeed(uint8_t duty)
{
    if (duty > 100) duty = 100;

    uint32_t arr = htim3.Instance->ARR;               // Valeur du compteur
    uint32_t pulse = (arr * duty) / 100;              // Calcule du duty cycle

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse);

    pump_state = (duty > 0);
}


// Arrêt de la pompe
void Pump_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    pump_state = false;
}


//  Retourne TRUE si la pompe tourne
bool Pump_IsOn(void)
{
    return pump_state;
}

