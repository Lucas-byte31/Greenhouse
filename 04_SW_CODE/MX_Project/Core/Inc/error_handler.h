/*
 * error_handler.h
 *
 *  Created on: Jan 5, 2026
 *      Author: lucas
 */

#ifndef INC_ERROR_HANDLER_H_
#define INC_ERROR_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>

/* Initialisation */
void error_handler_init(int temp_init, int hum_init);

/* Mise à jour périodique */
void error_handler_update(int temp,
                          int hum,
                          bool fan_on,
                          bool pump_on,
                          uint32_t now_ms);

/* État global erreur */
bool error_handler_has_error(void);


#endif /* INC_ERROR_HANDLER_H_ */
