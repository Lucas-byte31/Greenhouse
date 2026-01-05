/*
 * error_handler.c
 *
 * Gestion des erreurs si:
 * Ventisateur HS
 *  ou Pompe HS
 *   ou Push Button HS
 *    ou LCD HS
 *     ou Capteur Température HS
 *      ou Capteur Humidité HS
 *
 *  Created on: Jan 5, 2026
 *      Author: lucas
 */


#include "error_handler.h"
#include "main.h"   // pour LED_BLINK, GPIO, HAL
#include <stdlib.h>

/* ================== CONFIG ================== */
#define TEMP_NO_DROP_TIMEOUT_MS     6000
#define HUM_NO_RISE_TIMEOUT_MS      6000
#define ERROR_RESET_TIMEOUT_MS      2500  // Temps pour réinitialiser l'erreur

#define TEMP_MAX_JUMP               15     // °C instantané
#define HUM_MAX_JUMP                60     // % instantané

#define LED_BLINK_PERIOD_MS         200
/* ============================================ */

/* État interne */
static int last_temp = 0;
static int last_hum  = 0;

static uint32_t temp_timer = 0;
static uint32_t hum_timer  = 0;
static uint32_t temp_error_reset_timer = 0;
static uint32_t hum_error_reset_timer  = 0;

static bool temp_error = false;
static bool hum_error  = false;

/* ================== API ================== */

void error_handler_init(int temp_init, int hum_init)
{
    last_temp = temp_init;
    last_hum  = hum_init;
    temp_timer = 0;
    hum_timer  = 0;
    temp_error_reset_timer = 0;
    hum_error_reset_timer  = 0;
    temp_error = false;
    hum_error  = false;
}

void error_handler_update(int temp,
                          int hum,
                          bool fan_on,
                          bool pump_on,
                          uint32_t now_ms)
{
    /* ---------- Détection sauts anormaux ---------- */
    if (abs(temp - last_temp) > TEMP_MAX_JUMP)
    {
        temp_error = true;
        temp_error_reset_timer = now_ms;  // Démarre le timer de réinitialisation
    }
    else if (temp_error && (now_ms - temp_error_reset_timer) > ERROR_RESET_TIMEOUT_MS)
    {
        temp_error = false;  // Réinitialise l'erreur après un délai stable
    }

    if (abs(hum - last_hum) > HUM_MAX_JUMP)
    {
        hum_error = true;
        hum_error_reset_timer = now_ms;
    }
    else if (hum_error && (now_ms - hum_error_reset_timer) > ERROR_RESET_TIMEOUT_MS)
    {
        hum_error = false;
    }

    /* ---------- Température ne baisse pas ---------- */
    if (fan_on)
    {
        if (temp < last_temp)
        {
            temp_timer = now_ms;  // OK → reset timer
        }
        else if ((now_ms - temp_timer) > TEMP_NO_DROP_TIMEOUT_MS)
        {
            temp_error = true;
            temp_error_reset_timer = now_ms;
        }
    }
    else
    {
        temp_timer = now_ms;  // ventilateur OFF → pas de diagnostic
    }

    /* ---------- Humidité ne monte pas ---------- */
    if (pump_on)
    {
        if (hum > last_hum)
        {
            hum_timer = now_ms;
        }
        else if ((now_ms - hum_timer) > HUM_NO_RISE_TIMEOUT_MS)
        {
            hum_error = true;
            hum_error_reset_timer = now_ms;
        }
    }
    else
    {
        hum_timer = now_ms;
    }

    /* ---------- Action LED ---------- */
    if (temp_error || hum_error)
    {
        LED_BLINK(LED_BLINK_PERIOD_MS);
    }
    else
    {
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    }

    /* ---------- Historique ---------- */
    last_temp = temp;
    last_hum  = hum;
}

bool error_handler_has_error(void)
{
    return (temp_error || hum_error);
}
