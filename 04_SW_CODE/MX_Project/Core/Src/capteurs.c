/*
 * capteurs.c
 *
* Ce module définit les interfaces de conversion pour les capteurs analogiques
 * du système Greenhouse (TMP235 pour la température et SEN0193 pour l'humidité).
 *
 *
 *  Created on: Nov 27, 2025
 *      Author: paul
 */

#include "capteurs.h"


// Convertit la valeur brute de l'ADC en température Celsius.
// Basé sur les spécifications du capteur TMP235.
// raw_adc  : Valeur numérique (0-4095).
// vref     : Tension de référence du microcontrôleur (3.3V).
// v_offset : Décalage de tension mesuré au multimètre (étalonnage).
 // Retourne la Température en degrés Celsius (°C).

float CAPTEURS_ComputeTemperature(uint32_t raw_adc, float vref, float v_offset)
{
    // Conversion brute -> tension
	// Résolution de 12 bits -> 4095 pas.
    float voltage = ((float)raw_adc * vref) / 4095.0f;

    // Correction offset
    // Permet de compenser les chutes de tension dans les câbles ou l'erreur de l'ADC.
    float voltage_corr = voltage - v_offset;
    if (voltage_corr < 0.0f)
        voltage_corr = 0.0f;

    //Application de la courbe caractéristique du TMP235
    // Loi TMP235 : T = 100 * Vcorr - 50
    float temperature = 100.0f * voltage_corr - 50.0f;

    return temperature;
}


// Convertit la valeur brute de l'ADC en pourcentage d'humidité.
// Capteur capacitif (SEN0193). La valeur diminue quand l'humidité augmente.
// raw_adc     : Valeur numérique brute.
// air_value   : Valeur lue par l'ADC à l'air sec (étalonnage min).
//  water_value : Valeur lue par l'ADC plongé dans l'eau (étalonnage max).
// Retouene Humidité relative en pourcentage (0 à 100%).
float CAPTEURS_ComputeHumidity(uint32_t raw_adc, float air_value, float water_value)
{
    float Air   = air_value;
    float Water = water_value;

    // securite : eviter division par 0
    if (Air == Water)
        return 0.0f;

    // Formule d'interpolation linéaire :
    // Plus la valeur ADC est proche de 'Water', plus le pourcentage est élevé.
    // H(%) = (Air - raw) / (Air - Water) * 100
    float H = (Air - (float)raw_adc) * 100.0f / (Air - Water);

    // Saturation : empêche d'afficher des valeurs incohérentes (<0 ou >100)
    // dues aux variations de température ou de salinité du sol.
    if (H < 0.0f)   H = 0.0f;
    if (H > 100.0f) H = 100.0f;

    return H;
}

