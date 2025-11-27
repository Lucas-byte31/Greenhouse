/*
 * capteurs.h
 *
 *  Created on: Nov 27, 2025
 *      Author: paul
 */

#ifndef CAPTEURS_H
#define CAPTEURS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calcule la temperature en °C a partir de la valeur ADC brute.
 * @param raw_adc   : valeur ADC (0..4095)
 * @param vref      : tension de reference (ex: 3.3f)
 * @param v_offset  : offset a retirer (en Volts), ex: 0.130f
 * @return          : temperature en degres Celsius (float)
 *
 * Formule utilisee (TMP235) :
 *   V = raw * vref / 4095
 *   Vcorr = max(V - v_offset, 0)
 *   T(°C) = 100 * Vcorr - 50
 */
float CAPTEURS_ComputeTemperature(uint32_t raw_adc, float vref, float v_offset);

/**
 * @brief Calcule l'humidite en % a partir de la valeur ADC brute.
 * @param raw_adc    : valeur ADC (0..4095)
 * @param air_value  : valeur ADC en air sec
 * @param water_value: valeur ADC dans l'eau (100 %)
 * @return           : humidite en %, saturee entre 0 et 100
 *
 * Formule (capteur capacitif type SEN0193, raw diminue quand l'humidite augmente) :
 *   H(%) = (Air - raw) / (Air - Water) * 100
 */
float CAPTEURS_ComputeHumidity(uint32_t raw_adc, float air_value, float water_value);

#ifdef __cplusplus
}
#endif

#endif // CAPTEURS_H
