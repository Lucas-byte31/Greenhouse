/*
 * error_handler.c
 *
 * Ce module assure la surveillance du système
 * Il détecte les pannes matérielles (capteurs débranchés) ou les pannes
 * d'actionneurs (ventilateur bloqué, réservoir vide) par une analyse temporelle
 *
 * Gestion des erreurs si:
 * Ventisateur HS
 *  ou Pompe HS
 *   ou Capteur Température HS
 *    ou Capteur Humidité HS
 *
 *  Created on: Jan 5, 2026
 *      Author: lucas
 */


#include "error_handler.h"
#include "main.h"   // pour LED_BLINK, GPIO, HAL
#include <stdlib.h>

/* ================== CONFIGURATION DES SEUILS================== */
// Temps d'attente maximum (15s) avant de déclarer une panne si la consigne n'est pas rejointe
#define TEMP_NO_DROP_TIMEOUT_MS     15000

// Seuils de détection de capteur défaillant (valeurs aberrantes)
#define TEMP_MAX_JUMP               80     // °C instantané à confirmer pendant TEMP_NO_DROP_TIMEOUT_MS
#define HUM_MAX_JUMP                98     // % instantané à confirmer pendant TEMP_NO_DROP_TIMEOUT_MS

// Fréquence d'alerte visuelle (LED verte) : 200ms = clignotement rapide
#define LED_BLINK_PERIOD_MS         200
/* ============================================ */

/* État interne - Init à 0*/
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

// Initialisation de l'error handler
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

//Analyse les mesures et déclenche les alertes si nécessaire.
// Si le système est hors consigne, on lance un timer.
// Si le timer expire sans retour à la normale, on lève une erreur.
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
        if (temp <= seuil_temp && temp <= TEMP_MAX_JUMP) // Si la température est OK (sous le seuil) et cohérente
        {
            temp_timer = now_ms;  // OK → reset timer
            temp_error = false;
        }
        else if ((now_ms - temp_timer) > TEMP_NO_DROP_TIMEOUT_MS) // Si on dépasse le délai de 15s sans retour au seuil
        {
            temp_error = true;
            temp_error_reset_timer = now_ms;
        }


    /* ---------- Humidité n'augmente pas malgré la consigne---------- */
        /* ---------- Capteur d'humidité HS ( >98% )---------- */
        if (hum >= seuil_hum && hum <= HUM_MAX_JUMP) // Si l'humidité est OK (au-dessus du seuil) et cohérente
        {
        	hum_timer = now_ms;  // OK → reset timer
        	hum_error = false;
        }
        else if ((now_ms - hum_timer) > TEMP_NO_DROP_TIMEOUT_MS) // Si on dépasse le délai de 15s sans retour au seuil
        {
        	hum_error = true;
        	hum_error_reset_timer = now_ms;
        }



    /* ---------- On active la LED si erreur ---------- */
    if (temp_error || hum_error)
    {
        LED_BLINK(LED_BLINK_PERIOD_MS); // Activation de la LED si erreur
    }
    else
    {
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); // Extinction de la LED si tout est OK
    }

    /* ---------- Historique ---------- */
    last_temp = temp;
}

// Generation des codes erreur
// Retourne retval 0: OK, 1: Erreur Temp, 2: Erreur Hum, 3: Double Erreur
int error_handler_has_error(void)
{
	if (!temp_error && !hum_error) error_code = 0;
	else if (temp_error && !hum_error) error_code = 1;
	else if  (!temp_error && hum_error) error_code = 2;
	else if  (temp_error && hum_error) error_code = 3;
    return (error_code);
}
