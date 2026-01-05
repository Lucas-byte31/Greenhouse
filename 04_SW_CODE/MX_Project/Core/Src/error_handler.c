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
#define TEMP_NO_DROP_TIMEOUT_MS     15000

#define TEMP_MAX_JUMP               80     // °C instantané à confirmer pendant xsecondes
#define HUM_MAX_JUMP                98     // % instantané à confirmer pendant xsecondes

#define LED_BLINK_PERIOD_MS         200
/* ============================================ */

/* État interne */
static int last_temp = 0;
static int last_hum  = 0;

static uint32_t temp_timer = 0;
static uint32_t hum_timer  = 0;
static uint32_t temp_error_reset_timer = 0;
static uint32_t hum_error_reset_timer  = 0;
static uint32_t error_code = 0;

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
						  int seuil_temp,
						  int seuil_hum,
                          bool fan_on,
                          bool pump_on,
                          uint32_t now_ms)
{

    /* ---------- Température ne baisse pas malgré la consigne---------- */
    	/* ---------- Capteur de température HS ( >80°C )---------- */
        if (temp <= seuil_temp && temp <= TEMP_MAX_JUMP)
        {
            temp_timer = now_ms;  // OK → reset timer
            temp_error = false;
        }
        else if ((now_ms - temp_timer) > TEMP_NO_DROP_TIMEOUT_MS)
        {
            temp_error = true;
            temp_error_reset_timer = now_ms;
        }


    /* ---------- Humidité n'augmente pas malgré la consigne---------- */
        /* ---------- Capteur d'humidité HS ( >98% )---------- */
        if (hum >= seuil_hum && hum <= HUM_MAX_JUMP)
        {
        	hum_timer = now_ms;  // OK → reset timer
        	hum_error = false;
        }
        else if ((now_ms - hum_timer) > TEMP_NO_DROP_TIMEOUT_MS)
        {
        	hum_error = true;
        	hum_error_reset_timer = now_ms;
        }



    /* ---------- On active la LED si erreur ---------- */
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
}

int error_handler_has_error(void)
{
	if (!temp_error && !hum_error) error_code = 0;
	else if (temp_error && !hum_error) error_code = 1;
	else if  (!temp_error && hum_error) error_code = 2;
	else if  (temp_error && hum_error) error_code = 3;
    return (error_code);
}
