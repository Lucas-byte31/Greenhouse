/*
 * capteurs.c
 *
 *  Created on: Nov 27, 2025
 *      Author: paul
 */

#include "capteurs.h"

float CAPTEURS_ComputeTemperature(uint32_t raw_adc, float vref, float v_offset)
{
    // Conversion brute -> tension
    float voltage = ((float)raw_adc * vref) / 4095.0f;

    // Correction offset
    float voltage_corr = voltage - v_offset;
    if (voltage_corr < 0.0f)
        voltage_corr = 0.0f;

    // Loi TMP235 : T = 100 * Vcorr - 50
    float temperature = 100.0f * voltage_corr - 50.0f;

    return temperature;
}

float CAPTEURS_ComputeHumidity(uint32_t raw_adc, float air_value, float water_value)
{
    float Air   = air_value;
    float Water = water_value;

    // securite : eviter division par 0
    if (Air == Water)
        return 0.0f;

    // H(%) = (Air - raw) / (Air - Water) * 100
    float H = (Air - (float)raw_adc) * 100.0f / (Air - Water);

    if (H < 0.0f)   H = 0.0f;
    if (H > 100.0f) H = 100.0f;

    return H;
}

