/*
 * motor_driver.c
 *
 *  Created on: Nov 14, 2025
 *      Author: lucas
 */

// Include de motor_driver
#include "motor_driver_fan.h"

// Global data declaration et affectation
// Etat du moteur: OFF =0 et ON =1
static uint8_t G_MotorState = 0;


void Motor_On(void)
{
    // Rotation du Moteur : IN1 = 1 et IN2 = 0
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
    G_MotorState = 1;
}

void Motor_Off(void)
{
    // Arrêt du Moteur : IN1 = 0 et IN2 = 0
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
    G_MotorState = 0;
}


uint8_t Motor_IsOn(void)
{
    return G_MotorState;
}
