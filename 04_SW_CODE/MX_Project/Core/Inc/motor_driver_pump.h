/*
 * MOTOR_DRIVER_PUMP.h
 *
 *  Created on: Nov 27, 2025
 *      Author: lucas
 */

#ifndef INC_MOTOR_DRIVER_PUMP_H_
#define INC_MOTOR_DRIVER_PUMP_H_

#include "main.h"
#include <stdbool.h>

// Initialise la pompe (PWM + Direction)
void Pump_Init(void);

// Définit la vitesse en % (0–100)
void Pump_SetSpeed(uint8_t duty);

// Arrête totalement la pompe
void Pump_Stop(void);

// Retourne l'état (ON/OFF)
bool Pump_IsOn(void);


#endif /* INC_MOTOR_DRIVER_PUMP_H_ */
