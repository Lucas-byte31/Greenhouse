/*
 * lcd_page_manager.h
 *
 *  Created on: Dec 30, 2025
 *      Author: lucas
 */

#ifndef INC_LCD_PAGE_MANAGER_H_
#define INC_LCD_PAGE_MANAGER_H_

#include <main.h>

/* -------------------------------------------------------------------------- */
/*                    Fonctions publiques LCD PAGE MANAGER                    */
/* -------------------------------------------------------------------------- */

void lcd_page_1(lcd_state_t *curr, lcd_state_t *prev);
void lcd_page_2(lcd_state_t *curr, lcd_state_t *prev);
void lcd_page_3(lcd_state_t *curr, lcd_state_t *prev);
void lcd_page_4(lcd_state_t *curr, lcd_state_t *prev);
void lcd_page_5(lcd_state_t *curr, lcd_state_t *prev);

#endif /* INC_LCD_PAGE_MANAGER_H_ */
